//
// Created by wami on 2021/05/28.
//

#include "Json.h"
DynamicJsonDocument doc(4096);

void Json::addTestJson(JsonArray array,String title,String criteria,String status,uint8_t result) {
    DynamicJsonDocument testJson(1024);
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
    doc["title"] = "重量計出荷テスト";
    doc["version"] = "ver20210528_commit";
    doc["user"] = "ピノベーション";
    doc["testDevice"] = "hq_tp_00078";
    JsonArray data = doc.createNestedArray("testData");

    serializeJsonPretty(doc, Serial);
}