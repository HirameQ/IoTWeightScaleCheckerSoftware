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
    bool readLeftButton();
    void showADCTest(adc_result_t result[],int level,int scan,int base);
    void showProgramWriteError(String programName);
    void showProgramWrite(String programName);
    void showQRReadWait();
    void showQRReadDecision(String hirameQId);
    void showTestResult(test_t result,String errorMessage,String hirameQId);
private:
    void showStatus(const char* title, uint8_t status);
    void showAdcStatus(int base ,int scan, uint8_t status);
};


#endif //IOTWEIGHTSCALECHECKERSOFTWARE_DISPLAY_H
