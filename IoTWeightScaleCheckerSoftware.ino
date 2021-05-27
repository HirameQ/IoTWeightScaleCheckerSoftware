#include "SwitchCheck.h"
#include "GPIOChecker.h"
#include "Temperature.h"
#include <M5Stack.h>
#include <ArduinoJson.h>
SwitchCheck switchCheck = SwitchCheck();
GPIOChecker gpioChecker;
Temperature temperature;
#define TEST_WAIT 0
#define TEST_OK 1
#define TEST_NG 2


typedef struct {
    uint8_t testShortCheck;
    uint8_t testPowerUsbCheck;
    uint8_t testPowerBatteryCheck;
    uint8_t testProgramWriteCheck;
    uint8_t testADCCheck;
    uint8_t testSleepCurrentCheck;
    uint8_t testLEDCheck;
    uint8_t testTempCheck;
    uint8_t testUUIDWrite;
    uint8_t testButtonResetCheck;
    uint8_t testButtonSwCheck;
    uint8_t testUUIDCheck;
    uint8_t testOTACheck;
    uint8_t testBLECheck;
    uint8_t testAllCheck;

} test_t;
bool updateFlg = false;

void showLogo(){
    M5.Lcd.fillScreen(WHITE);
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    M5.Lcd.drawJpgFile(SPIFFS, "/logo.jpg", 25, 0);
    M5.Lcd.setCursor(80, 230);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.printf("System Start 15s waiting...");
}

void setUpdateDisplay(){
    updateFlg = true;
}

void showStatus(const char* title, uint8_t status){
    if(status == TEST_WAIT) {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.printf("%s : WAIT\n",title);
    } else if (status == TEST_OK){
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.printf("%s : OK\n",title);
    } else {
        M5.Lcd.setTextColor(TFT_MAGENTA);
        M5.Lcd.printf("%s : NG\n",title);
    }
}

void showTest(test_t result){
    if(!updateFlg)return;
    updateFlg = false;
    M5.Lcd.setTextSize(2);
    if(result.testAllCheck == TEST_WAIT) {
        M5.Lcd.fillScreen(BLACK);
    } else if (result.testAllCheck == TEST_OK){
        M5.Lcd.fillScreen(TFT_BLUE);
    } else {
        M5.Lcd.fillScreen(TFT_RED);
    }
    if(result.testShortCheck == TEST_OK){
        M5.Lcd.setCursor(0, 0);
        showStatus("Short Check",result.testShortCheck);
        showStatus("PowerCheck(USB)",result.testPowerUsbCheck);
        showStatus("PowerCheck(Battery)",result.testPowerBatteryCheck);
        showStatus("ProgramWrite Check",result.testProgramWriteCheck);
        showStatus("ADC Check",result.testADCCheck);
        showStatus("SleepCurrentCheck",result.testSleepCurrentCheck);
        showStatus("LED Check",result.testLEDCheck);
        showStatus("Temp Check",result.testTempCheck);
        showStatus("UUID Write",result.testUUIDWrite);
        showStatus("Button Check(SW)",result.testButtonSwCheck);
        showStatus("Button Check(Reset)",result.testButtonResetCheck);
        showStatus("UUID Check",result.testUUIDCheck);
        showStatus("OTA Update",result.testOTACheck);
        showStatus("BLE Check",result.testBLECheck);

    } else {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setCursor(12, 5);
        M5.Lcd.printf("Weight Scale Check Device");
        M5.Lcd.drawFastHLine(10, 25, 300, WHITE);
        M5.Lcd.setCursor(55, 40);
        M5.Lcd.setTextSize(2);
        M5.Lcd.printf("Place the board as");
        M5.Lcd.setCursor(55, 60);
        M5.Lcd.printf("shown in the image");
        M5.Lcd.drawJpgFile(SPIFFS, "/board.jpg", 60, 100);
    }
}

void resetTest(test_t *result){
    result->testShortCheck = TEST_WAIT;
    result->testPowerUsbCheck = TEST_WAIT;
    result->testPowerBatteryCheck = TEST_WAIT;
    result->testProgramWriteCheck = TEST_WAIT;
    result->testADCCheck = TEST_WAIT;
    result->testSleepCurrentCheck = TEST_WAIT;
    result->testLEDCheck = TEST_WAIT;
    result->testTempCheck = TEST_WAIT;
    result->testUUIDWrite = TEST_WAIT;
    result->testButtonResetCheck = TEST_WAIT;
    result->testButtonSwCheck = TEST_WAIT;
    result->testUUIDCheck = TEST_WAIT;
    result->testOTACheck = TEST_WAIT;
    result->testBLECheck = TEST_WAIT;
    result->testAllCheck = TEST_WAIT;
}

DynamicJsonDocument doc(2048);
DynamicJsonDocument testJson(1024);
void setup() {
    delay(500);
    M5.begin();

    Serial.println("--------------");
    Serial.println("Checker Start!");
    Serial.println("--------------");

    showLogo();

    gpioChecker.begin();
    gpioChecker.writePowerOff();

    temperature.begin();
    Serial2.begin(115200);
    //Serial2.begin(115200, SERIAL_8N1,17,16);

    Serial.println("System Waiting... 15s");
    delay(1500);
    setUpdateDisplay();
    testJson["title"] = "deviceTest1";
    testJson["criteria"] = "0";
    testJson["rawData"] = "0";
    testJson["result"] = "OK";
    doc["title"] = "重量計出荷テスト";
    doc["version"] = "ver20210526_commit";
    doc["user"] = "ピノベーション";
    doc["testDevice"] = "hq_tp_00078";
    JsonArray data = doc.createNestedArray("testData");
    data.add(testJson);
    serializeJsonPretty(doc, Serial);
    gpioChecker.writeUsbPowerOn();
}

uint8_t testPowerCheck(){
    if(gpioChecker.readVoltage() > 4000){
        Serial.println("Power ON");
        float current = gpioChecker.readBoardCurrent();
        if(current > 10 && current < 100){
            Serial.println("WightScale Hardware Active!");
            setUpdateDisplay();
            return TEST_OK;
        }else{
            Serial.println("WightScale Hardware Inactive!");
        }
    }
    return TEST_NG;
}

test_t test;
void loop() {
    resetTest(&test);
    showTest(test);
    gpioChecker.writeBatteryPowerOn();

//    while (Serial2.available()) {
//        int inByte = Serial2.read();
//        Serial.write(inByte);
//    }
    delay(10);

    test.testShortCheck = testPowerCheck();
    showTest(test);
    Serial.print("test.testShortCheck : ");
    Serial.println(test.testShortCheck);

    if(test.testShortCheck == TEST_OK){
        Serial2.println("TEST:LED:OFF");
        for(int i = 0;i<100;i++) {
            Serial.print("readBoardCurrent : ");
            Serial.println(gpioChecker.readBoardCurrent());
            delay(1);
        }
        showTest(test);
        delay(5000);

        // Finish
        setUpdateDisplay();
    }
    //gpioChecker.writePowerOff();
    delay(1000);
//    gpioChecker.writeUsbPowerOn();
//    Serial.println("writeUsbPowerOn Start!");
//    delay(500);
//    Serial.print("read Current: ");
//    Serial.println(gpioChecker.readBoardCurrent());
//    Serial.print("read Voltage: ");
//    Serial.println(gpioChecker.readBoardVoltage());
//    Serial.print("read convert Voltage: ");
//    Serial.println(gpioChecker.readVoltage());
//    Serial.print("read led: ");
//    Serial.println(gpioChecker.readLedLevel());
//    Serial.print("read temperature: ");
//    Serial.println(temperature.read());
//    delay(5000);
//    gpioChecker.writeBatteryPowerOn();
//    Serial.println("writeBatteryPowerOn Start!");
//    delay(500);
//    Serial.print("read Current: ");
//    Serial.println(gpioChecker.readBoardCurrent());
//    Serial.print("read Voltage: ");
//    Serial.println(gpioChecker.readBoardVoltage());
//    Serial.print("read convert Voltage: ");
//    Serial.println(gpioChecker.readVoltage());
//    Serial.print("read led: ");
//    Serial.println(gpioChecker.readLedLevel());
//    Serial.print("read temperature: ");
//    Serial.println(temperature.read());
//    delay(5000);
//    Serial.print("read Voltage: ");
//    Serial.println(gpioChecker.readVoltage());
//
//    gpioChecker.writeUsbPowerOn();
//    switchCheck.resetSwPush();
//    switchCheck.zeroSwPush();
//
//    gpioChecker.writeW1On();
//    Serial.println("writeW1On");
//    delay(5000);
//    gpioChecker.writeW2On();
//    Serial.println("writeW2On");
//    delay(5000);
//    gpioChecker.writeW3On();
//    Serial.println("writeW3On");
//    delay(5000);
}


// text print
//M5.Lcd.fillScreen(BLACK);
//M5.Lcd.setCursor(10, 10);
//M5.Lcd.setTextColor(WHITE);
//M5.Lcd.setTextSize(1);
//M5.Lcd.printf("Display Test!");
//
//delay(1000);