//
// Created by wami on 2021/05/28.
//

#ifndef IOTWEIGHTSCALECHECKERSOFTWARE_BEACONSCANNER_H
#define IOTWEIGHTSCALECHECKERSOFTWARE_BEACONSCANNER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "Arduino.h"
#include "Test.h"
class BeaconScanner {
public:
    bool scan(String uuid,test_t* test);
    bool scanQR(String uuid,uint16_t* _deviceType,uint16_t* _deviceNumber);
};



#endif //IOTWEIGHTSCALECHECKERSOFTWARE_BEACONSCANNER_H
