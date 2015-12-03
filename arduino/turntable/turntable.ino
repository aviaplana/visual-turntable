/*
 * TODO: What happens when we reach the max/min in fade mode? stay at max/min or get back to the initial speed?
 */

#include "buttons.h"

#define DEBUG 1

// Analog pins
#define PIN_POD_SPEED A0
#define PIN_BUTTONS A1

// Digital pins
#define PIN_MOTOR_SPEED 3 

#define ACTION_IDLE 0
#define ACTION_FADE_MIN 1
#define ACTION_FADE_MAX 2

Buttons buttons(PIN_BUTTONS);

// Speed value in which the motor stops.
uint8_t min_speed = 45;
uint8_t current_speed = 45;

uint8_t current_action = ACTION_IDLE;

int action_delay = 100;

unsigned long last_button_int;
unsigned long last_action;

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
  if ((current_action != ACTION_FADE_MIN) && (current_action != ACTION_FADE_MAX)) {
    current_speed = map(analogRead(PIN_POD_SPEED), 0, 1024, min_speed, 254);
  }
  
  if (DEBUG){
    if (button_pressed && buttons.readyToRead()) {
      int button = buttons.checkButtons();
      Serial.println("Button: " + String(button)); 
      assignAction(button);  
      button_pressed = false;   
    }
  }
  
  performAction();

  analogWrite(PIN_MOTOR_SPEED, current_speed);
}

void assignAction(int button)
{
  switch (button) {
    case 0:
      current_action = ACTION_FADE_MIN;
      break;
      
    case 1:
      current_action = ACTION_FADE_MAX;
      break;
      
    case 2:
      break;
      
    case 3:
      break;
      
    case 4:
      break;
      
    case 5:
      break;

    default:
      break;
  }
}

void performAction()
{
  switch (current_action) {
    case ACTION_IDLE:
      break;
      
    case ACTION_FADE_MIN:
        fadeMin();
      break;
      
    case ACTION_FADE_MAX:
        fadeMax();
      break;
  }
}

void fadeMin()
{
  if ((millis() - last_action) > action_delay) {
    current_speed -= 5;
    
    if (current_speed < min_speed) {
      current_speed = min_speed;
    }
    
    analogWrite(PIN_MOTOR_SPEED, current_speed);
    
    if (current_speed <= min_speed) {
      current_action = ACTION_IDLE;
    }

    if (DEBUG) {
      Serial.println("Current speed: " + String(current_speed));
    }
    
    last_action = millis();
  }
}

void fadeMax()
{
  if ((millis() - last_action) > action_delay) {
    current_speed += 5;
  
    if (current_speed > 254) {
      current_speed = 254;
    }
  
    analogWrite(PIN_MOTOR_SPEED, current_speed);
  
    if (current_speed >= 254) {
      current_action = ACTION_IDLE;
      Serial.println("entra");
    }
    
    if (DEBUG) {
      Serial.println("Current speed: " + String(current_speed));
    }
    
    last_action = millis();
  }
}

void buttonPress()
{
  if ((millis() - last_button_int) > 500) {
    last_button_int = millis();
    buttons.readButtons();
    button_pressed = true;
  }
}


