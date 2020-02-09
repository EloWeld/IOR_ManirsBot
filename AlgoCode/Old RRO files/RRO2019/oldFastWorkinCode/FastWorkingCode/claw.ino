#include <Servo.h>  

#define MIN_CLAW_STEP 155
#define MAX_CLAW_STEP 110
#define CLAW_DELAY 5000
#define CLAW_SPEED_DELAY 1000



Servo claw;

void clawSetup() {
  claw.attach(SERVO_CLAW_PIN);
 // clawClose();
 // delayMicroseconds(1000);
  //clawOpen();
}

void clawOpen() {
  //for(int i = MIN_CLAW_STEP; i > MAX_CLAW_STEP; i--) {
    claw.write(MAX_CLAW_STEP);
   // delayMicroseconds(CLAW_SPEED_DELAY);
//  }
  delayMicroseconds(CLAW_DELAY);
  claw_state = 1;
}

void clawClose() {
  //for(int i = MAX_CLAW_STEP; i < MIN_CLAW_STEP; i++) {
    claw.write(MIN_CLAW_STEP);
    //delayMicroseconds(CLAW_SPEED_DELAY);
//  }
  delayMicroseconds(CLAW_DELAY);
  claw_state = 0;
}

