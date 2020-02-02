#include <MultiStepper.h>
#include <AccelStepper.h>
#include <Servo.h>

#define START_BTN_PIN           19

#define STUPEED 0
  
#define STEP_HOR_DIR_PIN        55
#define STEP_HOR_STEP_PIN       54
#define STEP_HOR_ENABLE_PIN     38
#define STEP_HOR_END_PIN        3

#define STEP_VER_DIR_PIN        61
#define STEP_VER_STEP_PIN       60
#define STEP_VER_ENABLE_PIN     56
#define STEP_VER_END_PIN        14

#define SERVO_CLAW_PIN          4

#define SERVO_ROTATE_SPEED      0


#define MIN_CLAW_STEP 85
#define MAX_CLAW_STEP 40
#define CLAW_DELAY 5
#define CLAW_SPEED_DELAY 3

bool claw_state = 1;
long positions[2]; 
AccelStepper step_hor(AccelStepper::DRIVER, STEP_HOR_STEP_PIN, STEP_HOR_DIR_PIN);
AccelStepper step_ver(AccelStepper::DRIVER, STEP_VER_STEP_PIN, STEP_VER_DIR_PIN);
MultiStepper steppers;


void zeroAxis() {
  Serial.print("Start ZeroAxis");
  step_ver.setCurrentPosition(0);
  step_hor.setCurrentPosition(0);
  step_ver.enableOutputs();
  step_hor.enableOutputs();
  while ((digitalRead(STEP_HOR_END_PIN)) && (digitalRead(STEP_VER_END_PIN))) {
    positions[0] = 0;
    positions[1] = 2;
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    step_hor.setCurrentPosition(0);
  }
  step_hor.stop();
  while (digitalRead(STEP_HOR_END_PIN)) {
    positions[0] = 2;
    positions[1] = 2;
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    step_hor.setCurrentPosition(0);
    step_ver.setCurrentPosition(0);
  }
  step_hor.stop();
  step_ver.stop();
  while (digitalRead(STEP_VER_END_PIN)) {
    positions[0] = 0;
    positions[1] = 2;
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    step_hor.setCurrentPosition(0);
    step_ver.setCurrentPosition(2);
  }
  step_hor.stop();
  step_ver.stop();
  step_hor.setCurrentPosition(2925);
  step_ver.setCurrentPosition(1205);
}

Servo claw;

void clawOpen() {
  for(int i = MIN_CLAW_STEP; i >= MAX_CLAW_STEP; i--) {
    claw.write(i);
    delay(CLAW_SPEED_DELAY);
  }
  delay(CLAW_DELAY);
  claw_state = 1;
}

void clawClose() {
  for(int i = MAX_CLAW_STEP; i <= MIN_CLAW_STEP; i++) {
    claw.write(i);
    delay(CLAW_SPEED_DELAY);
  }
  delay(CLAW_DELAY);
  claw_state = 0;
}

void clawSetup() {
  claw.attach(SERVO_CLAW_PIN);
  clawClose();
  delay(1500);
  clawOpen();
}

void setup() {
  pinMode(STEP_HOR_ENABLE_PIN, OUTPUT);
  pinMode(STEP_VER_ENABLE_PIN, OUTPUT);
  pinMode(STEP_HOR_END_PIN, INPUT);
  pinMode(STEP_VER_END_PIN, INPUT);
  //pinMode(START_BTN_PIN, INPUT_PULLUP);

  //clawSetup();
  //clawOpen();

  step_hor.setEnablePin(STEP_HOR_ENABLE_PIN);
  step_hor.setPinsInverted(false, false, true);
  step_hor.setMaxSpeed(600);
  step_hor.setAcceleration(30000);
  step_hor.setSpeed(600);

  step_ver.setEnablePin(STEP_VER_ENABLE_PIN);
  step_ver.setPinsInverted(false, false, true);
  step_ver.setMaxSpeed(600);
  step_ver.setAcceleration(30000);
  step_ver.setSpeed(600);

  Serial.begin(9600);
  //Serial2.begin(9600);
  steppers.addStepper(step_ver);
  steppers.addStepper(step_hor);
  //clawSetup();
  zeroAxis();
  Serial.println("home");
//  int tmp1 = step_ver.currentPosition;
//  int tmp1 = step_ver.currentPosition;
  Serial.println(step_ver.currentPosition());
  Serial.println(step_hor.currentPosition());
  positions[1] = 2930;
  positions[0] = 1220;
  steppers.moveTo(positions);//y, x
  steppers.runSpeedToPosition();/**/
  /*positions[0] = 800;
  positions[1] = 1000;
  steppers.moveTo(positions);//y, x
  steppers.runSpeedToPosition();*/
  
  
}
int a = 0;
int b = 0;
void loop() {
//  Serial.println(digitalRead(STEP_VER_END_PIN));
//  Serial.println(digitalRead(STEP_HOR_END_PIN));
 // positions[0] = 1000;
 // positions[1] = 1000;
//  steppers.moveTo(positions);//y, x
//  steppers.runSpeedToPosition();
  /*

  //while(1){}
  //step_ver.stop();
  *//*positions[0] = 800;
    positions[1] = 800;
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    positions[0] = 800;
    positions[1] = 1200;
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();/**/
 /*steppers.runToNewPosition(1100);
  step_ver.runToNewPosition(1100);
  step_hor.runToNewPosition(800);
  step_ver.runToNewPosition(800);
  if (Serial.available() > 0) {
    Serial.println("available");
    byte c = Serial.read();
    if (c == 'h') {
      if ((a >= 10)&&(a <= 415)){
        step_hor.runToNewPosition(a * 10);
        a = 0;
      } else {
        a = 0;
      }
    } else if (c == 'v') {
      if ((a >= 0)&&(a <= 206)){
        step_ver.runToNewPosition(a * 10);
        a = 0;
      } else {
        Serial.println("Erorr!"); 
        a = 0;
      }
    } else if (c == 'c') {
      if ((claw_state)&&(a == 0)){
        clawClose();
        a = 0;
      } else if ((!claw_state)&&(a == 1)){
        clawOpen();
        a = 0;
      } else {
        Serial.println("Erorr!"); 
        a = 0;
      }
    } else {
      a = a*10 + (c - 48);
      Serial.println(a);
    }
  } else {
    //Serial.println("trabel");/
  }*/
      
}



