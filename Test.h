//
// Created by wami on 2021/05/27.
//

#ifndef IOTWEIGHTSCALECHECKERSOFTWARE_TESTSTRUCT_H
#define IOTWEIGHTSCALECHECKERSOFTWARE_TESTSTRUCT_H
#include "Arduino.h"
#define TEST_WAIT 0
#define TEST_OK 1
#define TEST_NG 2

typedef struct {
    uint8_t testShortCheck;
    String testShortStatus;

    uint8_t testProgramWriteCheck;
    String testProgramWriteStatus;

    uint8_t testPowerUsbCheck;
    String testPowerUsbStatus;

    uint8_t testPowerBatteryCheck;
    String testPowerBatteryStatus;

    uint8_t testVoltageCheck;
    String testVoltageStatus;

    uint8_t testADCCheck;
    String testADCStatus;

    uint8_t testLEDCheck;
    String testLedStatus;

    uint8_t testTempCheck;
    String testTempStatus;

    uint8_t testUUIDWriteCheck;
    String testUUIDWriteStatus;

    uint8_t testButtonSwCheck;
    String testButtonSwStatus;

    uint8_t testSleepCurrentCheck;
    String testSleepCurrentStatus;

    uint8_t testButtonResetCheck;
    String testButtonResetStatus;

    uint8_t testUUIDReadCheck;
    String testUUIDReadStatus;

    uint8_t testFirmwareUpdateCheck;
    String testFirmwareUpdateStatus;

    uint8_t testBLECheck;
    String testBLEStatus;

    uint8_t testAllCheck;
} test_t;

#define ADC_SCAN_MAX 3
typedef struct {
    int adcScan[ADC_SCAN_MAX];
    int adcBase;
    uint8_t adcScanCheck[ADC_SCAN_MAX];
} adc_result_t;

void resetTest(test_t *result);
void resetAdcResult(adc_result_t *result);
String ngAllResult(test_t* result);
void checkAllResult(test_t* result);
#endif //IOTWEIGHTSCALECHECKERSOFTWARE_TESTSTRUCT_H
