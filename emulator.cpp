#include "emulator.h"

Emulator::Emulator()
{
    logFile.setFileName("Logs/" + QCoreApplication::applicationName() + '_' + QDate::currentDate().toString("yyyy-MM-dd") + ".log");
    logFile.open(QFile::WriteOnly | QIODevice::Append | QFile::Text);
    logStream.setDevice(&logFile);
    regs_to_zero();
}

Emulator::~Emulator()
{
    logFile.close();
}

void Emulator::Prepare_response()
{
    clear_log();
    ready_to_send_response = true;// не нужно?
    //Первым делом смотрим на порядок байт, так как эмулятор написан для младшего порядка
    //Создаем тестовый заголовок, по которому определяем поряд
    PacketHeader request_pack_header = PacketHeader(request[0]);
    //Если порядок старший, то меняем порядок байт
    if(request_pack_header.ProtocolVersion == 0xf){
        Request_to_little_endian();
        littleEndian = false;
    }
    //Далее анализируем, и, если все в порядке, составляем заголовок пакета
    Prepare_Header();
    // Если заголовк пакета составлен и мы готовы с ним работать, то выполняется следующее
    if(ready_to_send_response)
    {
        // Если заголовок пакета типа статус и при этом пакет пришел в старшем порядке байт, то
        if(PacketHeader(request[0]).PacketType == 0x1 && request_pack_header.ProtocolVersion == 0xf)// Тип пакета и старший порядок байт обязательны
        {
            // Проверяем, размер пакета, исходя из правил протокола
            if(requestSize == 16 * Word_size)
            {
                //Если в пакете содержится хотя бы одно ненулевое слово, то это несоответствие протоколу
                for(quint8 i = 1; i < requestSize / 4; i++)
                    if(request[i] != 0x0)
                    {
                        ready_to_send_response = false;
                        log('<'+ QTime::currentTime().toString("hh:mm:ss") +">ERROR: Invalid Status Packet: non zero value somewhere");
                        break;
                    }
            }
            // Если размер пакета неверный, то отправлять его мы не будем
            else{
                ready_to_send_response = false;
                log('<'+ QTime::currentTime().toString("hh:mm:ss") +">ERROR: Invalid Status Packet: incorrect packet size");
            }

            response[1] = 0xF8070000;
            response[2] = 0x10000000;
            response[3] = 0xF0020020;
            response[4] = 0x0f0f0f0f;
            response[5] = 0x0f0f0f0f;
            response[6] = 0x0f0f0f0f;
            response[7] = 0x0e070f0f;
            response[8] = 0x200000F0;
            response[9] = 0x200000F0;
            response[10] =0x200000F0;
            response[11] =0x200000F0;
            response[12] =0x200000F0;
            response[13] =0x200000F0;
            response[14] =0x200000F0;
            response[15] =0x200000F0;

            //Если все верно, то не читаем пакет, сразу перекидываем счетчик
            counter += 15;
            //Размер пакета увеличиваем на 15
            responseSize += 15 * Word_size;
            log('<'+ QTime::currentTime().toString("hh:mm:ss") +"> Status packet  (" + (requestSize / 4 > 9 ? (requestSize / 4 > 99 ? "" : " ") : "  ") + QString::number(requestSize / 4) + " words " + (littleEndian ? 'L' : 'B') + "E)");
                    }
        // Если пакет типа control, то выплняем обработку транзакций
        if(PacketHeader(request[0]).PacketType == 0x0){
                log('<'+ QTime::currentTime().toString("hh:mm:ss") +"> Control packet (" + (requestSize / 4 > 9 ? (requestSize / 4 > 99 ? "" : " ") : "  ") + QString::number(requestSize / 4) + " words " + (littleEndian ? 'L' : 'B') + "E)");
                // Пока счетчик умноженный на размер 32 битного слова меньше чем размер запроса
                while(4 * counter < requestSize)
                   {// Готовим ответную транзакцию (причем дальше счетчик идет в самой транзакции)
                    Prepare_Transaction_response();
                    //Сколько транзакций пришло (для лога)
                }
                // После это записываем составленный пакет в Хэш буффер
                response_to_buffer();
                // Если ID был ненулевым, то следующий должен быть на 1 больше
                if(request_pack_header.PacketID != 0x0)
                    ++NextPacketID;

        }
        // Если пакет типа re-send и для него характерен старший порядок байт
        if(PacketHeader(request[0]).PacketType == 0x2 && request_pack_header.ProtocolVersion == 0xf)// Тип пакета и старший порядок байт обязательны
            //Ищем в хэше пакет с таким же идентификатором как и у request
        {
            log('<'+ QTime::currentTime().toString("hh:mm:ss") +"> ReSend packet\t(" + (requestSize / 4 > 9 ? (requestSize / 4 > 99 ? "" : " ") : "  ") + " words " + (littleEndian ? 'L' : 'B') + "E)");
            buffer_to_response();
        }

    }
    // Если пакет пришел к нам в старшем порядке байт, то мы возвращаем этот порядок
    if(request_pack_header.ProtocolVersion == 0xf)
        Response_to_Big_endian();
}

void Emulator::Prepare_Header()
{
//    qDebug()<<"Готовлю заголовок пакета";
    // Заголовок пакета, по типу которого определяем что это за пакет
    PacketHeader request_pack_header = PacketHeader(request[0]);
    // Если заголовок соответствует типу протокола IPbus, то мы формируем заголовок ответного пакета
    if(request_pack_header.Rsvd == 0\
            && request_pack_header.ProtocolVersion == 2\
            && request_pack_header.PacketType < 3)
    {
        response[0]=quint32(PacketHeader(request_pack_header.PacketType, request_pack_header.PacketID));
        // Увеличиваем размер ответного пакета на 1
        responseSize += Word_size;
        // Считываем запрос дальше
        counter = 1;
//    qDebug()<<"Заголовок пакета успешно создан";
    }
    else{
        //Заголовок пакета не соответствует протоколу, значит не готовы отправлять ответ
        log('<'+ QTime::currentTime().toString("hh:mm:ss") +"> Invalid Packet Type (" + QString::number(requestSize) +')');
        ready_to_send_response = false;
    }

}

void Emulator::Prepare_Transaction_response()
{
    // Заголовок транзакции, по типу которого, определяем, что с ним делать
    TransactionHeader trans_header = TransactionHeader(request[counter]);
    // Если заголовк соответствует правилам заголовка протокола IPbus
    if(trans_header.ProtocolVersion == 2 && trans_header.InfoCode == 0xf){
                switch (trans_header.TypeID) {
                case read:                 /*qDebug()<<"Обработка транзакции чтения";*/
                                           Read_transaction(trans_header);
                                           break;
                case nonIncrementingRead:  /*qDebug()<<"Обработка транзакции FIFO  чтения";*/
                                           Non_Incrementing_read_transaction(trans_header);
                                           break;
                case write:                /*qDebug()<<"Обработка транзакции записи";*/
                                           Write_transaction(trans_header);
                                           break;
                case nonIncrementingWrite: /*qDebug()<<"Обработка транзакции FIFO записи";*/
                                           Non_Incrementing_write_transaction(trans_header);
                                           break;
                case RMWsum:               /*qDebug()<<"Обработка транзакции побитового сложения";*/
                                           RMWsum_transaction(trans_header);
                                           break;
                case RMWbits:              /*qDebug()<<"Обработка транзакции побитовых операций";*/
                                           RMWbits_transaction(trans_header);
                                           break;
                case configurationRead:    trans_header.InfoCode = 0x0;
                                           response[counter]=quint32(trans_header);
                                           break;
                case configurationWrite:   trans_header.InfoCode = 0x0;
                                           response[counter]=quint32(trans_header);
                                           break;
                // В случае неопределенного типа транзакции считаем дальше
                default:                   counter += 1;
                                           //responseSize += Word_size;
                                           break;
                }
            }
            else{
                // Иначе возвращаем заголовок транзакции с ошибкой о неверном заголовке
//                response[counter] = quint32(TransactionHeader(trans_header.TypeID,trans_header.Words,trans_header.TransactionID, 0x1));
        ++counter;
    }
}

// При наличии нескольких транзакций в пакете, функции работают нормально.
// Определить вывод при переполнении, недозаполнении и т. д.

void Emulator::Write_transaction(TransactionHeader th){
    th.InfoCode = 0;
    bool read_only = false;
    for(quint8 i = 0; i < th.Words; ++i)
    {
        if(counter + 2 + i == requestSize / 4){
            th.InfoCode = 0x1;
            th.Words = i;
            log("writing  impossible: Bad header");
        }else{
            read_only = bd.read_only(request[counter + 1] + i);
            bd.set_registers(adress_space, request[counter + 1] + i,  request[counter + 2 + i]);
            log(bd.get_message());
            th.InfoCode = read_only ? 0x5 : 0x0;
            if(read_only){
                th.Words = i;
                break;
            }
        }
    }
    counter = read_only ? requestSize / 4 : (2 + th.Words + counter);
    response[responseSize / 4] = quint32(th);
    responseSize += Word_size;
}

void Emulator::Read_transaction(TransactionHeader th){
    bool error_on_read = false;
    th.InfoCode = 0;
    for(quint8 i = 0; i < th.Words; ++i){
        if(responseSize / 4 + i + 1 == maxWordsPerPacket){
            th.InfoCode = 4;
            th.Words = i;
            error_on_read = true;
            break;
        }
        response[responseSize / 4 + 1 + i] = adress_space[request[counter +1]+i];
    }
    response[responseSize / 4] = quint32(th);
    log("reading " + Words(th.Words) + " words from " + Hex(request[counter + 1]) + (error_on_read ? ": bus error on read" :" (sequental)"));
    counter = error_on_read ? requestSize / 4 : counter + 2;
    responseSize += Word_size * (th.Words + 1);
}

void Emulator::Non_Incrementing_read_transaction(TransactionHeader th){
    bool error_on_read = false;
    th.InfoCode = 0;
    for(quint8 i = 0; i<th.Words; i++){
        if(responseSize / 4 + i + 1 == maxWordsPerPacket){
                th.InfoCode = 4;
                th.Words = i;
                error_on_read = true;
                break;
            }
            response[responseSize / 4+ 1 +i] = request[counter + 1] == FIFO_adress ? (FIFO_queue.isEmpty() ? 0x0 : FIFO_queue.dequeue()) : adress_space[request[counter +1]];
            if(request[counter + 1] == FIFO_adress)
                adress_space[request[counter + 1]] = FIFO_queue.isEmpty() ? 0x0 : FIFO_queue.head();
        }
    response[responseSize / 4] = quint32(th);
    log("reading " + Words(th.Words) + " words from " + Hex(request[counter + 1])  + (error_on_read ? ": bus error on read" :" (non-incrementing)"));
    counter = error_on_read ? requestSize / 4 : counter + 2;
    responseSize += Word_size * (th.Words + 1);
}

void Emulator::Non_Incrementing_write_transaction(TransactionHeader th){
    //Если число слов указанное в заголовке больше имеющегося в транзакции
    if( th.Words + counter > requestSize / 4){
        th.InfoCode = 0x5;
        response[responseSize / 4] = quint32(th);
    }
    else{
        //  Меняю InfoCode на 0x0 чтобы поставить заголовок транзакции в ответ response
        th.InfoCode = 0;
        //  Если ответ уже формируется, то заголовок транзакции в массиве будет иметь номер, соответствующий размеру массива в байтах деленному на 4
        response[responseSize / 4] = quint32(th);
        // Проверка, если Base Adress соответствует FIFO адрессу
        if(request[counter + 1] == FIFO_adress){
            // Запись в очередь элементов запроса начиная со второго слова транзакции (первое заголовок, второе адрес)
            for(quint32 i = 0; i < th.Words; i++){
                FIFO_queue.enqueue(request[counter + 2 + i]);
                log("writing  " + Hex(request[counter + 2 + i]) + " to " + Hex(FIFO_adress) + " (non-incrementing)");
            }
            //Регистр соответствующий адрессу FIFO ссылается на первое слово в очереди
            adress_space[FIFO_adress] = FIFO_queue.head();
        }
        else
            // Если Base Adress не соответствует FIFO адрессу, то в регистр записывается последнее слово в транзакции (рано или поздно)
            for(quint32 i = 0; i<th.Words; i++){
                bd.set_registers(adress_space, request[counter + 1],  request[counter + 2 + i], " (non-incrementing)");
                log(bd.get_message());
                if(bd.read_only(request[counter + 1]))
                    break;
            }
    }
    // Ответная транзакция состоит из одного заголовка, поэтому увеличиваем размер только на одно 32-битное слово
    responseSize += Word_size;
    // Счетчик увеличивается на количество слов плюс заголовок 32 бита плюс адресс
    counter = bd.read_only(request[counter + 1]) ? requestSize / 4 : counter + 2 + th.Words;
}

void Emulator::RMWbits_transaction(TransactionHeader th){
    if(request[counter + 1 ]!= FIFO_adress){
        // В заголовке такой транзакции должно быть одно слово. В случае если это не так, то транзакция выполняется. но ответный заголовок содержит сообщение об ошибке
        th.InfoCode = (th.Words == 1 ? 0 : 5);
        // Ответный заголовк полностью соответствует заголовку запроса, за исключением InfoCode
        response[responseSize/4] = quint32(th);
        // В ответной транзакции содержится значение в регистре до изменения
        response[responseSize/4 + 1] = adress_space[request[counter + 1]];
        // Изменение значения в регистре выражением ( X & A) | B
        bd.set_registers(adress_space, request[counter + 1],  (adress_space[request[counter + 1]] & request[counter + 2])|request[counter + 3], "");
        log(bd.get_message());
        // Ответная транзакция содержит 2 32-битных слова: заголовок и содержимое до изменения
        responseSize += 2 * Word_size;
        // Счетчик перeдвигается на три слова + 1 слова полюбому, но если больше слов то сдвигаемя на столько, сколько указано в заголовке (так решили)
        counter = bd.read_only(request[counter + 1]) ? requestSize/4 : counter +  3 + th.Words;
    }
    else{
        th.InfoCode = 5;
        response[responseSize/4] = quint32(th);
        log("writing  " + Hex((adress_space[request[counter + 1]] & request[counter + 2])|request[counter + 3]) + " to read-only " + Hex(request[counter + 1]) + ": forbidden");
        responseSize += Word_size;
        counter = requestSize / 4;
    }

}

void Emulator::RMWsum_transaction(TransactionHeader th){
    if(request[counter + 1]  != FIFO_adress){
    // В заголовке такой транзакции должно быть одно слово. В случае если это не так, то транзакция выполняется. но ответный заголовок содержит сообщение об ошибке
    th.InfoCode = (th.Words == 1 ? 0 : 5);
    // Ответный заголовк полностью соответствует заголовку запроса, за исключением InfoCode
    response[responseSize/4] = quint32(th);
    // В ответной транзакции содержится значение в регистре до изменения
    response[responseSize/4 + 1] = adress_space[request[counter + 1]];
    // Изменение значения в регистре выражением ( X + A )
    bd.set_registers(adress_space, request[counter + 1],  adress_space[request[counter + 1]] + request[counter + 2], "");
    log(bd.get_message());
    // Ответная транзакция содержит 2 32-битных слова: заголовок и содержимое до изменения
    responseSize += 2 * Word_size;
    }
    else{
        th.InfoCode = 5;
        response[responseSize / 4] = quint32(th);
        responseSize += Word_size;
        log("writing  " + Hex(adress_space[request[counter + 1]] + request[counter + 2]) + " to read-only " + Hex(request[counter + 1]) + ": forbidden");
    }
    // Счетчик пердвигается на два слова + 1 слова полюбому, но если больше слов то сдвигаемя на столько, сколько указано в заголовке (так решили)
    counter =bd.read_only(adress_space[request[counter + 1]]) ? requestSize/4 : counter + 2 + th.Words;
}

void Emulator::response_to_buffer()
{
    // Если счетчик равен 16, то есть следующий пакет будет 17, то обнуляем счетчик и записываем пакет вместо нулевого
    if(Hash_counter == 16)
        Hash_counter = 0;
    // Запись ответного пакета в буффер
    for(int i = 0; i < Response_Size(); i++)
        // буфферное пространство представляет собой массив из 16 * max_Words_per_Packet элементов, поэтому запись будет производиться в ту область массива, где сейчас стоит Hash_counter
        *(buffer_responses + Hash_counter * maxWordsPerPacket + i) = response[i];
    // Запись в Хэш-таблицу адресса ячейки первого элемента пакета, где ключом к Хэшу является идентификатор пакета
    Old_response[PacketHeader(response[0]).PacketID] = (buffer_responses + Hash_counter * maxWordsPerPacket);
    // Длины тоже записываются в свой Хэш, чтобы знать сколько элементов считывать, для формирования re-send пакета
    Lenghts[PacketHeader(response[0]).PacketID] = Response_Size();
    // После записи, счетчик Хэша должен обязательно увеличиться на один
    Hash_counter++;
}

void Emulator::buffer_to_response()
{
    // Если у данного запроса ID содержится в буффере, то составляем ответный пакет
    // Если нет, то ничего не делаем
    ready_to_send_response = (Lenghts.contains(PacketHeader(request[0]).PacketID)? true : false);
    if(ready_to_send_response)
    {
        // Считываем из Хэша только то количество слов, которое соответствует запрашиваемому пакету
        for(int i = 0; i < Lenghts[PacketHeader(request[0]).PacketID]; i++)
            response[i] = *(Old_response[PacketHeader(request[0]).PacketID] + i);
        //Размер ответного пакета равен длине хранимого пакета
        responseSize = Lenghts[PacketHeader(request[0]).PacketID];
    }
    else
        qDebug()<<"No such Packet in buffer";
}


