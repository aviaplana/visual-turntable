#include "buttons.h"

#define DEBUG 1

// Analog pins
#define PIN_POD_SPEED A0
#define PIN_BUTTONS A1

// Digital pins
#define PIN_MOTOR_SPEED 3 

Buttons buttons(PIN_BUTTONS);

// Speed value in which the motor stops.
uint8_t min_speed = 45;

unsigned long last_button_int;
bool button_pressed = false;


void setup() 
{
  pinMode(PIN_MOTOR_SPEED, OUTPUT);
  
  if (DEBUG) {
    Serial.begin(9600);
  }

  last_button_int = millis();
  
  attachInterrupt(0, buttonPress, RISING);
}

void loop() 
{
  int motor_speed = map(analogRead(PIN_POD_SPEED), 0, 1024, min_speed, 254);
  
  if (DEBUG)
  {
    if (button_pressed && buttons.readyToRead()) {
      Serial.println("Button: " + String(buttons.checkButtons()));   
      button_pressed = false;   
    }
  }

  analogWrite(PIN_MOTOR_SPEED, motor_speed);
  
}

void buttonPress()
{
  if ((millis() - last_button_int) > 500) {
    last_button_int = millis();
    buttons.readButtons();
    button_pressed = true;
  }
}


