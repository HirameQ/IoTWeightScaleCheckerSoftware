#include "SwitchCheck.h"
#include "GPIOChecker.h"
#include "Temperature.h"
#include <M5Stack.h>
#include "ImageConverter.h"
SwitchCheck switchCheck = SwitchCheck();
GPIOChecker gpioChecker;
Temperature temperature;

typedef struct {
    bool testPowerFlg;
    char sex;
    int age;
    double height;
    double weight;
} test_t;

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

void showTest(test_t result){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Weight Scale Check Program");
    M5.Lcd.drawFastHLine(10, 20, 200, WHITE);
}

void resetTest(test_t *result){
    result->testPowerFlg = false;
}


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

    Serial.println("System Waiting... 15s");
    delay(15000);
}

bool testPowerCheck(){
    if(gpioChecker.readVoltage() > 4000){
        Serial.println("Power ON");
        float current = gpioChecker.readBoardCurrent();
        if(current > 10 && current < 100){
            Serial.println("WightScale Hardware Active!");
            return true;
        }else{
            Serial.println("WightScale Hardware Inactive!");
        }
    }
    return false;
}

test_t test;
void loop() {
    resetTest(&test);
    showTest(test);
    gpioChecker.writeUsbPowerOn();
    delay(10);

    test.testPowerFlg = testPowerCheck();
    Serial.print("test.testPowerFlg : ");
    Serial.println(test.testPowerFlg);
    gpioChecker.writePowerOff();
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