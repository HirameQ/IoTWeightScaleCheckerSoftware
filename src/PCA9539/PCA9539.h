/*
 * PCA9539.h
 *
 *  Created on: 27 jul. 2015
 *      Author: Nico
 */

//
// Created by wami on 2020/12/20.
// Copyright (c) 2020 HirameQ. All rights reserved.
//

//TODO: パラレルで書き込める機能追加したい 12/22

#ifndef PCA9539_H_
#define PCA9539_H_

#include "Arduino.h"

//
// PCA9555 defines
//
#define NXP_INPUT      0
#define NXP_OUTPUT     2
#define NXP_INVERT     4
#define NXP_CONFIG     6

class PCA9539 {
public:
    PCA9539(uint8_t address);                            // constructor
    void pinMode(uint8_t pin, uint8_t IOMode );          // pinMode
    void allPinMode(uint8_t a_channel, uint8_t b_channel );          // pinMode
    uint8_t digitalRead(uint8_t pin);                    // digitalRead
    void digitalWrite(uint8_t pin, uint8_t value );      // digitalWrite
    void allDigitalWrite(uint8_t a_channel, uint8_t b_channel );      // digitalWrite

private:
    //
    // low level methods
    //
    uint16_t I2CGetValue(uint8_t address, uint8_t reg);
    void I2CSetValue(uint8_t address, uint8_t reg, uint8_t value);

    union {
        struct {
            uint8_t _configurationRegister_low;          // low order byte
            uint8_t _configurationRegister_high;         // high order byte
        };
        uint16_t _configurationRegister;                 // 16 bits presentation
    };
    union {
        struct {
            uint8_t _valueRegister_low;                  // low order byte
            uint8_t _valueRegister_high;                 // high order byte
        };
        uint16_t _valueRegister;
    };
    uint8_t _address;                                    // address of port this class is supporting
    int _error;                                          // error code from I2C
};


#endif /* PCA9539_H_ */