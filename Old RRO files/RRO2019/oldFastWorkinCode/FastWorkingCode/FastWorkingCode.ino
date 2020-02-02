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

bool first_build = true;
byte start_pos[6];
byte end_pos[6];
int like_a_cube1 = 0;
int like_a_cube2 = 0;

int old_pos_x = 0;
int old_pos_y = 2;
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
//  step_hor.setCurrentPosition(7485);
//  step_ver.setCurrentPosition(2965);
  step_hor.setCurrentPosition(1448);//new for 80 cogs
  step_ver.setCurrentPosition(588);//new for 80 cogs
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

int xCoordsArray[6][3] = {{5375, 6050, 7050}, {4325, 5000, 6000}, {3275, 3950, 4950}, {2225, 2900, 3900}, {1175, 1850, 2850},{125,800 , 1800}};
//int xCoordsArray[6][3] = {{5375, 6050, 7050}, {4325, 5000, 6000}, {3275, 3950, 4950}, {2225, 2900, 3900}, {1175, 1850, 2850},{125,800 , 1800}};
//int xCoordsArray[6][3] = {{4150, 4150, 4150}, {3380, 3380, 3380}, {2560, 2560, 2560},{1700, 1700, 1700},  {860, 860, 860}, {100, 100, 100}};
int getPosHor(int x,int y) {  
  return xCoordsArray[x][y];
}

int yCoordsArray[6][3] = {{5125, 4450, 3450}, {4075, 3400, 2400}, {3025, 2350, 1350}, {1975, 1300, 300}, {925, 250, -750}, {-125, -800, -1800}};
//int yCoordsArray[6][3] = {{5125, 4450, 3450}, {4075, 3400, 2400}, {3025, 2350, 1350}, {1975, 1300, 300}, {925, 250, -750}, {-125, -800, -1800}};
//int yCoordsArray[6][3] = {{10, 855, 1660}, {10, 855, 1660}, {10, 855, 1660}, {10, 855, 1660}, {10, 855, 1660}, {10, 855, 1660}};
int getPosVer(int x,int y) {  
  return yCoordsArray[x][y];
}

void stepMove(int hor, int ver, bool half) {

  if (ver > old_pos_y) {
    //============/ Ver /=============
    positions[0] = getPosVer(old_pos_x, ver) - (half * 500);
    positions[1] = getPosHor(old_pos_x, ver) + (half * 500);
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    //============/ Hor /=============
    positions[0] = getPosVer(hor, ver) - (half * 500);
    positions[1] = getPosHor(hor, ver) + (half * 500);
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
  } else {
    //============/ Hor /=============
    positions[0] = getPosVer(old_pos_x, old_pos_y) - (half * 500);
    positions[1] = getPosHor(old_pos_x, old_pos_y) + (half * 500);
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    positions[0] = getPosVer(hor, old_pos_y) - (half * 500);
    positions[1] = getPosHor(hor, old_pos_y) + (half * 500);
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
    //============/ Ver /=============
    positions[0] = getPosVer(hor, ver) - (half * 500);
    positions[1] = getPosHor(hor, ver) + (half * 500);
    steppers.moveTo(positions);//y, x
    steppers.runSpeedToPosition();
  }
  old_pos_x = hor;
  old_pos_y = ver;
}

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

  step_hor.setEnablePin(STEP_HOR_ENABLE_PIN);
  step_hor.setPinsInverted(false, false, true);
  step_hor.setMaxSpeed(3500);
  step_hor.setAcceleration(100000);
  step_hor.setSpeed(3500);

  step_ver.setEnablePin(STEP_VER_ENABLE_PIN);
  step_ver.setPinsInverted(false, false, true);
  step_ver.setMaxSpeed(3500);
  step_ver.setAcceleration(100000);
  step_ver.setSpeed(3500);

  //Serial.begin(9600);
  Serial2.begin(9600);
  steppers.addStepper(step_ver);
  steppers.addStepper(step_hor);
  zeroAxis();
}


void loop() {

  for (int i = 0; i < 6; i++) {
    start_pos[i] = 0;
  }

  for (int i = 0; i < 6; i++) {
    end_pos[i] = 0;
  }
  byte moves[30];
  for (int i = 0; i < 30; i++) {
    moves[i] = 0;
  }

  int moves_count = 0;
  int read_pos = 0;
  bool vision = false;
  
  bool start = false;
  while (!start) {
    if (Serial2.available() > 0) {
      byte c = Serial2.read();
      //Serial.println(c);
      if (c == '9') {
        vision = true;
        //Serial.println("I Vision");
        bool start_smart = false;
        while(!start_smart) {
          if (Serial2.available() > 0) {
            //clawClose();
            //delay(1500);
            byte ch = Serial2.read();
            if (ch == '7') {
              //Serial.println("I finished");
              start_smart = true;
              break;
            }
            moves[moves_count] = (ch - 48) * 10;
            while(Serial2.available() <= 0) {
            }
            byte ch2 = Serial2.read();
            moves[moves_count] += (ch2 - 48);
            //Serial.print(moves[moves_count]);
            //Serial.print(" ");
            moves_count++;
          }
        }
        start = true;
      } else if (c == '8'){
        vision = false;
        //Serial.println("StUpId");
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
            //Serial.print(read_pos);
            if (read_pos == 12) {
              //Serial.println("WereFound");
              psevdoCube();
              //Serial.println("PsevdoCubesWereFound");
              start_stupid = true;
            }
          }
        }
        //Serial.println("I quit out while");
        start = true;
      }
    }
  }
  
  start = false; // Для рестарта
  first_build = false;
  
  step_ver.enableOutputs();
  step_hor.enableOutputs();
  
  zeroAxis();
  if (STUPEED){
    //Serial.println("I started");
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
      }
    }
  }else if (vision) {
    //Serial.println("I Vision, again");
    
    for (int i = 0; i < moves_count; i += 2){
       actionMoveGraf(moves[i], moves[i+1]);
    }
    zeroAxis();
    
  } 
  //Serial.write(8);
}
