//
// Created by wami on 2021/05/28.
//

#include "BeaconScanner.h"
String Scan_UUID;
char message[128];
bool scanBleFindFlg;
uint16_t* deviceType;
uint16_t* deviceNumber;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        char uuid[60];
        BLEAddress addr = advertisedDevice.getAddress();
        int rssi = advertisedDevice.getRSSI();
        std::string data = advertisedDevice.getManufacturerData();

        if(data.length() == 25)
        {
            if((data[0] == 0x4c) && (data[1] == 0x00) && (data[2] == 0x02) && (data[3] == 0x15)) {
                sprintf(uuid,"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"
                        ,data[19],data[18],data[17],data[16],data[15],data[14],data[13],data[12],data[11],data[10]
                        ,data[9],data[8],data[7],data[6],data[5],data[4]);
                int major = (int)(((data[20]&0xff) << 8) + (data[21] & 0xff));
                int minor = (int)(((data[22]&0xff) << 8) + (data[23] & 0xff));
                signed char power = (signed char)(data[24]&0xff);

                if(strcmp(uuid,Scan_UUID.c_str()) == 0){
                    scanBleFindFlg = true;
                    sprintf(message,"addr=%s rssi=%d uuid=%s,major=%d,minor=%d,power=%d",addr.toString().c_str(),rssi,uuid,major,minor,power);
                    Serial.println(message);
                }

            }
        }
    }
};

class QRAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        char uuid[60];
        BLEAddress addr = advertisedDevice.getAddress();
        int rssi = advertisedDevice.getRSSI();
        std::string data = advertisedDevice.getManufacturerData();

        if(data.length() == 25)
        {
            if((data[0] == 0x4c) && (data[1] == 0x00) && (data[2] == 0x02) && (data[3] == 0x15)) {
                sprintf(uuid,"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"
                        ,data[19],data[18],data[17],data[16],data[15],data[14],data[13],data[12],data[11],data[10]
                        ,data[9],data[8],data[7],data[6],data[5],data[4]);
                int major = (int)(((data[20]&0xff) << 8) + (data[21] & 0xff));
                int minor = (int)(((data[22]&0xff) << 8) + (data[23] & 0xff));
                signed char power = (signed char)(data[24]&0xff);

                if(strcmp(uuid,Scan_UUID.c_str()) == 0){
                    scanBleFindFlg = true;
                    *deviceType = major;
                    *deviceNumber = minor;
//                    sprintf(message,"addr=%s rssi=%d uuid=%s,major=%d,minor=%d,power=%d",addr.toString().c_str(),rssi,uuid,major,minor,power);
//                    Serial.println(message);
                }

            }
        }
    }
};


bool BeaconScanner::scan(String uuid,test_t* test) {
    Scan_UUID = uuid;
    scanBleFindFlg = false;
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(),false);   // 重複を許さない場合は、falseに変更のこと。
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = pBLEScan->start(7);
    test->testBLEStatus = message;
    Serial.println("Scan done!");
    return scanBleFindFlg;
}

bool BeaconScanner::scanQR(String uuid,uint16_t* _deviceType,uint16_t* _deviceNumber) {
    Scan_UUID = uuid;
    deviceType = _deviceType;
    deviceNumber = _deviceNumber;
    scanBleFindFlg = false;
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new QRAdvertisedDeviceCallbacks(),false);   // 重複を許さない場合は、falseに変更のこと。
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = pBLEScan->start(1);
    Serial.println("Scan done!");
    return scanBleFindFlg;
}

