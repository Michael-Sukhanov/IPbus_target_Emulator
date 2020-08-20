#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <board.h>
#include <FEE.h>
#include <cmath>
#include <random>
#include <Temperature.h>

const quint32 Clock = 40000000; //40 MHz
enum Side{ A, C};

//____________________________________________COMMON_READOUT_UNIT__IMPLEMENTATION_______
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


//____________________________________LASER_IMITATION__________________________________________________________
class Laser : public QObject{
    Q_OBJECT

public:
    Laser(){connect(&LaserMeandr, &QTimer::timeout, this, [=](){
            for(quint8 i = 0; i < 20; ++i){
            N_CFD_to_send[i] += N_CFD;
            N_TRG_to_send[i] += Phase ? N_CFD : 0;
            }
            emit LaserFlash();});
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

    void reset_counters(quint8 PM){N_CFD_to_send[PM] = 0; N_TRG_to_send[PM] = 0;}
    double N_CFD_to_send[20] = {0}, N_TRG_to_send[20] = {0};

signals:
    void LaserFlash();

private:
    double N_CFD;
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

//__________________________________________CLASS___FOR__EVENT'S_HANDLING_______________________________________
class EventHandler : public QObject{
    Q_OBJECT

public:
    EventHandler(TypeFEE* FEE, Board* testboard){
        bd = testboard; AddressSpace = FEE;
        connect(&internalTimer, &QTimer::timeout, this, [=](){
            AddressSpace->TEMPERATURE = quint32(*TCM_BOARD_temp);
            AddressSpace->FPGA_TEMP = quint32(*TCM_FPGA_temp);
            write_to_avail_PMs(quint32(*PM_BOARD_temp), 0xBC);
            write_to_avail_PMs(quint32(*PM_FPGA_temp), 0xFC);
            });
        connect(&laser, SIGNAL(LaserFlash()), this, SLOT(LaserHandler()));
        connect(&Update_counters, &QTimer::timeout, this, &EventHandler::FIFOCountersFiller);
        connect(&CR_unit, SIGNAL(Send_data(quint32, quint16)), this,SLOT(BC_ORBIT_MONITOR(quint32,quint16)));
        connect(bd, SIGNAL(config_changed()), this, SLOT(init_values()));}


public slots:
        void handle(quint16 address){
            quint8 PMn0 = address / 0x200;
            if(bd->contains_register(address))
                PMn0 ? PMregHandler(PMn0, address) : TCMregHandler(address);
        }

        void handle_FIFO(quint16 address){
            --AddressSpace->PM[(address - 0x100) / 0x200 - 1].COUNTERS_FIFO_LOAD;
        }

        void ReadHandler(quint16 address){
            switch(address){
            case 0x1A:  AddressSpace->readinessChangeA = 0;                                     break;
            case 0x3A:  AddressSpace->readinessChangeC = 0;                                     break;
            }
        }

private:
    TypeFEE* AddressSpace;
    Board* bd;
    Laser laser;
    CRU CR_unit;
    QTimer Update_counters, internalTimer;
    temperature* TCM_BOARD_temp = new temperature(0xEB, 0.35);
    temperature* TCM_FPGA_temp = new temperature(40186, 35.45);
    temperature* PM_BOARD_temp = new temperature(397, 0.72);
    temperature* PM_FPGA_temp = new temperature(40382, 36.23);

    float update_rate[8] = {0, .1, .2, 0.5, 1.0, 2.0, 5.0, 10.0};

//  обработка измений значений регистров PM
    void PMregHandler(quint8 PM, quint16 address){
        switch(address - (PM * 0x200)){
//        case 0x100: --AddressSpace->PM[PM - 1].COUNTERS_FIFO_LOAD;                      break;
        case 0x7F: {if(AddressSpace->PM[PM-1].RESET_COUNTERS)
                        laser.reset_counters(PM - 1);
                        AddressSpace->PM[PM-1].RESET_COUNTERS = 0;                      break;}
        case 0xD8: {if(AddressSpace->PM[PM - 1].GBT.Control.RESET){
                        AddressSpace->PM[PM - 1].GBT.Status.BCID_SYNC_MODE = 1;
                        AddressSpace->PM[PM - 1].GBT.Control.RESET = 0;
                        CR_unit.start();                                               }break;}
        }
    }
//  обработка измений значений регистров TCM
    void TCMregHandler(quint16 address){
        switch(address){
        case 0x2 : laser.PhaseControl(AddressSpace->LASER_DELAY);                       break;
        case 0x1B: laser.LaserControl(AddressSpace->LASER_ON, AddressSpace->LASER_DIV); break;
        case 0x1A: mask_changed(A);                                                     break;
        case 0x3A: mask_changed(C);                                                     break;
        case 0x50: {quint8 upd_mode = quint8(AddressSpace->COUNTERS_UPD_RATE) & 7;
//                    qDebug() << upd_mode << int(update_rate[upd_mode] * 1000);
                    if(upd_mode) Update_counters.start(int(update_rate[upd_mode] * 1000));
                    else Update_counters.stop();                                        break;}
        case 0xD8: {if(AddressSpace->GBT.Control.RESET){
                AddressSpace->GBT.Status.BCID_SYNC_MODE = 1;
                AddressSpace->GBT.Control.RESET = 0;
                CR_unit.start();                                                       }
                    if(AddressSpace->GBT.Control.SEND_READOUT_COMMAND){
                        AddressSpace->GBT.Status.READOUT_MODE =
                                AddressSpace->GBT.Control.SEND_READOUT_COMMAND;
                    AddressSpace->GBT.Control.SEND_READOUT_COMMAND = 0;                }
                                                                                        break;}
        default:                                                                        break;
        }
    }



private slots:

    void LaserHandler(){
        for(quint8 pmN = 0; pmN < 20; ++pmN)
            if(bd->contains_register((pmN + 1) * 0x200 + 0xC0))
                for(quint8 i = 0; i < 12; ++ i){
                    AddressSpace->PM[pmN].Counters[i].CNT_CFD = quint32(lround(laser.N_CFD_to_send[pmN]));
                    AddressSpace->PM[pmN].Counters[i].CNT_TRG = quint32(lround(laser.N_TRG_to_send[pmN]));
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

    void init_values(){
        internalTimer.start(1000);
        init_TCM_channel_mask();
    }

    void init_TCM_channel_mask(){
        quint16 chmsk = 1;
        for(quint8 i = 0; i <10 ; ++i){
            if(bd->contains_register(0x200 * (i + 1))){
                AddressSpace->CH_MASK_A |= (chmsk<<i);
                AddressSpace->PM_LINK_A[i] = Ok_link;

            }else
                AddressSpace->PM_LINK_A[i] = 0x0;
            if(bd->contains_register(0x200 * (i + 11))){
                AddressSpace->CH_MASK_C |= (chmsk<<i);
                AddressSpace->PM_LINK_C[i] = Ok_link;
            }else
                AddressSpace->PM_LINK_C[i] = 0x0;
        }
        if(AddressSpace->CH_MASK_A){
            AddressSpace->sideAenabled = 1;
            AddressSpace->sideAready = 1;
        }if(AddressSpace->CH_MASK_C){
            AddressSpace->sideCenabled = 1;
            AddressSpace->sideCready = 1;
        }
    }

    void mask_changed(Side sd){
        bool links_Ok, previous_state;
        for(quint8 i = 0; i < 10; ++i){
            if(bd->contains_register(0x200 * (i + 1 + sd * 10))
            && contains_bit( !sd ? AddressSpace->CH_MASK_A : AddressSpace->CH_MASK_C, i))
                (!sd ? AddressSpace->PM_LINK_A[i] : AddressSpace->PM_LINK_C[i]) = Ok_link;
            else if(bd->contains_register(0x200 * (i + 1 + sd * 10))
            || contains_bit(!sd ? AddressSpace->CH_MASK_A : AddressSpace->CH_MASK_C, i))
                (!sd ? AddressSpace->PM_LINK_A[i] : AddressSpace->PM_LINK_C[i]) = Bad_link;
            else
                (!sd ? AddressSpace->PM_LINK_A[i] : AddressSpace->PM_LINK_C[i]) = 0x0;
        }
        if(!sd)
            AddressSpace->sideAenabled = (AddressSpace->CH_MASK_A != 0) ;
        else
            AddressSpace->sideCenabled = (AddressSpace->CH_MASK_C != 0) ;

        links_Ok = HDMIlinks_ok(sd);
        previous_state = !sd ? AddressSpace->sideAready : AddressSpace->sideCready;
        if(!sd){
            AddressSpace->readinessChangeA = !(previous_state == links_Ok);
            AddressSpace->sideAready = links_Ok ? 1 : 0;
        }
        else{
            AddressSpace->readinessChangeC = !(previous_state == links_Ok);
            AddressSpace->sideCready = links_Ok ? 1 : 0;
        }
    }

    bool contains_bit(quint32 mask, quint8 pos){
       return mask & (1<<pos);
    }

    bool HDMIlinks_ok(Side sd){
        for(quint8 i = 0; i < 10; ++i)
            if(((sd == A) ? AddressSpace->PM_LINK_A[i] : AddressSpace->PM_LINK_C[i]) == Bad_link)
                return false;
        return true;
    }

    void write_to_avail_PMs(quint32 data, quint16 address){
        for(quint8 i=0; i <= 20; ++i){
            if(bd->contains_register(0x200*(i + 1))){
                (*AddressSpace)[address + 0x200 * (i + 1)] = data;
            }
        }
    }
};



#endif // EVENTHANDLER_H
