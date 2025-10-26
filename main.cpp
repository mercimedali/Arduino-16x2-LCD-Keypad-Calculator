#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

// Initialize the LCD (pins: RS, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {30, 31, 32, 33};
byte colPins[COLS] = {34, 35, 36, 37};

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum CalcState { STATE_IDLE, STATE_SELECT_MODE, STATE_ENTER_FIRST, STATE_ENTER_SECOND, STATE_SHOW_RESULT };
enum CalcMode { MODE_NONE, MODE_ADD, MODE_SUB, MODE_MUL, MODE_DIV, MODE_MOD };

CalcState currentState = STATE_IDLE;
CalcMode currentMode = MODE_NONE;
String firstNumber = "";
String secondNumber = "";
float lastResult = 0.0;

// Forward declarations
void handleNumberInput(String &number, char key);
void computeResult();
void selectMode();

void setup() {
  lcd.begin(16, 2);
}

// Display mode options safely for 16x2 LCD
void selectMode() {
  lcd.clear();
  lcd.print("1:+ 2:- 3:* 4:/");
  lcd.setCursor(0,1);
  lcd.print("5:%  C:-");
}

// Main calculator logic
void calculator() {
  static bool shownWelcome = false;
  if (currentState == STATE_IDLE && !shownWelcome) {
    lcd.clear();
    lcd.print("Welcome to Calc!");
    lcd.setCursor(0,1); lcd.print("Press any key");
    shownWelcome = true;
  }

  char key = customKeypad.getKey();
  if (!key) return;

  switch(currentState) {

    case STATE_IDLE:
      currentState = STATE_SELECT_MODE;
      shownWelcome = false;
      selectMode();
      break;

    case STATE_SELECT_MODE:
      if (key >= '1' && key <= '5') {
        currentMode = MODE_NONE;
        switch (key) {
          case '1': currentMode = MODE_ADD; break;
          case '2': currentMode = MODE_SUB; break;
          case '3': currentMode = MODE_MUL; break;
          case '4': currentMode = MODE_DIV; break;
          case '5': currentMode = MODE_MOD; break;
        }
        firstNumber = ""; secondNumber = "";
        currentState = STATE_ENTER_FIRST;
        lcd.clear();
        lcd.print("First:");
      }
      break;

    case STATE_ENTER_FIRST:
      handleNumberInput(firstNumber, key);
      break;

    case STATE_ENTER_SECOND:
      handleNumberInput(secondNumber, key);
      break;

    case STATE_SHOW_RESULT:
      currentState = STATE_SELECT_MODE;
      selectMode();
      break;
  }
}

// Unified input handler for first and second number
void handleNumberInput(String &number, char key) {
  if (key >= '0' && key <= '9' && number.length() < 16) {
    number += key;
  } 
  else if (key == 'A' && number.indexOf('.') == -1 && number.length() < 15) {
    number = (number.length() == 0) ? "0." : number + ".";
  } 
  else if (key == 'B' && number.length() > 0) {
    number.remove(number.length()-1);
  } 
  else if (key == 'C') { // toggle negative
    if (number.startsWith("-")) {
      number.remove(0,1);
    } else {
      number = "-" + number;
    }
  } 
  else if (key == '*') { // cancel
    currentState = STATE_IDLE;
    lcd.clear();
    return;
  } 
  else if (key == '#') { // enter
    if (&number == &firstNumber && number.length() > 0) {
      currentState = STATE_ENTER_SECOND;
      lcd.clear();
      lcd.print("Second:");
    } 
    else if (&number == &secondNumber && number.length() > 0) {
      computeResult();
      return;
    }
  }

  lcd.setCursor(0,1);
  lcd.print("                "); // clear line
  lcd.setCursor(0,1);
  lcd.print(number);
}

// Compute the operation result
void computeResult() {
  float a = firstNumber.toFloat();
  float b = secondNumber.toFloat();
  bool err = false;

  switch(currentMode) {
    case MODE_ADD: lastResult = a + b; break;
    case MODE_SUB: lastResult = a - b; break;
    case MODE_MUL: lastResult = a * b; break;
    case MODE_DIV: err = (b==0.0); if(!err) lastResult = a/b; break;
    case MODE_MOD: err = (b==0.0); if(!err) lastResult = fmod(a,b); break;
    default: err = true; break;
  }

  lcd.clear();
  if (err) {
    lcd.print("Error");
    lcd.setCursor(0,1); lcd.print("Invalid op");
  } else {
    lcd.print("Result:");
    lcd.setCursor(0,1);
    String out = String(lastResult,6);
    if(out.length() > 16) out = out.substring(0,16);
    lcd.print(out);
  }
  currentState = STATE_SHOW_RESULT;
}

void loop() {
  calculator();
}
