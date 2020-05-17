#include "board.h"

Board::Board()
{

}

Board::~Board(){

}

bool Board::set_regulations(QString filename){
    bool ok;
    quint16 registers_address;
    QString tmp;
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }
    QTextStream in(&file);
    while(!in.atEnd()){
        in >> tmp;
        registers_address = tmp.toUtf8().toUInt(&ok, 16);
        if(!readonly.contains(registers_address)){
            in >> tmp;
            readonly[registers_address] = bool(tmp.toUtf8().toInt());
            in >> tmp;
            rangecorr[registers_address] =bool(tmp.toUtf8().toInt());
            in >> tmp;
            sign[registers_address] = bool(tmp.toUtf8().toInt());
            in >> tmp;
            LowerMask[registers_address] = tmp.toUtf8().toUInt(&ok, 16);
            in >> tmp;
            UpperMask[registers_address] = tmp.toUtf8().toUInt(&ok, 16);
        }
    }
    file.close();
    return true;
}

void Board::set_registers(quint32 *reg, quint16 address, quint32 value){
    bool corrected = false;
    if(readonly.contains(address) && !read_only(address)){
        if(is_signed(address)){
            if(range_correction(address)){
                if(qint32(value) < qint32(get_Lower_mask(address))){
                    reg[address] = get_Lower_mask(address);
                    corrected = true;
                }
                else if(value > get_upper_mask(address)){
                    reg[address] = get_upper_mask(address);
                    corrected = true;
                }
                else{
                    reg[address] = value;
                    corrected = false;
                }
                message = "writing " + Hex(value) + " to " + Hex(address) + (corrected ? ": out of range, result:" + Hex(reg[address]) : "") ;
            }else{
                reg[address] = qint32(value) < 0 ? value | get_Lower_mask(address) : value & get_upper_mask(address);
                message = "writing " + Hex(value) + " to " + Hex(address);
            }
        }else{

            if(range_correction(address)){
                reg[address] = get_Lower_mask(address) <= value && value <= get_upper_mask(address) ? value :
                               (value < get_Lower_mask(address) ? get_Lower_mask(address) : get_upper_mask(address));
            }else{
                reg[address] = value & get_upper_mask(address);
                message = "writing " + Hex(value) + " to " + Hex(address);
             }
         }
    }
    else if(readonly.contains(address)){
        message = "writing " + Hex(value) + " to read only " + Hex(address) + ": forbidden";
    }
    else{
        reg[address] = value;
        message = "writing " + Hex(value) + " to " + Hex(address);
    }

}
