#ifndef FEE_H
#define FEE_H

#include "gbt.h"

struct HDMIlinkStatus {
    quint32
        line0delay          : 5,
        line0signalLost     : 1,
        line0signalStable   : 1,
                            : 1,
        line1delay          : 5,
        line1signalLost     : 1,
        line1signalStable	: 1,
                            : 1,
        line2delay          : 5,
        line2signalLost     : 1,
        line2signalStable	: 1,
        bitPositionsOK		: 1,
        line3delay          : 5,
        line3signalLost     : 1,
        line3signalStable	: 1,
        linkOK              : 1;
};

struct TypePM{
    quint32 OR_GATE      : 8,           //┐
                         : 24;          //┘00
    struct{ quint32 val  : 12,          //┐
                         : 20;          //│1-C
          } TIME_ALIGN[12];             //┘
    struct{ quint32 ADC0 : 12, : 20,    //┐
                    ADC1 : 12, : 20;    //│D-24
    } ADC_OFFSET[12];                   //┘
    struct{ quint32 ADC0 : 12, : 20,    //┐
                    ADC1 : 12, : 20;    //│25-3C
    } ADC_RANGE[12];                    //┘
    quint32 CFD_SATR     : 12, : 20,    //]3D
            TDC1_PH_TUNE : 8,           //┐
            TDC2_PH_TUNE : 8,  : 16,    //│3E-3F
            TDC3_PH_TUNE : 8,  : 24;    //┘
    struct{ quint32 val1 : 7,  : 1,     //┐
                    val2 : 6,  : 18;    //│40-4B
    } RAW_TDC_DATA[12];                 //┘
    struct{quint32 ADC0  : 13, : 19,    //┐
                   ADC1  : 13, : 19;    //│4C-63
    } ZEROLVL[12];                      //┘
    struct{quint32 ADC0  : 13, : 19,    //┐
                   ADC1  : 13, : 19;    //│64-7B
    } MEANAMPL[12];                     //┘
    quint32 CH_MASK      : 12, : 18,    //]7C
      reserved[2],
      mainPLLlocked      : 1,           //┐
         TDC1PLLlocked   : 1,           //│
         TDC2PLLlocked   : 1,           //│
         TDC3PLLlocked   : 1,           //│
         GBTlinkPresent  : 1,           //│
         GBTreceiverError: 1,           //│
         TDC1syncError   : 1,           //│
         TDC2syncError   : 1,           //│
         TDC3syncError   : 1,           //│7F
         RESET_COUNTERS  : 1,           //│
         ZERO_LVL_CALIBR : 1,           //│
                         : 1,           //│
         GBTRxPhaseError : 1,           //│
         BCIDsyncLost    : 1,           //│
         droppingHits    : 1,           //│
                         :17;           //┘
    struct ChannelSettings{             //┐
        quint32 CFD_THRESHOLD :16, : 16,//│
                CFD_ZERO      :16, : 16,//│
                ADC_ZERO      :16, : 16,//│80-AF
                ADC_DELAY     :16, : 16;//│
    } Ch[12];                           //┘
    qint32 THRESHOLD_CALIBR[12],        //]B0-BB
    TEMPERATURE;                        //]BC
    quint32 SERIAL_NUM        :16, : 16,//]BD
    reserved1[2];
    struct{ quint32 CNT_CFD,            //┐
                    CNT_TRG;            //│C0-D7
    } Counters[12];                     //┘

    GBTunit GBT;                        //]D8-F7
    quint32 reserved2[4];
    quint32 FPGA_TEMP,                  //]FC
            POWER_1V,                   //]FD
            POWER_1_8V,                 //]FE
            FW_SECOND         : 6,      //┐
            FW_MINUTE         : 6,      //│
            FW_HOUR           : 5,      //│FF
            FW_YEAR           : 6,      //│
            FW_MONTH          : 4,      //│
            FW_DAY            : 5;      //┘
    quint32 COUNTER_VALUES_READOUT,
            COUNTERS_FIFO_LOAD: 9, : 23,
            reserved3[254];

};

struct TypeFEE{
        qint32  DELAY_A         :16, //┐
                                :16, //┘00
                DELAY_C         :16, //┐
                                :16, //┘01
                LASER_DELAY     :16, //┐
                                :16; //┘02
        quint32 ATTEN_VALUE     :13, //┐
                                : 1, //│
                attenBusy       : 1, //│03
                attenNotFound   : 1, //│
                                :16, //┘
                EXT_SW          : 4, //┐
                                :28; //┘04
        qint32  TEMPERATURE     :16, //┐
                                :16, //┘05
                                :32; //]06
        quint32 SERIAL_NUM      :16, //┐
                                :16; //┘07
        qint32  VTIME_LOW       : 9, //┐
                                :23, //┘08
                VTIME_HIGH      : 9, //┐
                                :23; //┘09
        quint32 SC_LEVEL_A      :16, //┐
                                :16, //┘0A
                SC_LEVEL_C      :16, //┐
                                :16, //┘0B
                C_LEVEL_A       :16, //┐
                                :16, //┘0C
                C_LEVEL_C       :16, //┐
                                :16, //┘0D
                ADD_C_DELAY     : 1, //┐
                C_SUM_SIDES     : 1, //│
                SC_SUM_SIDES    : 1, //│0E
                EXTENDED_READOUT: 1, //│
                                :28, //┘
                PLLlockC        : 1, //┐
                PLLlockA        : 1, //│
                systemRestarted : 1, //│
                externalClock   : 1, //│
                GBTRxReady      : 1, //│
                GBTRxError      : 1, //│
                GBTRxPhaseError : 1, //│0F
                BCIDsyncLost    : 1, //│
                droppingHits    : 1, //│
                resetCounters   : 1, //│
                forceLocalClock : 1, //│
                resetSystem     : 1, //│
                PMstatusChanged :20; //┘
        HDMIlinkStatus PM_LINK_A[10];//]10-19
        quint32 CH_MASK_A       :10, //┐
                                : 7, //│
                syncErrorInLinkA:10, //│
                sideAenabled    : 1, //│1A
                delayRangeErrorA: 1, //│
                readinessChangeA: 1, //│
                sideAready      : 1, //┘
                LASER_DIV       :24, //┐
                                : 7, //│1B
                LASER_ON        : 1, //┘
                LASER_PATTERN_1,     //]1C
                LASER_PATTERN_0,     //]1D
                reserved1[18];       //]1E-2F
        HDMIlinkStatus PM_LINK_C[10];//]30-39
        quint32 CH_MASK_C       :10, //┐
                                : 7, //│
                syncErrorInLinkC:10, //│
                sideCenabled    : 1, //│3A
                delayRangeErrorC: 1, //│
                readinessChangeC: 1, //│
                sideCready      : 1, //┘
                reserved2[21],       //]3B-4F
                COUNTERS_UPD_RATE,
                reserved3[15];
        quint32 OR_A_SIGN       :14, //┐
                                :18, //┘60
                OR_A_RATE       :31, //┐
                                : 1, //┘61
                OR_C_SIGN       :14, //┐
                                :18, //┘62
                OR_C_RATE       :31, //┐
                                : 1, //┘63
                SC_SIGN         :14, //┐
                                :18, //┘64
                SC_RATE         :31, //┐
                                : 1, //┘65
                C_SIGN          :14, //┐
                                :18, //┘66
                C_RATE          :31, //┐
                                : 1, //┘67
                V_SIGN          :14, //┐
                                :18, //┘68
                V_RATE          :31, //┐
                                : 1, //┘69
                OR_A_ENABLED    : 1, //┐
                OR_A_MODE       : 2, //│
                OR_C_ENABLED    : 1, //│
                OR_C_MODE       : 2, //│
                SC_ENABLED      : 1, //│
                SC_MODE         : 2, //│6A
                C_ENABLED       : 1, //│
                C_MODE          : 2, //│
                V_ENABLED       : 1, //│
                V_MODE          : 2, //│
                                :17; //┘
        quint32 reserved4[5];
        quint32 CNT_OR_A,                               //┐
                CNT_OR_C,                               //│
                CNT_SC,                                 //│
                CNT_C,                                  //│
                CNT_V,                                  //│
                CNT_bgA,                                //│
                CNT_bgC,                                //│
                CNT_bgA_and_bgC,                        //│70-7E
                CNT_bgA_or_bgC,                         //│
                CNT_orA_or_orC,                         //│
                CNT_orA_and_orC,                        //│
                CNT_bgA_and_not_orA,                    //│
                CNT_bgC_and_not_orC,                    //│
                CNT_bgA_and_not_orA_OR_bgC_and_not_orC, //│
                CNT_bgA_and_not_orA_AND_bgC_and_not_orC;//┘
        quint32 reserved5[89];
        GBTunit GBT;                 //]D8-F7
        quint32 reserved6[4];
        quint32 FPGA_TEMP,           //]FC
                POWER_1V,            //]FD
                POWER_1_8V,          //]FE
                FW_SECOND       : 6, //┐
                FW_MINUTE       : 6, //│
                FW_HOUR         : 5, //│FF
                FW_YEAR         : 6, //│
                FW_MONTH        : 4, //│
                FW_DAY          : 5; //┘
        quint32 COUNTER_VALUES_READOUT,
                COUNTERS_FIFO_LOAD : 10, : 22;
        quint32 reserved7[254];
        TypePM  PM[20];
        quint32 &operator[] (quint16 address){
            return *(reinterpret_cast<quint32 *>(this) + address);}



};


#endif // FEE_H
