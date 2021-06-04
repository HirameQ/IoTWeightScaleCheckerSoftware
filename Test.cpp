//
// Created by wami on 2021/05/27.
//

#include "Test.h"

void resetTest(test_t *result){
    result->testShortCheck = TEST_WAIT;
    result->testShortStatus = "No Response";

    result->testProgramWriteCheck = TEST_WAIT;
    result->testProgramWriteStatus = "No Response";

    result->testPowerUsbCheck = TEST_WAIT;
    result->testPowerUsbStatus = "No Response";

    result->testPowerBatteryCheck = TEST_WAIT;
    result->testPowerBatteryStatus = "No Response";

    result->testVoltageCheck = TEST_WAIT;
    result->testVoltageStatus = "No Response";

    result->testADCCheck = TEST_WAIT;
    result->testADCStatus = "No Response";

    result->testLEDCheck = TEST_WAIT;
    result->testLedStatus = "No Response";

    result->testTempCheck = TEST_WAIT;
    result->testTempStatus = "No Response";

    result->testUUIDWriteCheck = TEST_WAIT;
    result->testUUIDWriteStatus = "No Response";

    result->testButtonSwCheck = TEST_WAIT;
    result->testButtonSwStatus = "No Response";

    result->testSleepCurrentCheck = TEST_WAIT;
    result->testSleepCurrentStatus = "No Response";

    result->testButtonResetCheck = TEST_WAIT;
    result->testButtonResetStatus = "No Response";

    result->testUUIDReadCheck = TEST_WAIT;
    result->testUUIDReadStatus = "No Response";

    result->testFirmwareUpdateCheck = TEST_WAIT;
    result->testFirmwareUpdateStatus = "No Response";

    result->testBLECheck = TEST_WAIT;
    result->testBLEStatus = "No Response";

    result->testAllCheck = TEST_WAIT;
}

bool checkNg(uint8_t checkFlg){
    if(checkFlg != TEST_WAIT){
        if(checkFlg == TEST_NG){
            return true;
        }
    }
    return false;
}

void checkAllResult(test_t* result){
    result->testAllCheck = TEST_OK;
    if(checkNg(result->testShortCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testProgramWriteCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testPowerUsbCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testPowerBatteryCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testVoltageCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testADCCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testLEDCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testTempCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testUUIDWriteCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testButtonSwCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testSleepCurrentCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testButtonResetCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testUUIDReadCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testFirmwareUpdateCheck)){
        result->testAllCheck = TEST_NG;
    }
    if(checkNg(result->testBLECheck)){
        result->testAllCheck = TEST_NG;
    }
}

String ngAllResult(test_t* result){
    String ngMessage = "";
    if(checkNg(result->testShortCheck)){
        ngMessage += "Short Error, ";
    }
    if(checkNg(result->testProgramWriteCheck)){
        ngMessage += "TestProgram Write Error, ";
    }
    if(checkNg(result->testPowerUsbCheck)){
        ngMessage += "PowerUSB Error, ";
    }
    if(checkNg(result->testPowerBatteryCheck)){
        ngMessage += "PowerBattery Error, ";
    }
    if(checkNg(result->testVoltageCheck)){
        ngMessage += "Voltage Error, ";
    }
    if(checkNg(result->testADCCheck)){
        ngMessage += "ADC Error, ";
    }
    if(checkNg(result->testLEDCheck)){
        ngMessage += "LED Error, ";
    }
    if(checkNg(result->testTempCheck)){
        ngMessage += "Temp Error, ";
    }
    if(checkNg(result->testUUIDWriteCheck)){
        ngMessage += "UUID Write Error, ";
    }
    if(checkNg(result->testButtonSwCheck)){
        ngMessage += "ButtonSW Error, ";
    }
    if(checkNg(result->testSleepCurrentCheck)){
        ngMessage += "Sleep Error, ";
    }
    if(checkNg(result->testButtonResetCheck)){
        ngMessage += "ButtonReset Error, ";
    }
    if(checkNg(result->testUUIDReadCheck)){
        ngMessage += "UUID Read Error, ";
    }
    if(checkNg(result->testFirmwareUpdateCheck)){
        ngMessage += "ReleaseProgram Error, ";
    }
    if(checkNg(result->testBLECheck)){
        ngMessage += "BLE Error, ";
    }
    return ngMessage;
}

void resetAdcResult(adc_result_t *result){
    result->adcBase = 0;

    for (int i = 0; i < ADC_SCAN_MAX; ++i) {
        result->adcScan[i] = 0;
        result->adcScanCheck[i] = TEST_WAIT;
    }
}