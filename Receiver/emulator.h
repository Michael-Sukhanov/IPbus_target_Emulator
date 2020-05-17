#ifndef EMULATOR_H
#define EMULATOR_H

#include <QtNetwork/QUdpSocket>
#include <IPbusHeaders.h>
#include <board.h>
#include <QDebug>
#include <QQueue>
#include <QTime>
#include <QDate>
#include <qmath.h>

const quint16 maxWordsPerPacket = 368; // Максимальное число 32 битных слов в пакете ограничено MTU
const quint32 FIFO_adress = 0x00001014;// Адресс FIFO регистра - один такой
const quint8 Word_size = sizeof(quint32);// Все слова 32-битные, значит и размер слова 32 бита

class Emulator{   
public:
    Emulator();
    ~Emulator();
    bool ready(){return ready_to_send_response;}//Если пакет, пришедший с сокета соответствует протоколу, то можно отправлять ответ
    char* get_response(){return Presponse;}
    quint32 response[maxWordsPerPacket];//Ответный пакет
    quint16 Response_Size(){return responseSize;} //Получение размера ответного пакета. Не нужна???
    quint32* Get_info(){return adress_space;}// Получение данных из регистров
    void Prepare_response(); // Формирование ответа на основе запроса
    void Clear_response(){responseSize = 0;} // Функция, подготовки response к следующей отправке
    quint32 request[maxWordsPerPacket]; // Пакет запроса
    char* Prequest = reinterpret_cast<char*>(request);//Для работы с массивом quint32 в дальнейшем
    quint16 requestSize = 0; // Размер запрашивающего пакета, который задается количеством байт, пришедшим с сокета
    char* Presponse = reinterpret_cast<char*>(response); //Указатель на первый элеммент массива ответного пакета переводится в char чтобы отправить по UDP
    quint16 responseSize = 0; // Размер ответного пакета, который задается при его формировании для корректной отправки по UDP протоколу с помощью функции writeDatagramm

    //  Форматирует вывод числа в 16-ричной системе в "человекочитаемый" вид
    QString Hex(quint32 val){
        if(val != 0)
            return "0x" + QString(7 - qFloor(qLn(val)/qLn(16)), '0') + QString::number(val, 16).toUpper();
        else
            return "0x00000000";
    }

    QString Words(int w){
        if(w < 9)
            return "  " + QString::number(w);
        else{
            if(w > 99)
                return QString::number(w);
            else
                return " " + QString::number(w);
        }
    }

    QString GetLogMessage(bool write_only){
        int read_ind;
        if(write_only && ((Packet_log_message.contains("Control") && Packet_log_message.count(':') < 3) || Packet_log_message.contains("Status")))
           Packet_log_message = Packet_log_message.contains("writing") ? Packet_log_message : "";
        if(write_only && Packet_log_message != ""){
            int strings_to_remove = Packet_log_message.count("reading");
            for(quint8 counter = 0; counter < strings_to_remove; ++counter){
                read_ind = Packet_log_message.indexOf("reading");
                if(!Packet_log_message.mid(read_ind, Packet_log_message.indexOf('\n', read_ind) + 1 - read_ind).contains(':'))
                    Packet_log_message.remove(read_ind, Packet_log_message.indexOf('\n', read_ind) + 1 - read_ind);
            }
        }

        return Packet_log_message;}

    bool set_board(QString filename, QString Name = "New Board"){
        bd.set_board_name(Name);
        return bd.set_regulations(filename);
    }

    QString board_info(quint16 address){
       QString readonly = (bd.read_only(address) ? " is read only," : " is") ;
       if(bd.contains_register(address)){
           return  readonly +
                   (bd.is_signed(address) ? " signed," : " unsigned,") +
                   (bd.range_correction(address) ? " with auto-correction in range from" : " with value in range from") +
                   " " + Hex(bd.get_Lower_mask(address)) + " to " + Hex(bd.get_upper_mask(address));
       }
       else
           return " without restrictions";
    }


private:
    quint32 adress_space[65536]; // адрессное пространство, где хранятся все записанные по умолчанию регистры
    bool ready_to_send_response = true; //false если пакет из сокета не соответствует протоколу IPbus
    quint16 NextPacketID = 1;
    QQueue<quint32> FIFO_queue; // FIFO ячейка, находящаяся по адресу 0x00001014
    quint16 counter = 0; //Пересчет обработанных слов в request
    quint32* buffer_responses = new quint32[maxWordsPerPacket * 16]; // FTM может хранить 16 пакетов одновременно, поэтому буфер будет таким
    //Для обращеня к буферу использую Хеш-таблицы
    QHash<quint16, quint32*> Old_response; //Хранение адресов заголовков response где ключ - ID заголовка
    QHash<quint16, quint16> Lenghts; //Хранение длин ответных пакетов (для их считывания)
    quint8 Hash_counter = 0;//Счетчик, позволяющий регулировать замену самых старых данных на самые новые

    QFile logFile;
    QTextStream logStream;
    QString Packet_log_message;
    bool littleEndian = true;

    Board bd;


    //Загрузка пакета из буфера в ответный пакет
    void buffer_to_response();
    //Ответный пакет, который сразу после формирования переходит в буфер
    void response_to_buffer();
    //Функция, составляющая заголовок ответного пакета
    void Prepare_Header();
    //Функция, составляющая ответ на транзакции
    void Prepare_Transaction_response();

    //Функции, меняющие порядок байт в каждом 32-битном слове массива ответа или запроса
    void Request_to_little_endian(){
        for(quint16 i = 0; i < requestSize; i++)
        request[i] = qFromBigEndian(request[i]);
    }
    void Response_to_Big_endian(){
        for(quint16 i = 0; i < responseSize; i++)
        response[i] = qToBigEndian(response[i]);
    }

//  Функции обработчики транзакций
    void Write_transaction(TransactionHeader);
    void Read_transaction(TransactionHeader);
    void Non_Incrementing_read_transaction(TransactionHeader);
    void Non_Incrementing_write_transaction(TransactionHeader);
    void RMWbits_transaction(TransactionHeader);
    void RMWsum_transaction(TransactionHeader);

    void log(QString msg){
        logStream << msg << endl;
        Packet_log_message.append(msg + '\n');
    }

    void clear_log(){
        littleEndian = true;
        Packet_log_message = "";
    }


};

#endif // EMULATOR_H
