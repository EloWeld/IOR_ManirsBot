#include <Servo.h>  

#define MIN_CLAW_STEP 130
#define MAX_CLAW_STEP 85
#define CLAW_DELAY 100
#define CLAW_SPEED_DELAY 4000



Servo claw;

void clawSetup() {
  claw.attach(SERVO_CLAW_PIN);
  //clawClose();
  //delay(10);//Microseconds
  clawOpen();
  while(0){
  delay(1000);
  clawClose();
  delay(1000);//Microseconds
  clawOpen();}
}

void clawOpen() {
  //for(int i = MIN_CLAW_STEP; i > MAX_CLAW_STEP; i--) {
    claw.write(MAX_CLAW_STEP);
   // delayMicroseconds(CLAW_SPEED_DELAY);
  //}
  delay(CLAW_DELAY);
  claw_state = 1;
}

void clawClose() {
  //for(int i = MAX_CLAW_STEP; i < MIN_CLAW_STEP; i++) {
    claw.write(MIN_CLAW_STEP);
  //  delayMicroseconds(CLAW_SPEED_DELAY);
  //}
  delay(CLAW_DELAY);
  claw_state = 0;
}

