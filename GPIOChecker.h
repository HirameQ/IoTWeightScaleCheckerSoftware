//
// Created by wami on 2021/05/25.
//

#ifndef IOTWIGHTSCALECHECKERSOFTWARE_GPIO_H
#define IOTWIGHTSCALECHECKERSOFTWARE_GPIO_H

#include "src/PCA9539/PCA9539.h"
#include "src/ina226/INA226PRC.h"

class GPIOChecker {
public:
    void begin();

    uint16_t readVoltage();
    uint16_t readLedLevel();
    bool readIO0();

    void writeBatteryPowerOn();
    void writeUsbPowerOn();
    void writePowerOff();

    float readBoardCurrent(void);
    float readBoardVoltage(void);

    void writeWOff();
    void writeW1On();
    void writeW2On();
    void writeW3On();
};


#endif //IOTWIGHTSCALECHECKERSOFTWARE_GPIO_H
