Arduino (Elegoo Mega 2560) LCD & Keypad Calculator
Overview

A fully functional calculator built using an Elegoo Mega 2560 (Arduino-compatible), a 16x2 LCD, and a 4x4 keypad. This project demonstrates embedded systems programming, state-machine design, and real-time user interface handling.

It supports basic arithmetic operations, negative numbers, and decimal input, displaying results clearly on the LCD.

Features

Addition, subtraction, multiplication, division, modulus

Decimal and negative number support

Clear, cancel, and backspace functionality

Error handling for invalid operations (e.g., division by zero)

Results can be reused for subsequent calculations

Clean, modular Arduino code

Hardware Used

Elegoo Mega 2560 R3 (Arduino-compatible)

16x2 LCD with LiquidCrystal library

4x4 Keypad with Keypad library

Breadboard & jumper wires

How It Works

Idle State: Displays a welcome message.

Mode Selection: User selects the operation via keypad (1–5).

First Number Entry: Supports decimal points and negative numbers.

Second Number Entry: Similar input handling.

Computation: Arduino calculates the result based on the chosen operation.

Result Display: Shows the result or an error message.

Next Operation: Press any key to return to mode selection.

Key Mappings
Key	Function
'1'	Addition (+)
'2'	Subtraction (-)
'3'	Multiplication (*)
'4'	Division (/)
'5'	Modulus (%)
'A'	Decimal point
'B'	Backspace
'C'	Toggle negative
'*'	Cancel / reset
'#'	Enter / confirm
Example Usage

Select an operation (e.g., 1 for addition).

Enter the first number (e.g., -12.5).

Enter the second number (e.g., 7).

View the result (LCD displays Result: -5.5).

Press any key to return to mode selection for a new calculation.

Error Handling:

Division by zero triggers "Invalid op".

Pressing enter without a second number triggers "No 2nd number".

Screenshots / Media


https://github.com/user-attachments/assets/5cd700c8-acb7-40d8-9239-b9756cc310d6

Usage

![waiting_for_mode_selection](https://github.com/user-attachments/assets/181da7a6-ca32-4829-b304-3e30d326b035)

Waiting for mode selection

![welcome_screen](https://github.com/user-attachments/assets/ee241e0c-f91b-4671-8d00-cb9e3506f824)

Welcome screen





Code Snippet

void handleNumberInput(String &number, char key) {
  if (key >= '0' && key <= '9' && number.length() < 16) number += key;
  else if (key=='A' && number.indexOf('.')==-1 && number.length()<15) number = (number.length()==0)?"0.":number+".";
  else if (key=='B' && number.length()>0) number.remove(number.length()-1);
  else if (key=='C') number.startsWith("-")?number.remove(0,1):number="-"+number;
  else if (key=='*') { currentState=STATE_IDLE; lcd.clear(); return; }
  else if (key=='#') { /* enter logic */ }
  lcd.setCursor(0,1); lcd.print("                "); lcd.setCursor(0,1); lcd.print(number);
}


Full code is included in the repository/project folder.

Demonstrates embedded systems programming, state-machine design, and interactive UI handling on a microcontroller.
