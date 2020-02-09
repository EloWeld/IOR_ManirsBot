#define GRABED_CUBE -1

//Checklist before start
//-вообще все шкивы
//-винты у клешни
//-верхние подшипники
//-нижний шкив мотора

#include <MultiStepper.h>
#include <AccelStepper.h>
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

#define CLAW_OPEN               85
#define CLAW_CLOSE              55   
#define SERVO_ROTATE_SPEED      0

bool first_build = true;
byte start_pos[6];
byte end_pos[6];
int like_a_cube1 = 0;
int like_a_cube2 = 0;
AccelStepper step_hor(AccelStepper::DRIVER, STEP_HOR_STEP_PIN, STEP_HOR_DIR_PIN);
AccelStepper step_ver(AccelStepper::DRIVER, STEP_VER_STEP_PIN, STEP_VER_DIR_PIN);

//Servo claw;

void zeroAxis() {
  //Serial.print("Start ZeroAxis");
  step_ver.enableOutputs();
  step_hor.enableOutputs();
  while (digitalRead(STEP_VER_END_PIN)) {
    step_ver.runSpeed();
  }
  step_ver.stop();
  step_ver.setCurrentPosition(2060);
  
  while (digitalRead(STEP_HOR_END_PIN)) {
    step_hor.runSpeed();
  }
  step_hor.stop();
  step_hor.setCurrentPosition(4150);
}

void psevdoCube(){
  for (int i = 0; i < 6; i+=1){
    if (start_pos[i] == 5){
      like_a_cube1 = end_pos[i];  
    }
    if (start_pos[i] == 6){
      like_a_cube2 = end_pos[i];
    }
  }
}

int getPosHor(int x) {
  int pos = 0;
  switch (x) {
    case 5: pos = 0; break;//0
    case 4: pos = 855; break;//840
    case 3: pos = 1650; break;//1668
    case 2: pos = 2560; break;//2502
    case 1: pos = 3350; break;//3360
    case 0: pos = 4150; break;//4170
  }
  return pos;
}

int getPosVer(int y) {
  int pos = 0;
  switch (y) {
    case 0: pos = 0; break;//0
    case 1: pos = 860; break;//860
    case 2: pos = 1660; break;//1660
    case 3: pos = 2061; break;//1960
  }
  return pos;
}

void stepMove(int hor, int ver, bool half) {
  int new_ver = getPosVer(ver) + (half * 350);
  if (step_ver.currentPosition() < new_ver) {
      step_ver.runToNewPosition(new_ver);
      step_hor.runToNewPosition(getPosHor(hor));
  } else {
      step_hor.runToNewPosition(getPosHor(hor));
      step_ver.runToNewPosition(new_ver);
  }
}

//void clawOpen(bool cls) {
//  int pos = 0;
//  if (cls) {
//    for (pos = CLAW_CLOSE; pos <= CLAW_OPEN; pos += 1) { // goes from 180 degrees to 0 degrees
//      claw.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(SERVO_ROTATE_SPEED);
//    }
//  } else {
//    for (pos = CLAW_OPEN; pos >= CLAW_CLOSE; pos -= 1) { // goes from 180 degrees to 0 degrees
//      claw.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(SERVO_ROTATE_SPEED);
//    }
//  }
//  delay(200);
//}

void actionMove(byte sort_pos, byte insert_pos, byte bufer_pos) {
  //Serial.write(7);
  //Serial.write(insert_pos);
  //Serial.write(bufer_pos);
  // Освобожадем позицию постановки
  stepMove(abs(5-insert_pos), 0, false);
  clawClose();
  stepMove(abs(5-bufer_pos), 1, true);
  stepMove(abs(5-bufer_pos), 1, false);
  clawOpen();

  // Переносим сортируемый кубик в особоженную позицию
  stepMove(abs(5-sort_pos), 0, false);
  clawClose();
  if (abs(abs(5-sort_pos) - abs(5-insert_pos)) == 1)
    stepMove(insert_pos, 0, true);
  else if ((abs(5-bufer_pos) > abs(5-insert_pos) && abs(5-bufer_pos) < abs(5-sort_pos))||(abs(5-bufer_pos) > abs(5-sort_pos) && abs(5-bufer_pos) < abs(5-insert_pos)))
    stepMove(abs(5-insert_pos), 2, true);
  else
    stepMove(abs(5-insert_pos), 1, true);
      
  
  stepMove(abs(5-insert_pos), 0, false);
  clawOpen();

  // Переносим буферный кубик
  stepMove(abs(5-bufer_pos), 1, false);
  clawClose();
  stepMove(abs(5-sort_pos), 1, true);
  stepMove(abs(5-sort_pos), 0, false);
  clawOpen();
}



int cube_pos_on_second_level = -1;
byte pos_is = 30;

void actionMoveGraf(byte from_pos, byte to_pos) {
  
  if (from_pos > 5){
    cube_pos_on_second_level = -1;  
  }
  if (to_pos > 5){
    cube_pos_on_second_level = to_pos;  
  }
  stepMove((from_pos % 10), whatLevel(pos_is, from_pos, cube_pos_on_second_level), false);
  stepMove((from_pos % 10), (from_pos / 10), false);
  clawClose();
  stepMove((to_pos % 10), whatLevel(from_pos,to_pos, cube_pos_on_second_level), true);
  stepMove((to_pos % 10), (to_pos / 10), false);
  clawOpen();
  pos_is = to_pos;
}

byte whatLevel(byte from_pos, byte to_pos,byte cube_on_level2){
  // 0 -> 0
  if ((from_pos < 10)&&(to_pos < 10)){
      // через 0
      if(abs(from_pos - to_pos) == 1){
        return 0;
      }
      // через 1
      if((abs(from_pos - to_pos) != 1)&&
      ((((from_pos % 10) > (cube_on_level2 % 10))&&((to_pos % 10) > (cube_on_level2 % 10)))
      ||(((from_pos % 10) < (cube_on_level2 % 10))&&((to_pos % 10) < (cube_on_level2 % 10))))){
        return 1;
      } 
      // через 2 
      if((abs(from_pos - to_pos) != 1)&&(cube_on_level2 != -1)&&
      ((((from_pos % 10) > (cube_on_level2 % 10))&&((to_pos % 10) < (cube_on_level2 % 10)))
      ||(((from_pos % 10) < (cube_on_level2 % 10))&&((to_pos % 10) > (cube_on_level2 % 10))))){
        return 2;
      }
  }
  if((from_pos > 5)&&(to_pos > 5)){
    return 1;
  }
  
  if(((from_pos > 5)&&(to_pos < 10))||((from_pos < 10)&&(to_pos > 5))){
    return 1;
  }
}


void setup() {  
  pinMode(STEP_HOR_ENABLE_PIN, OUTPUT);
  pinMode(STEP_VER_ENABLE_PIN, OUTPUT);
  pinMode(STEP_HOR_END_PIN, INPUT);
  pinMode(STEP_VER_END_PIN, INPUT);
  pinMode(START_BTN_PIN, INPUT_PULLUP);

  clawSetup();
  clawOpen();
  while(0){
  clawClose();
  delay(1000);
  clawOpen();
  delay(1000);
  }

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
  Serial2.begin(9600);
  zeroAxis();
}

void GrabCube(){
  if(GRABED_CUBE != -1){
    
  }
}

void loop() {

  for (int i = 0; i < 6; i++) {
    start_pos[i] = 0;
  }

  for (int i = 0; i < 6; i++) {
    end_pos[i] = 0;
  }
  byte moves[30];
  for (int i = 0; i < 6; i++) {
    moves[i] = 0;
  }

  int moves_count = 0;
  int read_pos = 0;
  bool vision = false;
  
  bool start = false;
  //Serial.write(9);
  while (!start) {
    if (Serial2.available() > 0) {
      byte c = Serial2.read();
      if (c = 9) {
        vision = true;
        Serial.println("I Vision");
        bool start_smart = false;
        while(!start_smart) {
          if (Serial2.available() > 0) {
            byte ch = Serial2.read();
            if (ch == '7') {
              Serial.println("I finished");
              start_smart = true;
              break;
            }
            moves[moves_count] = (ch - 48) * 10;
            while(Serial2.available() <= 0) {
              
            }
            byte ch2 = Serial2.read();
            moves[moves_count] += (ch2 - 48);
            Serial.print(moves[moves_count]);
            Serial.print(" ");
            moves_count++;
          }
        }
        start = true;
      } else if (c == '8'){
        vision = false;
        Serial.println("StUpId");
        bool start_stupid = false;
        while (!start_stupid) {
          if (Serial2.available() > 0) {
            byte chor = Serial2.read();
            if (read_pos < 7) {
              end_pos[read_pos] = chor - 48;
              //Serial.print(end_pos[read_pos]);
            } else {
              start_pos[read_pos - 6] = chor - 48;
              //Serial.print(start_pos[read_pos]);
            }
            read_pos = read_pos + 1;
            Serial.print(read_pos);
            if (read_pos == 12) {
              Serial.println("WereFound");
              psevdoCube();
              Serial.println("PsevdoCubesWereFound");
              start_stupid = true;
            }
          }
        }
        Serial.println("I quit out while");
        start = true;
      }
    }
  }
  
  start = false; // Для рестарта

  clawOpen();

  first_build = false;
  step_ver.enableOutputs();
  step_hor.enableOutputs();
  
  zeroAxis();
  stepMove(0, 3, false);
  

  if (STUPEED){
    Serial.println("I started");
  for (int sort_pos = 5; sort_pos >= 0; sort_pos--) {
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
      for (int i = 11; i >= 0; i--) {
        int tmp = insert_pos + pow(-1, i) * ((i-1) / 2 + 1);
        if (((start_pos[sort_pos] == 6)||(start_pos[sort_pos] == 5))&&((start_pos[tmp] != like_a_cube1)&&(start_pos[tmp] != like_a_cube1))){
          bufer_pos = tmp;
        }
        if (((start_pos[sort_pos] != 6)&&(start_pos[sort_pos] != 5))
        &&((start_pos[tmp] != 5) && (start_pos[tmp] != 6) && (tmp != sort_pos) && (tmp != insert_pos) && (tmp > -1) && (tmp < 6))) {
          bufer_pos = tmp;
        }
      }

      actionMove(sort_pos, insert_pos, bufer_pos);

      byte swap = start_pos[insert_pos];
      start_pos[insert_pos] = start_pos[sort_pos];
      start_pos[sort_pos] = swap;
      //Serial.write(9);
      /*for (int i = 0; i < 6; i++){
        Serial.write(end_pos[i]);
      }
      for (int i = 0; i < 6; i++){
        Serial.write(start_pos[i]);
      }*/
    }
  }
  }else if (vision) {
    Serial.println("I Vision, again");
    for (int i = 0; i < moves_count; i += 2){
      actionMoveGraf(moves[i], moves[i+1]);
    }
  }
  GrabCube();
  zeroAxis();

  //Serial.write(8);
}
