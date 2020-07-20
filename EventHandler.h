#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <board.h>
#include <FEE.h>
#include <cmath>
#include <random>

class Laser : public QObject{
    Q_OBJECT

public:
    Laser(){connect(&LaserMeandr, &QTimer::timeout, this, [=](){
            Npulses_to_send += Npulses_float;
//            qDebug() << Npulses_to_send << lround(Npulses_to_send);
            emit LaserFlash(quint32(lround(Npulses_to_send)));});}
    void LaserControl(bool LASER_ON, quint32 LASER_DIV){
        SetFrequency(LASER_DIV);
        LASER_ON ? LaserMeandr.start(50) : LaserMeandr.stop();
    }
signals:
    void LaserFlash(quint32);

private:
    double Npulses_float, Npulses_to_send = 0;
    QTimer LaserMeandr;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> nd;
    const quint32 Clock = 40000000;
//    const float stddev = 0.1;

    void SetFrequency(quint32 LASER_DIV){
//        std::normal_distribution<> nd{ (50 / 1000) * (LASER_DIV != 0 ?  (Clock / LASER_DIV) : 2.384), stddev};
//        qDebug()<< LASER_DIV << Clock/LASER_DIV;
        Npulses_float = double(50. / 1000.) * (LASER_DIV != 0 ? (Clock / LASER_DIV) : 2.384);
        qDebug() << Npulses_float;
    }

};

class EventHandler : public QObject{
    Q_OBJECT

public:
    EventHandler(TypeFEE* FEE, Board* testboard){
        bd = testboard; AddressSpace = FEE;
        connect(&laser, SIGNAL(LaserFlash(quint32)), this, SLOT(LaserHandler(quint32)));}

public slots:
        void handle(quint16 address){
            quint8 PMn0 = address / 0x200;
            if(bd->contains_register(address))
                PMn0 ? PMregHandler(PMn0, address) : TCMregHandler(address);
        }

private:
    TypeFEE* AddressSpace;
    Board* bd;
    Laser laser;

    void PMregHandler(quint8 PM, quint16 address){
        qDebug()<<"Регистр "<< address <<" PM #" << PM;
    }

    void TCMregHandler(quint16 address){
        switch(address){
        case 0x1B: laser.LaserControl(AddressSpace->LASER_ON, AddressSpace->LASER_DIV); /*qDebug() << AddressSpace->LASER_ON
                                                                                                 << AddressSpace->LASER_DIV;*/ break;
        default:                                                                        break;
        }
    }

private slots:

    void LaserHandler(quint32 Npulses){
//        qDebug()<<Npulses;
        for(quint8 pmN = 0; pmN < 20; ++pmN){
//            qDebug() << bd->contains_register((pmN + 1) * 0x200 + 0xC0);
            if(bd->contains_register((pmN + 1) * 0x200 + 0xC0))
                for(quint8 i = 0; i < 12; ++ i)
                    AddressSpace->PM[pmN].Counters[i].CNT_CFD = Npulses;
        }
    }

};



#endif // EVENTHANDLER_H
