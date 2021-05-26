//
// Created by wami on 2021/05/26.
//

#include "Temperature.h"

void Temperature::begin() {
    Wire.begin();
    sensor.setStandbyTime(2);
    sensor.setFilter(2);
    sensor.setTempOverSample(2);
    sensor.setI2CAddress(0x76);
    if (sensor.beginI2C()) {
        return;
    }

    sensor.setI2CAddress(0x77);
    if (sensor.beginI2C()) {
        return;
    }

    Serial.println("Sensor connect failed");
    while(1) { }
}

float Temperature::read() {
    return sensor.readTempC();
}