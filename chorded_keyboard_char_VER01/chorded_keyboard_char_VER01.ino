/*********************************************************************
  
  Human Interface Design (HID)
  Basic Chorded Keyboard
  with chord delay and auto repeat
  By Dylan Skewes
  based on the code by Laura Maisy
  RMIT University
  based on code by:
  Scott Mitchell, RMIT University
  Adafruit Bluefruit
  MIT license
  This code is designed for use with the Adafruit Bluefruit Feather board
*********************************************************************/

#include "keyboardSetup.h"
String KEYBOARD_NAME = "myKeyboardName";

// characters. Note that [32] is updated because there are 5 buttons, also '0x08' is hex for backspace
const char keyMap[64] = {'0', 'E', 'A', 'T', 'R', 'N', 'S', 'G', 'I', 'L',
                         'C', 'B', 'U', 'F', 'Y', 'J', 'O', 'D', 'P', 'W',
                         'M', 'K', 'V', 'Q', 'H', 'X', 'Z', '0', '1', '2',
                         '3', '4', '5', '6', '7', '8', '9', '_', '_', ' ',
                         '.', ',', '?', '!', '/', '(', ')'
                        };

// pin settings
const int button0 = 12;
const int button1 = 11;
const int button2 = 10;
const int button3 = 9;
const int button4 = 6;
const int button5 = 13;
const int LEDpin = 5;


//
// Setup the system - run once
//
void setup(void)
{
  while (!Serial);  // wait for Serial to startup - maybe don't need this?
  delay(500);

  Serial.begin(115200);
  Serial.println("Basic Chorded Keyboard");
  Serial.println("---------------------------------------");

  setupKeyboard(KEYBOARD_NAME);

  // setup pins
  pinMode(button0, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);
}


//
// Loop the program - run forever
//
void loop(void) { // button states are represented by 0s and 1s in a binary number
  byte buttonState = readButtonState();
  byte h = 0; // the 'highest' key press

  // only runs when there are keys being pressed
  while (buttonState != 0)
  { if (buttonState > h)
    {
      h = buttonState;  // write the button state as the highest option
    }
    buttonState = readButtonState();
  }

  // sends h if it is not 0
  if (h != 0)
  {
    sendChar(h);
  }
}


byte readButtonState()
{ // start with them all off. Note B00000 not B000000 because 5 not 6 buttons
  byte bState = B00000;

  // turn on the bits if the button is being pressed
  if (digitalRead(button0) == false) bitSet(bState, 0);
  if (digitalRead(button1) == false) bitSet(bState, 1);
  if (digitalRead(button2) == false) bitSet(bState, 2);
  if (digitalRead(button3) == false) bitSet(bState, 3);
  if (digitalRead(button4) == false) bitSet(bState, 4);

  return (bState);
}

void sendChar(byte buttonState)
{ Serial.print("Sending Byte: ");
  Serial.print(buttonState, BIN);
  Serial.print(" Number: ");
  Serial.print(buttonState, DEC);
  Serial.print(", Char: ");
  Serial.println(keyMap[buttonState]);

  ble.print("AT+BleKeyboard=");
  ble.println(keyMap[buttonState]);

  // wait for feedback
  if ( ble.waitForOK() )
  {
    Serial.println("OK!");
  }
  else
  {
    Serial.println("FAILED!");
  }
}
