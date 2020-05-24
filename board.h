#ifndef BOARD_H
#define BOARD_H

#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QDebug>
#include <qmath.h>

class Board
{
public:
    Board();
    ~Board();

    bool contains_register(quint16 address){return readonly.count(address);}

    bool is_signed(quint16 address)        {return sign.contains(address) ? sign[address] : false;}
    bool read_only(quint16 address)        {return readonly.contains(address) ? readonly[address] : false;}
    bool range_correction(quint16 address) {return rangecorr.contains(address) ? rangecorr[address] : false;}

    void set_board_name(QString nm)        {Board_name = nm;}

    bool set_regulations(QString filename);

    void set_registers(quint32* reg, quint16 address, quint32 value, QString type =" (sequental)");

    quint32 get_upper_mask(quint16 address){return UpperMask.contains(address) ? UpperMask[address] : 0xFFFFFFFF;}
    quint32 get_Lower_mask(quint16 address){return LowerMask.contains(address) ? LowerMask[address] : 0x00000000;}
    QString get_board_name()               {return Board_name;}
    QString get_message()                  {return message;}

    QString Hex(quint32 val){
        if(val != 0)
            return "0x" + QString(7 - qFloor(qLn(val)/qLn(16)), '0') + QString::number(val, 16).toUpper();
        else
            return "0x00000000";
    }


private:
    QString Board_name, message;
    QHash<quint16, bool> readonly;
    QHash<quint16, bool> sign;
    QHash<quint16, bool> rangecorr;
    QHash<quint16, quint32> UpperMask;
    QHash<quint16, quint32> LowerMask;



};

#endif // BOARD_H
