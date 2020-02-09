#include <Servo.h>  

#define MIN_CLAW_STEP 23
#define MAX_CLAW_STEP 50
#define CLAW_DELAY 5
#define CLAW_SPEED_DELAY 3

Servo claw;

void clawSetup() {
  claw.attach(SERVO_CLAW_PIN);
}

void clawOpen() {
  for(int i = MIN_CLAW_STEP; i < MAX_CLAW_STEP; i++) {
    claw.write(i);
    delay(CLAW_SPEED_DELAY);
  }
  delay(CLAW_DELAY);
}

void clawClose() {
  for(int i = MAX_CLAW_STEP; i >= MIN_CLAW_STEP; i--) {
    claw.write(i);
    delay(CLAW_SPEED_DELAY);
  }
  delay(CLAW_DELAY);
}

