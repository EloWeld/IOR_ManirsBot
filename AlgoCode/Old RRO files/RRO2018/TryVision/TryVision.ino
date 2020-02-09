//Checklist before start
//-вообще все шкивы
//-винты у клешни
//-верхние подшипники
//-нижний шкив мотора

#include <MultiStepper.h>
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

#define CLAW_OPEN               85
#define CLAW_CLOSE              55   
#define SERVO_ROTATE_SPEED      0

bool first_build = true;
byte start_pos[6];
byte end_pos[6];
int like_a_cube1 = 0;
int like_a_cube2 = 0;
int cube_pos_on_second_level = -1;
byte pos_is = 30;


AccelStepper step_hor(AccelStepper::DRIVER, STEP_HOR_STEP_PIN, STEP_HOR_DIR_PIN);
AccelStepper step_ver(AccelStepper::DRIVER, STEP_VER_STEP_PIN, STEP_VER_DIR_PIN);

Servo claw;

void zeroAxis() {
  //============================================================================/Переделать/============================================================================
  //Serial.print("Start ZeroAxis");
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
    case 4: pos = 840; break;//840
    case 3: pos = 1668; break;//1668
    case 2: pos = 2502; break;//2502
    case 1: pos = 3360; break;//3360
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
 //============================================================================/Переделать/============================================================================
 /* И так допустим, что с открытой клешней мы можем ехать на этаж ниже (прям через кубики, Why not?!),
    тогда half отвечает за подъем на 1 этаж и еще 350 (Cм. C:\Users\ArinAdmin\Desktop\TryVision\picture.png)*/
Ёvoid stepMove(int hor, int ver, bool half) {
  /*
    if(half) {  // Мы едем пустые за контейнером
      if (cube_pos_on_second_level == -1){ //Если на втором этаже пусто
      new_ver = getPosVer
      } else { //Если на втором этаже НЕ пусто
      
      }
    } else {    // Мы отвозим контейнером

    }
  */
  int new_ver = getPosVer(ver) + (half * 350);
  if (step_ver.currentPosition() < new_ver) {
      step_ver.runToNewPosition(new_ver);
      step_hor.runToNewPosition(getPosHor(hor));
  } else {
      step_hor.runToNewPosition(getPosHor(hor));
      step_ver.runToNewPosition(new_ver);
  }
}

void clawOpen(bool cls) {
  int pos = 0;
  if (cls) {
    for (pos = CLAW_CLOSE; pos <= CLAW_OPEN; pos += 1) { // goes from 180 degrees to 0 degrees
      claw.write(pos);              // tell servo to go to position in variable 'pos'
      delay(SERVO_ROTATE_SPEED);
    }
  } else {
    for (pos = CLAW_OPEN; pos >= CLAW_CLOSE; pos -= 1) { // goes from 180 degrees to 0 degrees
      claw.write(pos);              // tell servo to go to position in variable 'pos'
      delay(SERVO_ROTATE_SPEED);
    }
  }
  delay(200);
}


void actionMoveGraf(byte from_pos, byte to_pos) {
 //============================================================================/WHAT?!/============================================================================  
  if (from_pos > 5){
    cube_pos_on_second_level = -1;  
  }
  if (to_pos > 5){
    cube_pos_on_second_level = to_pos;  
  }
   //==================================================================/Мы едем пустые за контейнером/==================================================================
  @stepMove((from_pos % 10), whatLevel(pos_is, from_pos, cube_pos_on_second_level), false);
  stepMove((from_pos % 10), (from_pos / 10), false);
  clawOpen(false);
  //======================================================================/Мы отвозим контейнером/======================================================================
  stepMove((to_pos % 10), whatLevel(from_pos,to_pos, cube_pos_on_second_level), true);
  stepMove((to_pos % 10), (to_pos / 10), false);
  clawOpen(true);
  pos_is = to_pos;
}

Ъbyte whatLevel(byte from_pos, byte to_pos,byte cube_on_level2){
  // 0 -> 0
  if ((from_pos < 10)&&(to_pos < 10)){
      // через 0 этаж
      if(abs(from_pos - to_pos) == 1){
        return 0;
      }
      // через 1 этаж
      if((abs(from_pos - to_pos) != 1)&&
      ((((from_pos % 10) > (cube_on_level2 % 10))&&((to_pos % 10) > (cube_on_level2 % 10)))
      ||(((from_pos % 10) < (cube_on_level2 % 10))&&((to_pos % 10) < (cube_on_level2 % 10))))){
        return 1;
      } 
      // через 2 этаж
      if((abs(from_pos - to_pos) != 1)&&(cube_on_level2 != -1)&&
      ((((from_pos % 10) > (cube_on_level2 % 10))&&((to_pos % 10) < (cube_on_level2 % 10)))
      ||(((from_pos % 10) < (cube_on_level2 % 10))&&((to_pos % 10) > (cube_on_level2 % 10))))){
        return 2;
      }
  }
  // 1 -> 1
  if((from_pos > 5)&&(to_pos > 5)){
    return 1;
  }
  // 0 -> 1
  // 1 -> 0
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

  claw.attach(SERVO_CLAW_PIN);
  claw.write(CLAW_OPEN);

//  Serial.begin(9600);
  Serial2.begin(9600);
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
  while (!start) {
    if (Serial2.available() > 0) {
      byte c = Serial2.read();
      if (false) {
        vision = true;
//        Serial.println("I Vision");
        bool start_smart = false;
        while(!start_smart) {
          if (Serial2.available() > 0) {
            byte ch = Serial2.read();
            if (ch == '7') {
//              Serial.println("I finished");
              start_smart = true;
              break;
            }
            moves[moves_count] = (ch - 48) * 10;
            while(Serial2.available() <= 0) {    
            }
            byte ch2 = Serial2.read();
            moves[moves_count] += (ch2 - 48);
//            Serial.print(moves[moves_count]);
//            Serial.print(" ");
            moves_count++;
          }
        }
        start = true;
      }
    }
  }
  
  start = false; // Для рестарта

  clawOpen(true);

  first_build = false;
  step_ver.enableOutputs();
  step_hor.enableOutputs();
  
  zeroAxis();
  stepMove(0, 3, false);
//  Serial.println("I Vision, again");
  for (int i = 0; i < moves_count; i += 2){
      actionMoveGraf(moves[i], moves[i+1]);
  }

  stepMove(0, 3, false);
  zeroAxis();

}
