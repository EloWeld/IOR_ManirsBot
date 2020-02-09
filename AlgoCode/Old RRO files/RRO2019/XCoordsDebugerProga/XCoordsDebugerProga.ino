#include <MultiStepper.h>
#include <AccelStepper.h>
  
#define STEP_HOR_DIR_PIN        55
#define STEP_HOR_STEP_PIN       54
#define STEP_HOR_ENABLE_PIN     38
#define STEP_HOR_END_PIN        3

#define STEP_VER_DIR_PIN        61
#define STEP_VER_STEP_PIN       60
#define STEP_VER_ENABLE_PIN     56
#define STEP_VER_END_PIN        14

AccelStepper step_hor(AccelStepper::DRIVER, STEP_HOR_STEP_PIN, STEP_HOR_DIR_PIN);
AccelStepper step_ver(AccelStepper::DRIVER, STEP_VER_STEP_PIN, STEP_VER_DIR_PIN);


void setup() {
  pinMode(STEP_HOR_ENABLE_PIN, OUTPUT);
  pinMode(STEP_VER_ENABLE_PIN, OUTPUT);
  pinMode(STEP_HOR_END_PIN, INPUT);
  pinMode(STEP_VER_END_PIN, INPUT);
  
  step_hor.setEnablePin(STEP_HOR_ENABLE_PIN);
  step_hor.setPinsInverted(false, false, true);
  step_hor.setMaxSpeed(22000);
  step_hor.setAcceleration(30000);
  step_hor.setSpeed(1000);

  step_ver.setEnablePin(STEP_VER_ENABLE_PIN);
  step_ver.setPinsInverted(false, false, true);
  step_ver.setMaxSpeed(22000);
  step_ver.setAcceleration(30000);
  step_ver.setSpeed(1000);

  Serial.begin(9600);
  Serial.print("hellow");
 // step_ver.enableOutputs();
  step_hor.enableOutputs();
  while(digitalRead(STEP_VER_END_PIN)) {
    step_ver.runSpeed();
    //Serial.println(step_ver.currentPosition());
  }
  step_ver.stop();
  step_ver.setCurrentPosition(2060);
  //Serial.println(step_ver.currentPosition());
  while (digitalRead(STEP_HOR_END_PIN)) {
    step_hor.runSpeed();
    //Serial.println(step_hor.currentPosition());
  }
  step_hor.stop();
  step_hor.setCurrentPosition(4150);
 // step_ver.enableOutputs();
 // step_hor.enableOutputs();
}

void loop() {
  byte a;
  int b = 0;
  if(Serial.available() > 0) {
    while(a != 'z') {
      if (Serial.available() > 0) {
        a = Serial.read();
        if(a != 'z') {
          b = b * 10 + a - 48;
          Serial.println(a);
        }
      }
    }
    Serial.println(b);
    if (b > 4150 || b < 0) {
      Serial.println("YOU'RE AN IDIOT!");
    } else 
      step_hor.runToNewPosition(b);
    step_hor.stop();
  }
}

