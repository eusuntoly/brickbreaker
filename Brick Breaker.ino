#include <LedControl.h>
#include <LiquidCrystal.h>
#define CONTRAST_PIN 9
LedControl ledControl = LedControl(12, 11, 10, 1);
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219
const int rs = 13, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2, leftButton = 8, rightButton = 7;
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);
int inGame = 0, leftButtonCurrentState, leftButtonLastState, rightButtonCurrentState, rightButtonLastState;
int lifes = 0, blocksNumber = 0, gameTable[10][10], rowBall = 0, rowMove = 0, colBall = 0, colMove = 0, currentTime = 0, lastTime = 0;
void setup() {
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  ledControl.shutdown(0, false);// turn off power saving, enables display
  ledControl.setIntensity(0, 8);// sets brightness (0~15 possible values)
  ledControl.clearDisplay(0);// clear screen
  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite(CONTRAST_PIN, 300);
  LCD.clear();
  LCD.begin(16, 2);
  LCD.print("Press any button");
  LCD.setCursor(0, 1);
  LCD.print("to begin");
}
void gamePlay() {
  if (gameTable[rowBall][colBall - 1] == 1)
    colMove = 1;
  if (gameTable[rowBall][colBall + 1] == 2)
    colMove = -1;
  if (gameTable[rowBall - 1][colBall] == 3)
    rowMove = 1;
  if (gameTable[rowBall + 1][colBall] == 5) {
    rowMove = -1;
    colMove = -1;
  }
  if (rowMove == -1) {
    if (gameTable[rowBall - 1][colBall] == -1 && gameTable[rowBall][colBall + 1] == -1 && colMove == 1) {
      rowMove = 1;
      colMove = -1;
      gameTable[rowBall - 1][colBall] = 0;
      gameTable[rowBall][colBall + 1] = 0;
      blocksNumber -= 2;
    }
    else
      if (gameTable[rowBall - 1][colBall] == -1 && gameTable[rowBall][colBall - 1] == -1 && colMove == -1) {
        rowMove = 1;
        colMove = 1;
        gameTable[rowBall - 1][colBall] = 0;
        gameTable[rowBall][colBall - 1] = 0;
        blocksNumber -= 2;
      }
      else
        if (gameTable[rowBall - 1][colBall] == -1) {
          rowMove = 1;
          gameTable[rowBall - 1][colBall] = 0;
          blocksNumber--;
        }
        else
          if (gameTable[rowBall - 1][colBall - 1] == -1 && colMove == -1) {
            rowMove = 1;
            colMove = 1;
            gameTable[rowBall - 1][colBall - 1] = 0;
            blocksNumber--;
          }
          else
            if (gameTable[rowBall - 1][colBall + 1] == -1 && colMove == 1) {
              rowMove = 1;
              colMove = -1;
              gameTable[rowBall - 1][colBall + 1] = 0;
              blocksNumber--;
            }
  }
  if (rowMove == 1) {
    if (gameTable[rowBall + 1][colBall] == -1 && gameTable[rowBall][colBall + 1] == -1 && colMove == 1) {
      rowMove = 1;
      colMove = -1;
      gameTable[rowBall + 1][colBall] = 0;
      gameTable[rowBall][colBall + 1] = 0;
      blocksNumber -= 2;
    }
    else
      if (gameTable[rowBall + 1][colBall] == -1 && gameTable[rowBall][colBall - 1] == -1 && colMove == -1) {
        rowMove = 1;
        colMove = 1;
        gameTable[rowBall + 1][colBall] = 0;
        gameTable[rowBall][colBall - 1] = 0;
        blocksNumber -= 2;
      }
      else
        if (gameTable[rowBall + 1][colBall] == -1) {
          rowMove = 1;
          gameTable[rowBall + 1][colBall] = 0;
          blocksNumber--;
        }
        else
          if (gameTable[rowBall + 1][colBall - 1] == -1 && colMove == -1) {
            rowMove = 1;
            colMove = 1;
            gameTable[rowBall + 1][colBall - 1] = 0;
            blocksNumber--;
          }
          else
            if (gameTable[rowBall + 1][colBall + 1] == -1 && colMove == 1) {
              rowMove = 1;
              colMove = -1;
              gameTable[rowBall + 1][colBall + 1] = 0;
              blocksNumber--;
            }
  }
  if (gameTable[rowBall + 1][colBall] == 5) {
    rowMove = -1;
    colMove = -1;
  }
  if (gameTable[rowBall + 1][colBall] == 6) {
    rowMove = -1;
  }
  if (gameTable[rowBall + 1][colBall] == 7) {
    rowMove = -1;
    colMove = 1;
  }
  if (gameTable[rowBall][colBall - 1] == 1)
    colMove = 1;
  if (gameTable[rowBall][colBall + 1] == 2)
    colMove = -1;
  if (gameTable[rowBall - 1][colBall] == 3)
    rowMove = 1;
  if (gameTable[rowBall + 1][colBall] == 4) {
    lifes = lifes - 1;
    buildPaddle();
  }
  else{
    for (int row = 1; row < 3; ++row)
      for (int col = 1; col < 9; ++col)
        if (gameTable[row][col])
          ledControl.setLed(0, row - 1, col - 1, true);
        else
          ledControl.setLed(0, row - 1, col - 1, false);
    ledControl.setLed(0, rowBall - 1, colBall - 1, false);
    rowBall = rowBall + rowMove;
    colBall = colBall + colMove;
    ledControl.setLed(0, rowBall - 1, colBall - 1, true);
  }
}
void buildGame() {
  lifes = 3;
  blocksNumber = 16;
  for (int row = 0; row <= 9; ++row)
    for (int col = 0; col <= 9; ++col)
      gameTable[row][col] = 0; //set matrix on 0
  for (int i = 0; i <= 9; ++i) {//boarding
    gameTable[i][0] = 1; //left border
    gameTable[0][i] = 3; //top border
    gameTable[i][9] = 2; //right border
    gameTable[9][i] = 4; //bottom border
  }
  for (int row = 0; row < 2; ++row)
    for(int col = 0; col < 8; ++col) {
      ledControl.setLed(0, row, col, true);
      gameTable[row + 1][col + 1] = -1;
    }
  for (int row = 1; row < 9; ++row)
    for (int col = 1; col < 9; ++col)
      if (gameTable[row][col])
        ledControl.setLed(0, row - 1, col - 1, true);
      else
        ledControl.setLed(0, row - 1, col - 1, false);
}
void buildPaddle() {
  rowBall = 7;
  colBall = 5;
  rowMove = -1;
  colMove = 0;
  ledControl.setLed(0, 6, 4, true);
  for(int col = 1; col <= 8; ++col)
    gameTable[8][col] = 0;
  gameTable[8][4] = 5;
  gameTable[8][5] = 6;
  gameTable[8][6] = 7;
  for(int col = 1; col <= 8; ++col)
    if (gameTable[8][col])
      ledControl.setLed(0, 7, col - 1, true);
    else
      ledControl.setLed(0, 7, col - 1, false);
}
void loop() {
  leftButtonCurrentState = digitalRead(leftButton);
  if (leftButtonCurrentState != leftButtonLastState && leftButtonCurrentState == HIGH) {
    moveLeft();
  }
  leftButtonLastState = leftButtonCurrentState;
  rightButtonCurrentState = digitalRead(rightButton);
  if(rightButtonCurrentState != rightButtonLastState && rightButtonCurrentState == HIGH){
    moveRight();
  }
  rightButtonLastState = rightButtonCurrentState;
  currentTime = millis();
  if (currentTime - lastTime >= 250) {
    if (inGame == 1) {
      LCD.setCursor(0, 0);
      LCD.clear();
      LCD.print("Score:");
      LCD.setCursor(0, 1);
      LCD.print("Lifes:");
      inGame = 2;
      buildGame();
      buildPaddle();
    }
    if (inGame == 2 && lifes && blocksNumber){
      gamePlay();
      LCD.setCursor(7, 0);
      LCD.print(16 - blocksNumber);
      LCD.setCursor(7, 1);
      LCD.print(lifes);
    }
    if (inGame == 2 && !lifes && blocksNumber) {
      inGame = 0;
      LCD.clear();
      LCD.print("You lost!");
      LCD.setCursor(0, 1);
      LCD.print("Score:");
      LCD.setCursor(7, 1);
      LCD.print(16 - blocksNumber);
    }
    if (inGame == 2 && lifes && !blocksNumber) {
      inGame = 0;
      LCD.clear();
      LCD.print("You won! Press");
      LCD.setCursor(0, 1);
      LCD.print("to start again");
    }
    lastTime = millis();
  }
}
void movePaddleLeft() {
  for (int col = 1; col < 8; ++col)
    gameTable[8][col] = gameTable[8][col + 1];
  gameTable[8][8] = 0;
  for (int col = 1; col <= 8; ++col)
    if (gameTable[8][col])
      ledControl.setLed(0, 7, col - 1, true);
    else
      ledControl.setLed(0, 7, col - 1, false);
}
void moveLeft() {
 if (!inGame)
  inGame = 1;
 else
  if (inGame && gameTable[8][1] == 0)
    movePaddleLeft();
}
void movePaddleRight(){
  for (int col = 8; col > 0; --col) 
    gameTable[8][col] = gameTable[8][col - 1];
  gameTable[8][1] = 0;
  for (int col = 1; col <= 8; ++col)
    if (gameTable[8][col])
      ledControl.setLed(0, 7, col - 1, true);
    else
      ledControl.setLed(0, 7, col - 1, false);
}
void moveRight() {
 if (!inGame)
  inGame = 1;
 else
  if (inGame && gameTable[8][8] == 0)
    movePaddleRight();
}

