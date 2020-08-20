#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QtGlobal>
#include <tgmath.h>
#include <QRandomGenerator>

class temperature{


public:
    temperature(const int t, const double s){T = t; sigma = s; current_T = t;}

    operator quint32(){
        double gamma = double(QRandomGenerator::global()->generate()) / double(0xffffffff);
        double P = (erf(double(current_T - T)/ (sqrt(2) * sigma)) + 1.0) / 2.0;
        gamma < P ? --current_T : ++current_T;
        return quint32(this->current_T);
    }

private:
    int T, current_T;
    double sigma;


};

#endif // TEMPERATURE_H
