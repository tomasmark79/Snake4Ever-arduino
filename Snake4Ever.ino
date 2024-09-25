//
//    ____________________________
//   /                           /|
//  /     Licence Unlicence     / |
// /___________________________/  |
// |    Arduino Snake4Ever     |  |
// |    Tomas Mark    2024     |  |
// |  tomas@digitalspace.name  |  /
// | https://digitalspace.name | /
// |___________________________|/
//
// Task for you visitor!
// You have to add some fruit and snake grow up to the code. This is now your Job. :-)
//
// Thanks my lovely girlfriend for her support!
// Three very funy days during doing this project. 🤓
//

#define SpecialElementsEnabled
#define InitialSnakeLength 50

// Audio
#include "Playtune.h"

// Midi music
#include "midi.hpp"

// Oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Playtune pt;

// Reset
void (*resetArduino)(void) = 0;  // declare reset function @ address 0

// Freemem
extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;
int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
};

// Snake
#define SNAKE_HEAD 0
#define ON_BTN_NONE (uint8_t)0
#define ON_BTN_LEFT (uint8_t)1
#define ON_BTN_RIGHT (uint8_t)2
#define ON_BTN_UP (uint8_t)3
#define ON_BTN_DOWN (uint8_t)4
#define CONFLICT_SELF (uint8_t)1
#define CONFLICT_WALL (uint8_t)2
#define CONFLICT_NONE (uint8_t)0

static const bool isMusicOn = true;
static volatile bool isDemo = true;

// constants won't change. They're used here to set pin numbers:
const int8_t buttonLeftPin = 2;   // the number of the pushbutton pin
const int8_t buttonRightPin = 3;  // the number of the pushbutton pin
const int8_t buttonUpPin = 4;     // the number of the pushbutton pin
const int8_t buttonDownPin = 5;   // the number of the pushbutton pin

// button states
int8_t buttonLeftState = 0;
int8_t buttonRightState = 0;
int8_t buttonUpState = 0;
int8_t buttonDownState = 0;

bool onAnyButtonDown() {
  buttonLeftState = digitalRead(buttonLeftPin);
  buttonRightState = digitalRead(buttonRightPin);
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  if ((buttonLeftState == HIGH) || (buttonRightState == HIGH) || (buttonUpState == HIGH) || (buttonDownState == HIGH))
    return true;
  return false;
}

// *********************************** O B J E C T  D E B U G
class Debug {
public:

  Debug &log(char *msg, bool isWrapped = false) {
    Serial.print(msg);
    if (isWrapped)
      Serial.print('\n');
    return *this;
  }

  Debug &log(uint32_t number, bool isWrapped = false) {
    char numberArray[20];
    Serial.print(itoa(number, numberArray, 10));
    if (isWrapped)
      Serial.print('\n');
    return *this;
  }
};

Debug dbg;

// *********************************** O B J E C T  S C E N E
class Scene {
public:
  Scene(int (*fn)(void), int durationInMs)
    : m_Fn(fn), m_DurationInMs(durationInMs) {
    this->showScene();
  };

  ~Scene() {
    display.clearDisplay();
    display.display();
  };

  Scene &setFunction(int (*fn)(void)) {
    this->m_Fn = fn;
    return *this;
  }

  Scene &setDuration(int durationInMs) {
    m_DurationInMs = durationInMs;
    return *this;
  }

  Scene &showScene() {
    m_Fn();
    pt.tune_delay(m_DurationInMs);
    return *this;
  }

private:
  int (*m_Fn)(void);
  int m_DurationInMs;
};

// *********************************** O B J E C T  S N A K E
class SnakeCoop {
public:
  int8_t m_X[80]{ 0 };  // very limited due RAM size (you can try to experimenting)
  int8_t m_Y[80]{ 0 };  // very limited due RAM size (you can try to experimenting)

  int getSize() const {
    return m_SnakeSize;
  }

  SnakeCoop(int16_t snakeSize) /* all sizes included head */
  {
    m_SnakeSize = snakeSize;

    // Snake is starting from the Big Bang Point
    for (int16_t i = 0; i < m_SnakeSize; i++) {
      m_X[i] = (display.width() / 2) + i;
      m_Y[i] = (display.height() / 2);  // + snakePart;
    }
  }

  ~SnakeCoop() {
  }

private:
  int16_t m_SnakeSize;
};

SnakeCoop snake(InitialSnakeLength);

int8_t lastDirection = ON_BTN_NONE;

// *********************************** M O V E  H E A D  C O O
int8_t moveSnakeHeadCoo(int8_t Direction) {

  switch (Direction) {

    case ON_BTN_LEFT:
      lastDirection = ON_BTN_LEFT;
      snake.m_X[SNAKE_HEAD]--;
      return ON_BTN_LEFT;

    case ON_BTN_RIGHT:
      lastDirection = ON_BTN_RIGHT;
      snake.m_X[SNAKE_HEAD]++;
      return ON_BTN_RIGHT;

    case ON_BTN_UP:
      lastDirection = ON_BTN_UP;
      snake.m_Y[SNAKE_HEAD]--;
      return ON_BTN_UP;

    case ON_BTN_DOWN:
      lastDirection = ON_BTN_DOWN;
      snake.m_Y[SNAKE_HEAD]++;
      return ON_BTN_DOWN;
  }

  return lastDirection;
}

// *********************************** E V E N T  B U T T O N S
int8_t onButtonDown(void) {

  // avoid against yourself direction included

  if (digitalRead(buttonLeftPin) && lastDirection != ON_BTN_RIGHT) {
    return moveSnakeHeadCoo(ON_BTN_LEFT);
  }

  if (digitalRead(buttonRightPin) && lastDirection != ON_BTN_LEFT) {
    return moveSnakeHeadCoo(ON_BTN_RIGHT);
  }

  if (digitalRead(buttonUpPin) && lastDirection != ON_BTN_DOWN) {
    return moveSnakeHeadCoo(ON_BTN_UP);
  }

  if (digitalRead(buttonDownPin) && lastDirection != ON_BTN_UP) {
    return moveSnakeHeadCoo(ON_BTN_DOWN);
  }

  // if here - no button pressed then continue in the last direction
  return moveSnakeHeadCoo(lastDirection);
}

// *********************************** R A N D O M  D E M O  M O V E M E N T
int8_t direction = random(0, 5);
int8_t moveSnakeRandom(void) {

  // left
  if (snake.m_X[SNAKE_HEAD] <= 12 && direction == ON_BTN_LEFT) {
    direction = random(2) ? ON_BTN_UP : ON_BTN_DOWN;
  }

  // right
  if (snake.m_X[SNAKE_HEAD] >= display.width() - 12 && direction == ON_BTN_RIGHT) {
    direction = random(2) ? ON_BTN_UP : ON_BTN_DOWN;
  }

  // up
  if (snake.m_Y[SNAKE_HEAD] <= 12 && direction == ON_BTN_UP) {
    direction = random(2) ? ON_BTN_RIGHT : ON_BTN_LEFT;
  }

  // down
  if (snake.m_Y[SNAKE_HEAD] >= display.height() - 12 && direction == ON_BTN_DOWN) {
    direction = random(2) ? ON_BTN_RIGHT : ON_BTN_LEFT;
  }

  moveSnakeHeadCoo(direction);
  return direction;
}

// *********************************** S N A K E  C O N F L I C T  M G M T
int conflictManagement() {

  // wall
  if (snake.m_X[SNAKE_HEAD] <= 0 || snake.m_X[SNAKE_HEAD] >= display.width() || snake.m_Y[SNAKE_HEAD] <= 0 || snake.m_Y[SNAKE_HEAD] >= display.height())
    return CONFLICT_WALL;

  // self
  for (int i = 2 /*!*/; i < snake.getSize(); i++) {
    if (snake.m_X[SNAKE_HEAD] == snake.m_X[i] && snake.m_Y[SNAKE_HEAD] == snake.m_Y[i]) {
#ifdef SpecialElementsEnabled
      dbg.log("SnakeHead X: ").log(snake.m_X[SNAKE_HEAD]).log("\tY: ").log(snake.m_Y[SNAKE_HEAD], true);
      dbg.log("SnakeTail X: ").log(snake.m_X[i]).log("\tY: ").log(snake.m_Y[i], true);
#endif
      return CONFLICT_SELF;
    }
  }
  return CONFLICT_NONE;
}

// *********************************** S N A K E  L O G I C - S H I F T
void logicSnake() {
  // each part of snake get previous part of snake
  int8_t prevHeadX = snake.m_X[SNAKE_HEAD];
  int8_t prevHeadY = snake.m_Y[SNAKE_HEAD];

  // tail copy
  int8_t prev2X, prev2Y;
  for (int8_t snakePart = 1; snakePart < snake.getSize(); snakePart++) {
    // save current tail position
    prev2X = snake.m_X[snakePart];
    prev2Y = snake.m_Y[snakePart];

    // tail shift to previous position
    snake.m_X[snakePart] = prevHeadX;
    snake.m_Y[snakePart] = prevHeadY;

    // update for other tail parts
    prevHeadX = prev2X;
    prevHeadY = prev2Y;
  }
}

// *********************************** G A M E  L O O P
void loop() {

  if (isDemo) {
    // DEMO ------------
    if (moveSnakeRandom() != ON_BTN_NONE)
      logicSnake();

    if (onAnyButtonDown()) {  // interupt Demo mode and jump to the game
      isDemo = false;
      // setup();  // recall setup
      resetArduino();
    }

  } else {
    // GAME ------------
    if (onButtonDown() != ON_BTN_NONE) {
      logicSnake();
      int8_t conflict = conflictManagement();
      if (conflict == CONFLICT_WALL) {
        sceneConflictWall();
        pt.tune_delay(2000);
        resetArduino();
      }
      if (conflict == CONFLICT_SELF) {
        sceneConflictSelf();
        pt.tune_delay(2000);
        resetArduino();
      }
    }
  }

  // UI
  sceneFieldBorders();
  drawSnake();
  display.display();  // Update screen with each newly-drawn

  // fps
  pt.tune_delay(10);  // fps
}

// *********************************** O B J E C T  S N A K E
void drawSnake() {
  for (int8_t i = 0; i < snake.getSize(); i++) {
    display.drawPixel(snake.m_X[i], snake.m_Y[i], SSD1306_WHITE);
  }

  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

#ifdef SpecialElementsEnabled  // Draw size of the Snake in the left corner
  display.setCursor(1, 1);
  display.println(snake.getSize());
#endif

  if (isDemo) {
    display.setCursor(50, 28);
    display.println(F("DEMO"));
  }
}

// *********************************** S C E N E - F I E L D  B O R D E R S
void sceneFieldBorders(void) {
  display.clearDisplay();
  display.drawRect(0, 0, display.width(), display.height(), SSD1306_WHITE);
}

// *********************************** S C E N E  S E L F
void sceneConflictSelf() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(16, 10);
  display.println(F("SELF EAT"));
  display.println(F(""));
  display.setCursor(14, 40);
  display.println(F("GAMEOVER"));
  display.display();
}

// *********************************** S C E N E  W A L L S
void sceneConflictWall() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(16, 10);
  display.println(F("HIT WALL"));
  display.println(F(""));
  display.setCursor(14, 40);
  display.println(F("GAMEOVER"));
  display.display();
}

// *********************************** S C E N E  S T A R T I N G  G .
void sceneStartingGame() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.println(F("STARTING"));
  display.println(F(""));
  display.setCursor(19, 40);
  display.println(F("G A M E"));
  display.display();
}

// *********************************** S C E N E  S T A R T I N G  D .
void sceneStartingDemo() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.println(F("STARTING"));
  display.println(F(""));
  display.setCursor(19, 40);
  display.println(F("D E M O"));
  display.display();
}

// *********************************** S C E N E  S N A K E  4 E V E R
void sceneSnake4Ever() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(F("  SNAKE"));
  display.println(F(""));
  display.setCursor(10, 40);
  display.println(F(" -4EVER-"));
  display.display();
}

// *********************************** S C E N E  P R E S S  A N Y  B T N
void scenePressAnyButton() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(F("  PRESS"));
  display.println(F(""));
  display.setCursor(4, 40);
  display.println(F("ANY BUTTON"));
  display.display();
}

// *********************************** S C E N E  P R E S E N T
void scenePresent() {
  display.clearDisplay();
  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(F("Tomas Mark (c)2024"));
  display.setCursor(10, 30);
  display.println("     PRESENT");
  display.setCursor(10, 50);
  display.println(F("DIGITALSPACE.NAME "));
  display.display();
}

// *********************************** S C E N E  C O U N T D O W N
void sceneCountDown(int8_t countDownSince) {
  for (int8_t i = countDownSince; i > 0; i--) {
    display.clearDisplay();
    display.setTextSize(3);  // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 20);
    display.println(i);
    display.display();
    pt.tune_delay(500);
  }
  display.clearDisplay();
  display.setCursor(30, 20);
  display.println(F("GO"));
  display.display();
  pt.tune_delay(500);
}

// *********************************** S C E N E  F X
void sceneRectangleFx(void) {
  display.clearDisplay();
  for (int8_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SSD1306_WHITE);
    display.display();  // Update screen with each newly-drawn rectangle
    pt.tune_delay(1);
  }
}

// *********************************** H A R D W A R E  T I M E R S
void setAudioTimers(void) {
  pt.tune_initchan(8);
  pt.tune_initchan(9);
  pt.tune_initchan(10);
}

// *********************************** P I N  M O D E
void setButtonMode(void) {
  // initialize the pushbutton pin as an input:
  pinMode(buttonLeftPin, INPUT);
  pinMode(buttonRightPin, INPUT);
  pinMode(buttonUpPin, INPUT);
  pinMode(buttonDownPin, INPUT);
}

// *********************************** O L E D  D I S P L A Y
void setOled(void) {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
}

// *********************************** S E T U P  F U N C T I O N
void setup() {

  Serial.begin(9600);
  setOled();

  setButtonMode();
  setAudioTimers();
  randomSeed(analogRead(0));

  // Scene rotation
  if (!Playtune::tune_playing && isMusicOn)
    pt.tune_playscore(scoreWelcome);

  // rotate 2x
  for (int i = 0; i < 2; i++) {

    isDemo = false;

    Scene scene(scenePresent, 3000);
    if (onAnyButtonDown()) {
      break;
    }

    scene.setFunction(sceneSnake4Ever).setDuration(3000).showScene();
    if (onAnyButtonDown()) {
      break;
    }

    scene.setFunction(scenePressAnyButton).setDuration(3000).showScene();
    if (onAnyButtonDown()) {
      break;
    }

    scene.setFunction(sceneRectangleFx).setDuration(300).showScene();
    if (onAnyButtonDown()) {
      break;
    }

    isDemo = true;
  }

  if (Playtune::tune_playing && isMusicOn)
    pt.tune_stopscore();

  if (!Playtune::tune_playing && isMusicOn)
    pt.tune_playscore(scoreStartGame);  // Starting Game

  if (isDemo)
    Scene scene(sceneStartingDemo, 2000);
  else {
    Scene scene(sceneStartingGame, 2000);

    // countdown screens
    // 3 / 2 / 1 / GO
    sceneCountDown(3);
  }

  dbg.log("leaving setup() function", true);
  dbg.log("Available ").log(freeMemory).log(" bytes of RAM", true);
}