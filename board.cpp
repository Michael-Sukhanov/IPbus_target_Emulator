#include "board.h"

Board::Board(){

}

Board::~Board(){

}

bool Board::set_regulations(QString filename){
    FIFOs.clear();
    FIFOs[0x100] = QQueue<quint32>();
    bool ok;
    QString tmp;
    QFile file(filename);
    QRegExp hex_mask("0x[0-9a-fA-F]{1,5}"), bi_mask("[01]{1,20}"), mask_pt("[Mm][Aa]?[Ss][Kk]"), delimeter("[=:-]");
    if (!file.open(QFile::ReadOnly)){
        return false;
    }
    QTextStream in(&file);
    in >> tmp;
    if(mask_pt.exactMatch(tmp)){
        in >> tmp;
        if(delimeter.exactMatch(tmp))
            in>> tmp;
    }
    if(hex_mask.exactMatch(tmp.toUtf8()) || bi_mask.exactMatch(tmp.toUtf8())){
        quint32 mask = tmp.toUtf8().toUInt(&ok, hex_mask.exactMatch(tmp.toUtf8()) ? 16 : 2);
        for(quint8 counter = 0; counter < 20; ++counter){
            QFile pm("Configs/PM.txt");
            QTextStream pm_in(&pm);
            if(!pm.open(QFile::ReadOnly))
                break;
            if((mask & quint32(pow(2, counter))) == quint32(pow(2, counter))){
                config_read(&pm_in, (counter + 1) * 0x200);
                  FIFOs[0x200 * (counter + 1) + 0x100] = QQueue<quint32>();
            }
            pm.close();
        }
    }
    config_read(&in);
    file.close();
    emit config_changed();
    return true;
}
//create_mode установка значений регистров в диапазоне при загрузке файла кнфигурации
void Board::set_registers(quint32 *reg, quint16 address, quint32 value, QString type, bool create_mode){
//    qDebug()<<"Setting registers";
    bool corrected = false;
    if(readonly.contains(address) && (!read_only(address) || create_mode) && !is_FIFO(address)){
        if(is_signed(address)){
            if(range_correction(address) || create_mode){
                if(qint32(value) < qint32(get_Lower_mask(address))){
                    reg[address] = get_Lower_mask(address);
                    corrected = true;
                }
                else if(qint32(value) > qint32(get_upper_mask(address))){
                    reg[address] = get_upper_mask(address);
                    corrected = true;
                }
                else{
                    reg[address] = value;
                    corrected = false;
                }
                message = "writing " + Hex(value) + " to " + Hex(address) + (corrected ? ": out of range, result:" + Hex(reg[address]) : "") +  type;
            }else{
                reg[address] = qint32(value) < 0 ? value | get_Lower_mask(address) : value & get_upper_mask(address);
                message = "writing " + Hex(value) + " to " + Hex(address)  + type;
            }
        }else{

            if(range_correction(address) || create_mode){
                reg[address] = get_Lower_mask(address) <= value && value <= get_upper_mask(address) ? value :
                               (value < get_Lower_mask(address) ? get_Lower_mask(address) : get_upper_mask(address));
            }else{
                reg[address] = value & get_upper_mask(address);
                message = "writing " + Hex(value) + " to " + Hex(address) + type;
             }
         }
    }
    else if(readonly.contains(address)){
        message = "writing " + Hex(value) + " to " + (is_FIFO(address) ? "FIFO" : "read only ") + Hex(address) + ": forbidden";
    }
    else{
        reg[address] = value;
        message = "writing " + Hex(value) + " to " + Hex(address) + type;
    }


}

void Board::config_read(QTextStream * in, quint16 start){
    bool ok;
    QString tmp;
    quint16 registers_address;
    while(!in->atEnd()){
        *in >> tmp;
        if(QString(tmp.toUtf8()) == "*/")
            break;
    }
    while(!in->atEnd()){
        *in >> tmp;
        registers_address = quint16(tmp.toUtf8().toUInt(&ok, 16) + start);
        if(!readonly.contains(registers_address)){
            *in >> tmp;
            readonly[registers_address ] = bool(tmp.toUtf8().toInt());
            *in >> tmp;
            rangecorr[registers_address] =bool(tmp.toUtf8().toInt());
            *in >> tmp;
            sign[registers_address] = bool(tmp.toUtf8().toInt());
            *in >> tmp;
            LowerMask[registers_address] = tmp.toUtf8().toUInt(&ok, 16);
            *in >> tmp;
            UpperMask[registers_address] = tmp.toUtf8().toUInt(&ok, 16);
        }
    }

}
