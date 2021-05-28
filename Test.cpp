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

    result->testUUIDWrite = TEST_WAIT;
    result->testUUIDStatus = "No Response";

    result->testButtonSwCheck = TEST_WAIT;
    result->testButtonSwStatus = "No Response";

    result->testSleepCurrentCheck = TEST_WAIT;
    result->testSleepCurrentStatus = "No Response";

    result->testButtonResetCheck = TEST_WAIT;
    result->testButtonResetStatus = "No Response";

    result->testUUIDCheck = TEST_WAIT;
    result->testUUIDCheckStatus = "No Response";

    result->testFirmwareUpdateCheck = TEST_WAIT;
    result->testFirmwareUpdateStatus = "No Response";

    result->testBLECheck = TEST_WAIT;
    result->testBLEStatus = "No Response";

    result->testAllCheck = TEST_WAIT;
}

void resetAdcResult(adc_result_t *result){
    result->adcBase = 0;

    for (int i = 0; i < ADC_SCAN_MAX; ++i) {
        result->adcScan[i] = 0;
        result->adcScanCheck[i] = TEST_WAIT;
    }
}