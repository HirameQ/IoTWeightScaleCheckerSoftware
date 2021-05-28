//
// Created by wami on 2021/05/28.
//

#ifndef IOTWEIGHTSCALECHECKERSOFTWARE_JSON_H
#define IOTWEIGHTSCALECHECKERSOFTWARE_JSON_H

#include <ArduinoJson.h>
#include "Test.h"
class Json {
public:
    void createJson(test_t test);

private:
    void addTestJson(JsonArray array,String title,String criteria,String status,uint8_t result);
};


#endif //IOTWEIGHTSCALECHECKERSOFTWARE_JSON_H
