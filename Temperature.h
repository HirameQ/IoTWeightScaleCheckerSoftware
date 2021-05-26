//
// Created by wami on 2021/05/26.
//

#ifndef IOTWIGHTSCALECHECKERSOFTWARE_TEMPERATURE_H
#define IOTWIGHTSCALECHECKERSOFTWARE_TEMPERATURE_H

#include <Wire.h>
#include "src/BME280/SparkFunBME280.h"


class Temperature {
public:
    void begin();
    float read();
private:
    BME280 sensor;
};


#endif //IOTWIGHTSCALECHECKERSOFTWARE_TEMPERATURE_H
