#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <board.h>
#include <FEE.h>
#include <cmath>
#include <random>
#include <QRandomGenerator>

const quint32 Clock = 40000000; //40 MHz

class CRU : public QObject{
    Q_OBJECT
public:
    CRU(){connect(&Send_ID, &QTimer::timeout, this, &CRU::BC_ORBIT_checking);}
    void start(quint32 per= 100){
        period = per;
        Send_ID.start(int(period));
    }

signals:
    void Send_data(quint32, quint16);

private:
    quint32 CRU_ORBIT = QRandomGenerator::global()->generate() % 0xffffffff, period;
    quint16 BC_limit = 0xDEB, CRU_BC = QRandomGenerator :: global()->generate() % BC_limit;
    QTimer Send_ID;

private slots:
    void BC_ORBIT_checking(){
        CRU_BC += Clock * period / 1000;
        if(CRU_BC > BC_limit){
            ++CRU_ORBIT;
            CRU_BC = CRU_BC % BC_limit;
        }
        emit Send_data(CRU_ORBIT, CRU_BC);
    }
};

class Laser : public QObject{
    Q_OBJECT

public:
    Laser(){connect(&LaserMeandr, &QTimer::timeout, this, [=](){
            N_CFD_to_send += N_CFD;
            N_TRG_to_send += Phase ? N_CFD : 0;
            emit LaserFlash(quint32(lround(N_CFD_to_send)), quint32(lround(N_TRG_to_send)));});
            Start = randomize ? (-1024 + (QRandomGenerator::global()->generate()) % 2049) : -205;
            PhaseControl(0x0);
//            qDebug() << Start;
           }
    void LaserControl(bool LASER_ON, quint32 LASER_DIV){
        SetFrequency(LASER_DIV);
        LASER_ON ? LaserMeandr.start(5) : LaserMeandr.stop();
    }

    void PhaseControl(qint16 LASER_DELAY){
        Phase = LASER_DELAY > Start && LASER_DELAY < Start + Gate;
//        qDebug() << (Phase ? "Фаза найдена" : "Фаза не найдена");
    }

signals:
    void LaserFlash(quint32, quint32);

private:
    double N_CFD, N_TRG, N_CFD_to_send = 0, N_TRG_to_send = 0;
    QTimer LaserMeandr;
    std::random_device rd{};
//    std::mt19937 gen{rd()};
//    std::normal_distribution<float> nd;
    const quint16 Gate = 410; // 5 ns.
    qint16 Start;
    bool Phase = true, randomize = true;
//    const float stddev = 0.1;

    void SetFrequency(quint32 LASER_DIV){
//        std::normal_distribution<> nd{ (50 / 1000) * (LASER_DIV != 0 ?  (Clock / LASER_DIV) : 2.384), stddev};
        N_CFD = double(5. / 1000.) * (LASER_DIV != 0 ? (Clock / LASER_DIV) : 2.384);
    }

};


class EventHandler : public QObject{
    Q_OBJECT

public:
    EventHandler(TypeFEE* FEE, Board* testboard){
        bd = testboard; AddressSpace = FEE;
        connect(&laser, SIGNAL(LaserFlash(quint32, quint32)), this, SLOT(LaserHandler(quint32, quint32)));
        connect(&Update_counters, &QTimer::timeout, this, &EventHandler::FIFOCountersFiller);
        connect(&CR_unit, SIGNAL(Send_data(quint32, quint16)), this,SLOT(BC_ORBIT_MONITOR(quint32,quint16)));}

public slots:
        void handle(quint16 address){
            quint8 PMn0 = address / 0x200;
            if(bd->contains_register(address))
                PMn0 ? PMregHandler(PMn0, address) : TCMregHandler(address);
        }

        void handle_FIFO(quint16 address){
            --AddressSpace->PM[(address - 0x100) / 0x200 - 1].COUNTERS_FIFO_LOAD;
        }

private:
    TypeFEE* AddressSpace;
    Board* bd;
    Laser laser;
    CRU CR_unit;
    QTimer Update_counters;
    float update_rate[8] = {0, .1, .2, 0.5, 1.0, 2.0, 5.0, 10.0};

    void PMregHandler(quint8 PM, quint16 address){
        switch(address - (PM * 0x200)){
//        case 0x100: --AddressSpace->PM[PM - 1].COUNTERS_FIFO_LOAD;                      break;
        case 0xD8: {AddressSpace->PM[PM - 1].GBT.Status.BCID_SYNC_MODE = AddressSpace->PM[PM - 1].GBT.Control.RESET;
                    (*AddressSpace)[PM * 0x200 + 0xD8] = 0x0;  CR_unit.start();       break;}
        }
    }

    void TCMregHandler(quint16 address){
        switch(address){
        case 0x2 : laser.PhaseControl(AddressSpace->LASER_DELAY);                       break;
        case 0x1B: laser.LaserControl(AddressSpace->LASER_ON, AddressSpace->LASER_DIV); break;
        case 0x50: {quint8 upd_mode = quint8(AddressSpace->COUNTERS_UPD_RATE) & 7;
//                    qDebug() << upd_mode << int(update_rate[upd_mode] * 1000);
                    if(upd_mode) Update_counters.start(int(update_rate[upd_mode] * 1000));
                    else Update_counters.stop();                                        break;}
        case 0xD8: {AddressSpace->GBT.Status.BCID_SYNC_MODE = AddressSpace->GBT.Control.RESET;
                   (*AddressSpace)[0xD8]= 0x0;          CR_unit.start();      break;}
        default:                                                                        break;
        }
    }

private slots:

    void LaserHandler(quint32 N_CFD, quint32 N_TRG){
        for(quint8 pmN = 0; pmN < 20; ++pmN)
            if(bd->contains_register((pmN + 1) * 0x200 + 0xC0))
                for(quint8 i = 0; i < 12; ++ i){
                    AddressSpace->PM[pmN].Counters[i].CNT_CFD = N_CFD;
                    AddressSpace->PM[pmN].Counters[i].CNT_TRG = N_TRG;
                }
    }

    void FIFOCountersFiller(){
        for(quint8 pmN = 0; pmN < 20; ++pmN)
            if(bd->contains_register((pmN + 1) * 0x200 + 0x100) && AddressSpace->PM[pmN].COUNTERS_FIFO_LOAD < 481){
                for(quint8 i = 0; i < 12; ++i){
                    (bd->get_FIFO_pointer((pmN + 1) * 0x200 + 0x100))->enqueue(AddressSpace->PM[pmN].Counters[i].CNT_CFD);
                    (bd->get_FIFO_pointer((pmN + 1) * 0x200 + 0x100))->enqueue(AddressSpace->PM[pmN].Counters[i].CNT_TRG);
                    AddressSpace->PM[pmN].COUNTERS_FIFO_LOAD += 2;
                }
            }
    }

    void BC_ORBIT_MONITOR(quint32 CRU_ORBIT, quint16 CRU_BC){
        for(quint8 counter = 0; counter < 21; ++counter)
            if(bd->contains_register(0 + 0x200 * counter) &&
              (counter ? AddressSpace->PM[counter - 1].GBT.Status.BCID_SYNC_MODE :
                         AddressSpace->GBT.Status.BCID_SYNC_MODE)){
                    (*AddressSpace)[0x200 * counter + 0xE9] = CRU_ORBIT;
                    (*AddressSpace)[0x200 * counter + 0xEA] = CRU_BC;
            }
    }
};



#endif // EVENTHANDLER_H
