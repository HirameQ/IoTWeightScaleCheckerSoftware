//
// Created by wami on 2021/05/28.
//

#include "Json.h"
DynamicJsonDocument doc(4500);

void Json::addTestJson(JsonArray array,String title,String criteria,String status,uint8_t result) {
    DynamicJsonDocument testJson(1500);
    testJson["title"] = title;
    testJson["criteria"] = criteria;
    testJson["rawData"] = status;
    if(result == TEST_OK){
        testJson["result"] = "OK";
    }else if(result == TEST_NG){
        testJson["result"] = "NG";
    }else{
        testJson["result"] = "Waiting";
    }
    array.add(testJson);
}

void Json::createJson(test_t test) {
    doc.clear();
    doc["title"] = "重量計出荷テスト";
    doc["version"] = "ver20210824_9ab8d0d";
    doc["user"] = "ピノベーション";
    doc["testDevice"] = "hq_tp_00078";
    JsonArray data = doc.createNestedArray("testData");
    addTestJson(data,"ショートテスト","voltage > 4000 ,10 < current < 200",test.testShortStatus,test.testShortCheck);
    addTestJson(data,"テストプログラム書き込みテスト","read check program version",test.testProgramWriteStatus,test.testProgramWriteCheck);
    addTestJson(data,"5V電源起動テスト","voltage > 4000 , boardVoltage < 4500",test.testPowerUsbStatus,test.testPowerUsbCheck);
    addTestJson(data,"電池起動テスト","voltage > 4000 , boardVoltage < 4000",test.testPowerBatteryStatus,test.testPowerBatteryCheck);
    addTestJson(data,"電圧読み取りテスト","boardVoltage-200 < scanVoltage < boardVoltage+200",test.testVoltageStatus,test.testVoltageCheck);
    addTestJson(data,"ADC読み取りテスト","baseLevel-baseLevel *0.2 < scanADC < baseLevel+baseLevel *0.2",test.testADCStatus,test.testADCCheck);
    addTestJson(data,"LEDテスト","LED OFF < 200 ,LED ON > 3000",test.testLedStatus,test.testLEDCheck);
    addTestJson(data,"温度取得テスト","boardTemp-2 < scanVoltage < boardTemp+2",test.testTempStatus,test.testTempCheck);
    addTestJson(data,"UUID書き込み","response ok",test.testUUIDWriteStatus,test.testUUIDWriteCheck);
    addTestJson(data,"スイッチテスト","response ok",test.testButtonSwStatus,test.testButtonSwCheck);
    addTestJson(data,"スリーブテスト","sleep start & current < 5",test.testSleepCurrentStatus,test.testSleepCurrentCheck);
    addTestJson(data,"リセットボタンテスト","response ok",test.testButtonResetStatus,test.testButtonResetCheck);
    addTestJson(data,"UUID読み取りテスト","uuid match",test.testUUIDReadStatus,test.testUUIDReadCheck);
    addTestJson(data,"本番ファームアップデートテスト","serial read　trigger word`*********** ZAICON start!`",test.testFirmwareUpdateStatus,test.testFirmwareUpdateCheck);
    addTestJson(data,"BLE発信テスト","uuid match",test.testBLEStatus,test.testBLECheck);
    serializeJson(doc, Serial);
}