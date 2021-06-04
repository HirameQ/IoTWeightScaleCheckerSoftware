#include "SwitchCheck.h"
#include "GPIOChecker.h"
#include "Temperature.h"
#include "Display.h"
#include "Test.h"
#include "BeaconScanner.h"
#include "Json.h"
SwitchCheck switchCheck = SwitchCheck();
GPIOChecker gpioChecker;
Temperature temperature;
Display display;
BeaconScanner scanner;
Json json;

bool testCommand(char* testMode,char* testStatus,uint8_t timeout100mSec = 5 ,bool debug = false,HardwareSerial hardwareSerial = Serial2);
bool commandRead(HardwareSerial hardwareSerial,int timeoutSec);
bool commandParser();
bool serialClear();

char command[100];
char mode[50];
char status[50];
uint8_t commandIndex = 0;

test_t test;

String UUID = "";

void setUUID(String uuid){
    UUID = uuid;
}

String getUUID(){
    return UUID;
}

String HirameQDeviceId = "";

void setHirameQDeviceId(String deviceId){
    HirameQDeviceId = deviceId;
}

String getHirameQDeviceId(){
    return HirameQDeviceId;
}

String DeviceChipId = "";

void setChipId(String chipId){
    DeviceChipId = chipId;
}

String getChipId(){
    return DeviceChipId;
}

void setup() {
    delay(500);
    display.begin();

    Serial.println("--------------");
    Serial.println("Checker Start!");
    Serial.println("--------------");

    display.showLogo();

    gpioChecker.begin();
    gpioChecker.writePowerOff();

    temperature.begin();

    Serial.println("System Waiting... 15s");
    delay(1500);
    resetTest(&test);
    display.showTest(test);
}

bool bleScanQRDeviceId(){
    uint16_t deviceType = 65535;
    uint16_t deviceNumber = 65535;
    display.showQRReadWait();
    while (deviceNumber == 65535){
        scanner.scanQR("7D9A25E4-1BC2-0BDF-02CB-C7E26C48BE5C",&deviceType,&deviceNumber);
        Serial.printf("QR DeviceType: %d, DeviceNumber: %d\n",deviceType,deviceNumber);
    }

    char deviceId[20];
    if(deviceType == 0) {
        sprintf(deviceId, "hq_mp_%05d",deviceNumber);
    } else {
        sprintf(deviceId, "hq_tp_%05d",deviceNumber);
    }
    setHirameQDeviceId(deviceId);
}

//voltage > 4000 ,10 < current < 200
uint8_t testShortCheck(test_t *result){
    uint16_t voltage = gpioChecker.readVoltage();
    float current = gpioChecker.readBoardCurrent();
    result->testShortStatus = (String)"voltage:"+voltage+",current: "+current;
    if(voltage > 4000){
        //Serial.println(result->testShortStatus);
        if(current > 10 && current < 200) {
            Serial.println("testShortCheck PASS");
            result->testShortCheck = TEST_OK;
            return TEST_OK;
        }
       // Serial.println("testShortCheck NG");
    }
    result->testShortCheck = TEST_NG;
    return TEST_NG;
}

bool pcTestProgramWrite(){
    Serial2.end();
    pinMode(17,INPUT);
    pinMode(16,INPUT);
    Serial.println("TEST:PROGRAM:TEST_PROGRAM");
    delay(1000);
    if(testCommand("PROGRAM","FINISH_TEST_PROGRAM",200, false,Serial)){
        Serial2.begin(115200);
        Serial.println("pcTestProgramWrite true");
        return true;
    }
    Serial.println("pcTestProgramWrite false");
    return false;
}

//read check program version
uint8_t testCheckProgramWriteCheck(test_t *result){
    delay(5000);
    serialClear();
    for (int i = 0; i < 20; ++i) {
        Serial2.println("TEST:PROGRAM:VERSION");
        delay(500);
        if(testCommand("PROGRAM",NULL)) {
            result->testProgramWriteStatus = (String)"version:"+status;
            Serial.println(result->testProgramWriteStatus);
            Serial.println("testCheckProgramWriteCheck PASS");
            result->testProgramWriteCheck = TEST_OK;
            return TEST_OK;
        }
        delay(500);
    }
    Serial.println("testCheckProgramWriteCheck NG");
    result->testProgramWriteCheck = TEST_NG;
    return TEST_NG;
}

uint8_t testIdCheck(){
    serialClear();
    Serial2.println("TEST:ID:READ");
    delay(500);
    if(testCommand("ID",NULL,10)) {
        setChipId(status);
        Serial.println("testIdCheck PASS");
        Serial.println(getChipId());
        return TEST_OK;
    }
    Serial.println("testIdCheck NG");
    return TEST_NG;
}

//voltage > 4000 , boardVoltage < 4500
uint8_t testPowerUsbCheck(test_t *result){
    delay(100);
    uint16_t voltage = gpioChecker.readVoltage();
    float boardVoltage = gpioChecker.readBoardVoltage();
    result->testPowerUsbStatus = (String)"voltage:"+voltage+",UsbVoltage: "+boardVoltage;
    Serial.println(result->testPowerUsbStatus);
    if(voltage > 4000){
        if(boardVoltage > 4500) {
            Serial.println("testPowerUsbCheck PASS");
            result->testPowerUsbCheck = TEST_OK;
            return TEST_OK;
        }
    }
    Serial.println("testPowerUsbCheck NG");
    result->testPowerUsbCheck = TEST_NG;
    return TEST_NG;
}

//voltage > 4000 , boardVoltage < 4000
uint8_t testPowerBatteryCheck(test_t *result){
    delay(100);
    uint16_t voltage = gpioChecker.readVoltage();
    float boardVoltage = gpioChecker.readBoardVoltage();
    result->testPowerBatteryStatus = (String)"voltage:"+voltage+",batteryVoltage: "+boardVoltage;
    Serial.println(result->testPowerBatteryStatus);
    if(voltage > 4000){
        if(boardVoltage > 4500) {
            Serial.println("testPowerBatteryCheck PASS");
            result->testPowerBatteryCheck = TEST_OK;
            return TEST_OK;
        }
    }
    Serial.println("testPowerBatteryCheck NG");
    result->testPowerBatteryCheck = TEST_NG;
    return TEST_NG;
}

//boardVoltage-200 < scanVoltage < boardVoltage+200
uint8_t testVoltageCheck(test_t *result){
    serialClear();
    for (int i = 0; i < 5; ++i) {
        Serial2.println("TEST:VOLTAGE:READ");
        delay(500);
        if(testCommand("VOLTAGE",NULL)) {
            int boardVoltage = (int)gpioChecker.readBoardVoltage();
            int scanVoltage = atoi(status);
            result->testVoltageStatus = (String)"scanVoltage:"+status+",batteryVoltage: "+boardVoltage;
            Serial.println(result->testVoltageStatus);
            if(boardVoltage-200 < scanVoltage && scanVoltage < boardVoltage + 200){

                Serial.println("testVoltageCheck PASS");
                result->testVoltageCheck = TEST_OK;
                return TEST_OK;
            }
        }
    }
    Serial.println("testVoltageCheck NG");
    result->testVoltageCheck = TEST_NG;
    return TEST_NG;
}

#define ADC_LEVEL 3
//baseLevel-baseLevel *0.2 < scanADC < baseLevel+baseLevel *0.2
uint8_t testADCCheck(test_t *result){
    int baseLevel = 0;
    int diffLevel = 0;
    bool error = false;
    serialClear();
    adc_result_t adcResult[ADC_LEVEL];
    for (int y = 0; y < ADC_LEVEL; ++y) {
        resetAdcResult(&adcResult[y]);
    }
    display.showADCTest(adcResult ,ADC_LEVEL,ADC_SCAN_MAX,baseLevel);
    gpioChecker.writeWOff();//0mV
    delay(1000);
    for (int i = 0; i < ADC_SCAN_MAX; ++i) {
        Serial2.println("TEST:ADC:READ");
        if(testCommand("ADC",NULL,15)) {
            baseLevel += atoi(status);
        }
    }
    baseLevel = abs( baseLevel / ADC_SCAN_MAX );
    Serial.printf("Base ADC : %d",baseLevel);
    display.showADCTest(adcResult ,ADC_LEVEL,ADC_SCAN_MAX,baseLevel);
    for (int y = 0; y < ADC_LEVEL; ++y) {
        if(y==0){
            gpioChecker.writeW1On();//0.4mV
            diffLevel = 850;
        }else if(y == 1){
            gpioChecker.writeW2On();//0.54mV
            diffLevel = 1180;
        }else{
            gpioChecker.writeW3On();//1.08mV
            diffLevel = 2400;
        }
        adcResult[y].adcBase = diffLevel;
        delay(500);
        for (int i = 0; i < ADC_SCAN_MAX; ++i) {
            Serial2.println("TEST:ADC:READ");
            delay(500);
            if(testCommand("ADC",NULL)) {
                int scanAdc = abs(atoi(status)) - baseLevel;
                adcResult[y].adcScan[i] = scanAdc;
                if(result->testADCStatus.indexOf("No Response")==0){
                    result->testADCStatus = "";
                }
                result->testADCStatus += (String)"scan:"+ scanAdc +",base: "+diffLevel;

                if(!(diffLevel- diffLevel * 0.2 < scanAdc && scanAdc < diffLevel + diffLevel * 0.2 )){
                    adcResult[y].adcScanCheck[i] = TEST_NG;
                    error = true;
                    Serial.println("testADCCheck NG");
                }else{
                    adcResult[y].adcScanCheck[i] = TEST_OK;
                }
                display.showADCTest(adcResult ,ADC_LEVEL,ADC_SCAN_MAX,baseLevel);
            }else{
                error = true;
                Serial.println("testADCCheck no response NG");
            }
            delay(500);
        }
    }
    Serial.println(result->testADCStatus);

    if(!error){
        Serial.println("testADCCheck PASS");
        result->testADCCheck = TEST_OK;
        return TEST_OK;
    }
    Serial.println("testADCCheck NG");
    result->testADCCheck = TEST_NG;
    return TEST_NG;
}

//LED OFF < 100 ,LED ON > 2500
uint8_t testLedCheck(test_t *result){
    uint16_t offLevel = 5000;
    uint16_t onLevel = 0;
    serialClear();
    for (int i = 0; i < 2; ++i) {
        Serial2.println("TEST:LED:OFF");
        if(testCommand("LED","OFF")) {
            offLevel = gpioChecker.readLedLevel();
        }else{
            offLevel = gpioChecker.readLedLevel();
        }
        if(offLevel < 100)break;
        while (true){
            display.showDarkEnvironment(gpioChecker.readLedLevel());
            if(display.readCenterButton())break;
            delay(100);
        }
        delay(500);
    }
    for (int i = 0; i < 2; ++i) {
        Serial2.println("TEST:LED:ON");
        if(testCommand("LED","ON")) {
            onLevel = gpioChecker.readLedLevel();
        }else{
            onLevel = gpioChecker.readLedLevel();
        }
        if(onLevel > 2500)break;
        delay(500);
    }
    result->testLedStatus = (String)"led off:"+offLevel+",led on: "+onLevel;
    Serial.println(result->testLedStatus);
    if(offLevel < 100 && onLevel > 2500){
        Serial.println("testLedCheck PASS");
        result->testLEDCheck = TEST_OK;
        return TEST_OK;
    }
    Serial.println("testLedCheck NG");
    result->testLEDCheck = TEST_NG;
    return TEST_NG;
}

//boardTemp-2 < scanVoltage < boardTemp+2
uint8_t testTempCheck(test_t *result){
    serialClear();
    for (int i = 0; i < 5; ++i) {
        Serial2.println("TEST:TEMP:READ");
        delay(500);
        if(testCommand("TEMP",NULL)) {
            float boardTemp = 0;
            for (int j = 0; j < 5; ++j) {
                boardTemp += temperature.read();
            }
            boardTemp /= 5;
            float scanTemp = atof(status);
            result->testTempStatus = (String)"scanTemp:"+status+",systemTemp: "+boardTemp;
            Serial.println(result->testTempStatus);
            if(boardTemp-2 < scanTemp && scanTemp < boardTemp + 2){
                Serial.println("testTempCheck PASS");
                result->testTempCheck = TEST_OK;
                return TEST_OK;
            }
        }
    }
    Serial.println("testTempCheck NG");
    result->testTempCheck = TEST_NG;
    return TEST_NG;
}

bool pcUUIDWrite(){
    Serial.println((String)"TEST:UUID_WRITE:"+getHirameQDeviceId());
    delay(1000);
    if(testCommand("UUID_WRITE",NULL,50, false,Serial)){
        setUUID(status);
        return true;
    }
    return false;
}

//response ok
uint8_t testUUIDWrite(test_t *result){
    serialClear();
    Serial2.println((String)"TEST:UUID_WRITE:"+getUUID());
    delay(500);
    if(testCommand("UUID_WRITE","OK")) {
        result->testUUIDWriteStatus = (String)"UUID:"+getUUID();
        Serial.println(result->testUUIDWriteStatus);
        Serial.println("testUUIDWrite PASS");
        result->testUUIDWriteCheck = TEST_OK;
        return TEST_OK;
    }
    Serial.println("testUUIDWrite NG");
    result->testUUIDWriteCheck = TEST_NG;
    return TEST_NG;
}

//response ok
uint8_t testSwCheck(test_t *result){
    serialClear();
    Serial2.println("TEST:SW:READ");
    delay(500);
    switchCheck.zeroSwPush();
    if(testCommand("SW",NULL)) {
        if(strcmp(status, "OK") == 0) {
            result->testButtonSwStatus = "SW:OK";
            Serial.println(result->testButtonSwStatus);
            Serial.println("testSwCheck PASS");
            result->testButtonSwCheck = TEST_OK;
            return TEST_OK;
        }
    }
    result->testButtonSwStatus = "SW:TIMEOUT";
    Serial.println(result->testButtonSwStatus);
    Serial.println("testSwCheck NG");
    result->testButtonSwCheck = TEST_NG;
    return TEST_NG;
}

// sleep start & current < 5
uint8_t testSleepCheck(test_t *result){
    serialClear();
    Serial2.println("TEST:SLEEP:START");
    delay(1500);
    if(testCommand("SLEEP","START")) {
        float current = 0;
        for (int i = 0; i < 10; ++i) {
            current += gpioChecker.readBoardCurrent();
            delay(50);
        }
        current /= 10;
        result->testSleepCurrentStatus = (String)"SLEEP current: "+ current;
        Serial.println(result->testSleepCurrentStatus);
        if(current < 5) {
            Serial.println("testSleepCheck PASS");
            result->testSleepCurrentCheck = TEST_OK;
            return TEST_OK;
        }
    }
    Serial.println("testSleepCheck NG");
    result->testSleepCurrentCheck = TEST_NG;
    return TEST_NG;
}

//response ok
uint8_t testResetCheck(test_t *result){
    serialClear();
    switchCheck.resetSwPush();
    unsigned long targetTime = millis() + 6000;
    while (targetTime > millis()) {
        if (testCommand("SYSTEM", "START")) {
            result->testButtonResetStatus = "Reset:OK";
            Serial.println(result->testButtonResetStatus);
            Serial.println("testResetCheck PASS");
            result->testButtonResetCheck = TEST_OK;
            return TEST_OK;
        }
    }
    result->testButtonResetStatus = "Reset:TIMEOUT";
    Serial.println(result->testButtonResetStatus);
    Serial.println("testResetCheck NG");
    result->testButtonResetCheck = TEST_NG;
    return TEST_NG;
}

//uuid match
uint8_t testUUIDCheck(test_t *result){
    serialClear();
    Serial2.println((String)"TEST:UUID_READ:");
    delay(1000);
    Serial.println("testUUIDCheck start");
    if(testCommand("UUID_READ",(char *)getUUID().c_str(),10, true)) {
        result->testUUIDReadStatus = (String)"UUID:"+status;
        Serial.println(result->testUUIDReadStatus);
        Serial.println("testUUIDCheck PASS");
        Serial.println(command);
        result->testUUIDReadCheck = TEST_OK;
        return TEST_OK;
    }
    Serial.println("testUUIDCheck NG");
    result->testUUIDReadCheck = TEST_NG;
    return TEST_NG;
}


bool pcReleaseProgramWrite(){
    Serial2.end();
    pinMode(17,INPUT);
    pinMode(16,INPUT);
    Serial.println("TEST:PROGRAM:RELEASE_PROGRAM");
    delay(1000);
    if(testCommand("PROGRAM","FINISH_RELEASE_PROGRAM",200, false,Serial)){
        Serial2.begin(115200);
        return true;
    }
    return false;
}

//response ok
uint8_t testReleaseProgramWriteCheck(test_t *result){
    serialClear();
    switchCheck.resetSwPush();
    delay(1000);
    unsigned long targetTime = millis() + 3000;
    while (targetTime > millis()) {
        if (commandRead(Serial2,1)) {
            Serial.println(command);
            Serial.println(strncmp(command, "*********** ZAICON start!",25));
            if (strncmp(command, "*********** ZAICON start!",25) == 0) {
                result->testFirmwareUpdateStatus = command;
                Serial.println(result->testFirmwareUpdateStatus);
                Serial.println("testReleaseProgramWriteCheck PASS");
                result->testFirmwareUpdateCheck = TEST_OK;
                return TEST_OK;
            }
        }
    }
    Serial.println("testReleaseProgramWriteCheck NG");
    result->testFirmwareUpdateCheck = TEST_NG;
    return TEST_NG;
}

//uuid match
uint8_t testBleCheck(test_t *result){
    serialClear();
    delay(5000);
    if( scanner.scan(getUUID(),result)) {
        Serial.println(result->testBLEStatus);
        Serial.println("testBleCheck PASS");
        result->testBLECheck = TEST_OK;
        return TEST_OK;
    }
    Serial.println("testBleCheck NG");
    result->testBLECheck = TEST_NG;
    return TEST_NG;
}

void pcDeviceInfo(){
    Serial.println("TEST:INFO:"+getChipId()+":"+getHirameQDeviceId()+":"+getUUID());
}

void pcTestResult(test_t test){
    Serial.print("TEST:RESULT:"+getHirameQDeviceId()+":");
    json.createJson(test);
    Serial.println();
}

void loop() {
    if(test.testShortCheck == TEST_OK) {
        resetTest(&test);
        display.showTest(test);
    }
    gpioChecker.writeUsbPowerOn();

    delay(10);

    testShortCheck(&test);
    if(test.testShortCheck == TEST_OK) {
        delay(1000);
        display.showTest(test);

        if(!pcTestProgramWrite()){
            display.showProgramWriteError("Test Program");
            while (true){
                if(display.readCenterButton())break;
                delay(100);
            }
            Serial.println("Write Test Program Error");
            pcTestProgramWrite();
        }
        display.showTest(test);

        if(testCheckProgramWriteCheck(&test) == TEST_NG){
            gpioChecker.writePowerOff();
            delay(100);
            gpioChecker.writeUsbPowerOn();
            delay(1000);
            if(testCheckProgramWriteCheck(&test) == TEST_NG){
            }
        }
        display.showTest(test);

        bool scanQr = true;
        while (scanQr) {
            bleScanQRDeviceId();
            display.showQRReadDecision(getHirameQDeviceId());
            while (true) {
                if (display.readCenterButton()) {
                    scanQr = false;
                    break;
                }
                if (display.readLeftButton()) {
                    break;
                }
            }
        }

        display.showTest(test);

        testIdCheck();

        gpioChecker.writeUsbPowerOn();
        testPowerUsbCheck(&test);
        display.showTest(test);

        gpioChecker.writeBatteryPowerOn();
        testPowerBatteryCheck(&test);
        display.showTest(test);

        testVoltageCheck(&test);

        gpioChecker.writeUsbPowerOn();
        display.showTest(test);

        testADCCheck(&test);
        display.showTest(test);

        testLedCheck(&test);
        display.showTest(test);

        testTempCheck(&test);
        display.showTest(test);

        if (pcUUIDWrite()){
            testUUIDWrite(&test);
        } else {
            test.testUUIDWriteCheck = TEST_NG;
            test.testUUIDWriteStatus = "PC Data Error";
        }
        display.showTest(test);

        testSwCheck(&test);
        display.showTest(test);

        testSleepCheck(&test);
        display.showTest(test);

        testResetCheck(&test);
        display.showTest(test);

        testUUIDCheck(&test);
        display.showTest(test);

        if(!pcReleaseProgramWrite()){
            display.showProgramWriteError("Release Program");
            while (true){
                if(display.readCenterButton())break;
                delay(100);
            }
            pcReleaseProgramWrite();
        }
        display.showTest(test);

        testReleaseProgramWriteCheck(&test);
        display.showTest(test);

        testBleCheck(&test);
        display.showTest(test);
        checkAllResult(&test);
        pcDeviceInfo();
        pcTestResult(test);

        display.showTestResult(test,ngAllResult(&test),getHirameQDeviceId());
        while (true){
            switchCheck.resetSw();
            if(gpioChecker.readVoltage() < 3000)break;
            delay(100);
        }

    }
    gpioChecker.writePowerOff();
    delay(500);
}


/*****
 * コマンド処理
 *****/
bool testCommand(char* testMode,char* testStatus,uint8_t timeout100mSec,bool debug,HardwareSerial hardwareSerial) {
    unsigned long targetTime = millis() + 100*timeout100mSec;
    while (targetTime > millis()) {
        if (commandRead(hardwareSerial, 1)) {
            if (debug) {
                Serial.printf("command : %s\n",command);
            }
            if (commandParser()) {
                if (debug) {
                    Serial.printf("mode : %s\n",mode);
                    Serial.printf("status : %s\n",status);
                    Serial.printf("testStatusLen : %d ,statusLen : %d , strncmp : %d\n",strlen(testStatus),strlen(status),strncmp(status, testStatus, strlen(testStatus)));
                }
                if (strcmp(mode, testMode) == 0) {
                    if (testStatus == NULL ) return true;
                    if(strncmp(status, testStatus, strlen(testStatus)) == 0 && strlen(testStatus) == strlen(status)) return true;
                    return false;
                }
            }
        }
    }
    return false;
}


bool serialClear(){
    while (Serial2.available()) {
        Serial2.read();
    }
}

bool commandRead(HardwareSerial hardwareSerial,int timeout100mSec){
    //CR LF
    unsigned long targetTime = millis() + 100 * timeout100mSec;
    while (targetTime > millis()) {
        while (hardwareSerial.available()) {
            command[commandIndex] = hardwareSerial.read();
            if(command[commandIndex] == 10 ){//LF
                commandIndex = 0;
                break;
            }
            if(command[commandIndex] == 13 ){//CR
                command[commandIndex] = '\0';
                return true;
            }
            commandIndex++;
        }
    }
    return false;
}

bool commandParser(){
    bool modeFlg = true;
    int index = 0;
    if(command[0] != 'T'){
        commandIndex = 0;
        return false;
    }
    if(command[1] != 'E'){
        commandIndex = 0;
        return false;
    }
    if(command[2] != 'S'){
        commandIndex = 0;
        return false;
    }
    if(command[3] != 'T' ){
        commandIndex = 0;
        return false;
    }
    if(command[4] != ':'){
        commandIndex = 0;
        return false;
    }
    for(uint8_t i = 5;i < commandIndex;i++){
        if(modeFlg){
            if(command[i] == ':' ){
                mode[index] = '\0';
                index = 0;
                modeFlg = false;
                continue;
            }
            mode[index] = command[i];
            index++;
        }else{
            status[index] = command[i];
            index++;
        }
    }
    status[index] = '\0';
    commandIndex = 0;
    return true;
}