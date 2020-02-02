#include <AccelStepper.h>
#include <Servo.h>  

#define START_BTN_PIN           19

#define STEP_HOR_DIR_PIN        55
#define STEP_HOR_STEP_PIN       54
#define STEP_HOR_ENABLE_PIN     38
#define STEP_HOR_END_PIN        3

#define STEP_VER_DIR_PIN        61
#define STEP_VER_STEP_PIN       60
#define STEP_VER_ENABLE_PIN     56
#define STEP_VER_END_PIN        14

#define SERVO_CLAW_PIN          4

#define CLAW_OPEN               76
#define CLAW_CLOSE              50   

AccelStepper step_hor(AccelStepper::DRIVER, STEP_HOR_STEP_PIN, STEP_HOR_DIR_PIN);
AccelStepper step_ver(AccelStepper::DRIVER, STEP_VER_STEP_PIN, STEP_VER_DIR_PIN);

Servo claw;

void zeroAxis() {
  while (digitalRead(STEP_VER_END_PIN)) {
    step_ver.runSpeed();
  }
  step_ver.stop();
  step_ver.setCurrentPosition(1960);
  
  while (digitalRead(STEP_HOR_END_PIN)) {
    step_hor.runSpeed();
  }
  step_hor.stop();
  step_hor.setCurrentPosition(4290);
}

int getPosHor(int x) {
  int pos = 0;
  switch (x) {
    case 0: pos = 0; break;
    case 1: pos = 858; break;//860
    case 2: pos = 1716; break;//1720
    case 3: pos = 2574; break;//2580
    case 4: pos = 3432; break;//3440
    case 5: pos = 4290; break;
  }
  return pos;
}

int getPosVer(int y) {
  int pos = 0;
  switch (y) {
    case 0: pos = 0; break;
    case 1: pos = 860; break;
    case 2: pos = 1660; break;
    case 3: pos = 1960; break;
  }
  return pos;
}

void stepMove(int hor, int ver, bool half) {
  int new_ver = getPosVer(ver) + (half * 200);
  if (step_ver.currentPosition() < new_ver) {
      step_ver.runToNewPosition(new_ver);
      step_hor.runToNewPosition(getPosHor(hor));
  } else {
      step_hor.runToNewPosition(getPosHor(hor));
      step_ver.runToNewPosition(new_ver);
  }
}

void clawOpen(bool cls) {
  if (cls) {
    claw.write(CLAW_OPEN);
  } else {
    claw.write(CLAW_CLOSE);
  }
  delay(200);
}

void actionMove(byte sort_pos, byte insert_pos, byte bufer_pos) {
  // Освобожадем позицию постановки
  stepMove(abs(5-insert_pos), 0, false);
  clawOpen(false);
  stepMove(abs(5-bufer_pos), 1, true);
  stepMove(bufer_pos, 1, false);
  clawOpen(true);

  // Переносим сортируемый кубик в особоженную позицию
  stepMove(sort_pos, 0, false);
  clawOpen(false);
  stepMove(insert_pos, 2, true);
  stepMove(insert_pos, 0, false);
  clawOpen(true);

  // Переносим буферный кубик
  stepMove(bufer_pos, 1, false);
  clawOpen(false);
  stepMove(sort_pos, 1, true);
  stepMove(sort_pos, 0, false);
  clawOpen(true);
}

void setup() {  
  pinMode(STEP_HOR_ENABLE_PIN, OUTPUT);
  pinMode(STEP_VER_ENABLE_PIN, OUTPUT);
  pinMode(STEP_HOR_END_PIN, INPUT);
  pinMode(STEP_VER_END_PIN, INPUT);
  pinMode(START_BTN_PIN, INPUT_PULLUP);

  step_hor.setEnablePin(STEP_HOR_ENABLE_PIN);
  step_hor.setPinsInverted(false, false, true);
  step_hor.setMaxSpeed(8000);
  step_hor.setAcceleration(8000);
  step_hor.setSpeed(1000);

  step_ver.setEnablePin(STEP_VER_ENABLE_PIN);
  step_ver.setPinsInverted(false, false, true);
  step_ver.setMaxSpeed(8000);
  step_ver.setAcceleration(8000);
  step_ver.setSpeed(1000);

  claw.attach(SERVO_CLAW_PIN);
  claw.write(CLAW_OPEN);

  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  clawOpen(true);

  step_ver.enableOutputs();
  step_hor.enableOutputs();
  
  zeroAxis();

  while (!digitalRead(START_BTN_PIN)) {
    
  }

  Serial2.print('!');
  
  byte start_pos[6];
  for (int i = 0; i < 6; i++) {
    start_pos[i] = 0;
  }
  byte end_pos[6];
  for (int i = 0; i < 6; i++) {
    end_pos[i] = 0;
  }

  int read_pos = 0;
  bool start = false;
  
  while (!start) {
    if (Serial2.available() > 0) {
      byte c = Serial2.read();
      if (read_pos < 6) {
        end_pos[read_pos] = c - 48;
      } else {
        start_pos[read_pos - 6] = c - 48;
      }
      read_pos = read_pos + 1;
      if (read_pos >= 12) {
        start = true;
      }
    }
  }
//  for (int i = 0; i < 6; i++)
//    Serial.print(start_pos[i]);
//    Serial.println();
//  
//  for (int i = 0; i < 6; i++)
//    Serial.print(end_pos[i]);
//  Serial.println();
//  Serial.println();

  for (int sort_pos = 5; sort_pos >= 0; sort_pos--) {
    Serial.println(sort_pos);
    if (start_pos[sort_pos] != end_pos[sort_pos]) {
      // Ищем куда поставить
      byte insert_pos = 0;
      for (int i = 5; i >= 0; i--) {
        if (end_pos[i] == start_pos[sort_pos]) {
          insert_pos = i;
        }
      }
      // Ищем промежуточную позицию
      byte bufer_pos = 0;
      for (int i = 5; i >= 0; i--) {
        if ((start_pos[i] != 5) && (start_pos[i] != 6) && (i != sort_pos) && (i != insert_pos)) {
          bufer_pos = i;
        }
      }

      actionMove(sort_pos, insert_pos, bufer_pos);

      byte swap = start_pos[insert_pos];
      start_pos[insert_pos] = start_pos[sort_pos];
      start_pos[sort_pos] = swap;
    }
      for (int i = 0; i < 6; i++)
        Serial.print(start_pos[i]);
        Serial.println();
  }

  stepMove(5, 3, false);

  step_ver.disableOutputs();
  step_hor.disableOutputs();

  while (true) {
  
  }
}
