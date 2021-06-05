//
// Created by wami on 2021/05/27.
//

#include "Display.h"

void Display::showLogo(){
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

void Display::showStatus(const char* title, uint8_t status){
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

void Display::showAdcStatus(int base ,int scan, uint8_t status){
    if(status == TEST_WAIT) {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.printf("Base %d |Scan %d :WAIT\n",base,scan);
    } else if (status == TEST_OK){
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.printf("Base %d |Scan %d :OK\n",base,scan);
    } else {
        M5.Lcd.setTextColor(TFT_MAGENTA);
        M5.Lcd.printf("Base %d |Scan %d :NG\n",base,scan);
    }
}

void Display::showTest(test_t result){
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
        showStatus("ProgramWrite Check",result.testProgramWriteCheck);
        showStatus("PowerCheck(USB)",result.testPowerUsbCheck);
        showStatus("PowerCheck(Battery)",result.testPowerBatteryCheck);
        showStatus("VoltageCheck",result.testVoltageCheck);

        showStatus("ADC Check",result.testADCCheck);
        showStatus("LED Check",result.testLEDCheck);
        showStatus("Temp Check",result.testTempCheck);
        showStatus("UUID Write",result.testUUIDWriteCheck);
        showStatus("Button Check(SW)",result.testButtonSwCheck);

        showStatus("SleepCurrentCheck",result.testSleepCurrentCheck);
        showStatus("Button Check(Reset)",result.testButtonResetCheck);
        showStatus("UUID Check",result.testUUIDReadCheck);
        showStatus("Firmware Update",result.testFirmwareUpdateCheck);
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

void Display::showADCTest(adc_result_t result[],int level,int scan,int base){
    M5.Lcd.setTextSize(2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(20, 5);
    M5.Lcd.printf("ADC Check BaseLevel: %d",base);
    M5.Lcd.drawFastHLine(10, 25, 300, WHITE);
    M5.Lcd.setCursor(0, 30);
    for (int i = 0; i < level; ++i) {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.printf("--- level %d ----\n",i);
        for (int j = 0; j < scan; ++j) {
            showAdcStatus(result[i].adcBase,result[i].adcScan[j],result[i].adcScanCheck[j]);
        }
    }
}

void Display::showDarkEnvironment(uint16_t sensor){
    M5.Lcd.fillScreen(WHITE);

    M5.Lcd.setCursor(0, 60);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Create a dark environment\nScanValue:%d\nScanValue < 100\nPress the center button\nwhen you're ready\n\n\n           Push!",sensor);
}

void Display::showProgramWrite(String programName){
    M5.Lcd.fillScreen(WHITE);

    M5.Lcd.setCursor(10, 40);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Program Writing..\n\n  ------------\n  %s\n  ------------\n\n  10s wait...",programName.c_str());
}


void Display::showProgramWriteError(String programName){
    M5.Lcd.fillScreen(WHITE);

    M5.Lcd.setCursor(0, 60);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Program Write Error\nNot connected to PC\n    ------------\n    %s\n    ------------\nPress the center button\nwhen you're ready\n\n\n           Push!",programName.c_str());
}

void Display::showQRReadWait(){
    M5.Lcd.fillScreen(WHITE);

    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("   QR Code Reader\n   Reading Wait...");
    M5.Lcd.drawJpgFile(SPIFFS, "/qr.jpg", 60, 70);
}

void Display::showQRReadDecision(String hirameQId){
    M5.Lcd.fillScreen(WHITE);

    M5.Lcd.setCursor(40, 60);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("QR Code Read Result\n");
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(20, 120);
    M5.Lcd.printf("ID:%s",hirameQId.c_str());
    M5.Lcd.setCursor(40, 200);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Retry   OK");
}

void Display::showTestResult(test_t result,String errorMessage,String hirameQId){
    if(result.testAllCheck == TEST_WAIT) {
        M5.Lcd.fillScreen(BLACK);
    } else if (result.testAllCheck == TEST_OK){
        M5.Lcd.fillScreen(TFT_BLUE);
    } else {
        M5.Lcd.fillScreen(TFT_RED);
    }

    M5.Lcd.setCursor(40, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Test Finish!\n");
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(20, 50);
    M5.Lcd.printf("ID:%s",hirameQId.c_str());
    M5.Lcd.setCursor(0, 100);
    if(errorMessage != "") {
        M5.Lcd.setTextSize(2);
        M5.Lcd.printf("Error:%s", errorMessage.c_str());
    }else{
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(40, 120);
        M5.Lcd.printf("Success!!");
    }
    M5.Lcd.setCursor(40, 200);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Open Checker");
}

bool Display::readCenterButton() {
    return M5.BtnB.read();
}
bool Display::readLeftButton() {
    return M5.BtnA.read();
}

void Display::begin() {
    M5.begin();
}

