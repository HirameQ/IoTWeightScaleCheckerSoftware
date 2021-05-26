//
// Created by wami on 2021/05/25.
//

#include "GPIOChecker.h"
#define W1 3
#define W2 5
#define W3 7

PCA9539 pca9539(0x20);
INA226PRC ina226Prc;

void GPIOChecker::begin() {
    pca9539.allPinMode(0, 0b00000010);
    pca9539.allDigitalWrite(0, 0);
    ina226Prc.begin();
}

uint16_t GPIOChecker::readVoltage() {
    return analogRead(35);
}

uint16_t GPIOChecker::readLedLevel() {
    return analogRead(36);
}

bool GPIOChecker::readIO0() {
    return pca9539.digitalRead(10);
}

void GPIOChecker::writeBatteryPowerOn() {
    pca9539.digitalWrite(12,HIGH);
    pca9539.digitalWrite(11,LOW);
}

void GPIOChecker::writeUsbPowerOn() {
    pca9539.digitalWrite(12, LOW);
    pca9539.digitalWrite(11, HIGH);
}

void GPIOChecker::writePowerOff() {
    pca9539.digitalWrite(12, HIGH);
    pca9539.digitalWrite(11, HIGH);
}

float GPIOChecker::readBoardCurrent() {
    return ina226Prc.readCurrent();
}

float GPIOChecker::readBoardVoltage() {
    return ina226Prc.readVoltage();
}

void GPIOChecker::writeW1On() {
    pca9539.digitalWrite(W1, HIGH);
    pca9539.digitalWrite(W2, LOW);
    pca9539.digitalWrite(W3, LOW);
}

void GPIOChecker::writeW2On() {
    pca9539.digitalWrite(W1, LOW);
    pca9539.digitalWrite(W2, HIGH);
    pca9539.digitalWrite(W3, LOW);
}

void GPIOChecker::writeW3On() {
    pca9539.digitalWrite(W1, LOW);
    pca9539.digitalWrite(W2, LOW);
    pca9539.digitalWrite(W3, HIGH);
}