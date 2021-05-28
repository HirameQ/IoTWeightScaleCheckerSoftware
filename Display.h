//
// Created by wami on 2021/05/27.
//

#ifndef IOTWEIGHTSCALECHECKERSOFTWARE_DISPLAY_H
#define IOTWEIGHTSCALECHECKERSOFTWARE_DISPLAY_H
#include "Test.h"
#include <M5Stack.h>
class Display {
public:
    void begin();
    void showLogo();
    void showTest(test_t result);
    void showDarkEnvironment(uint16_t sensor);
    bool readCenterButton();
    void showADCTest(adc_result_t result[],int level,int scan);
private:
    void showStatus(const char* title, uint8_t status);
    void showAdcStatus(int base ,int scan, uint8_t status);
};


#endif //IOTWEIGHTSCALECHECKERSOFTWARE_DISPLAY_H
