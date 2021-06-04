//
// Created by wami on 2021/05/25.
//

#include "SwitchCheck.h"
#include <ESP32Servo.h>
#include "Arduino.h"
Servo resetServo;
Servo zeroServo;

void SwitchCheck::resetSwPush() {
    resetServo.attach(25);
    resetServo.write(70);
    delay(250);
    resetServo.write(0);
    delay(100);
    resetServo.detach();
}

void SwitchCheck::resetSw() {
    resetServo.attach(25);
    resetServo.write(0);
    delay(150);
    resetServo.detach();
}


void SwitchCheck::zeroSwPush() {
    zeroServo.attach(26);
    zeroServo.write(70);
    delay(250);
    zeroServo.write(0);
    delay(100);
    zeroServo.detach();
}