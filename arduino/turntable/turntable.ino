/*
 * TODO: What happens when we reach the max/min in fade mode? stay at max/min or get back to the initial speed?
 */

#include "buttons.h"

#define DEBUG 1

// Analog pins
#define PIN_BUTTONS A0
#define PIN_ENCODER A1 
#define PIN_POD_SPEED A2
#define PIN_POD_TIME_FADE A3
#define PIN_POD_TIME_ENCODER A4

// Digital pins
#define PIN_MOTOR_RIGHT 5 
#define PIN_MOTOR_LEFT 6 

#define ACTION_IDLE 0
#define ACTION_FADE_MIN 1
#define ACTION_FADE_MAX 2
#define ACTION_MOVE_DISTANCE 3
#define ACTION_MOVE_RANDOM 4

Buttons buttons(PIN_BUTTONS);

// Speed value in which the motor stops.
uint8_t min_speed = 120;
unsigned int current_speed = min_speed;

uint8_t current_action = ACTION_IDLE;

unsigned int tics_enc = 0;
unsigned int tics_enc_obj = 0;
unsigned int enc_comparator = 600;

unsigned int action_delay = 1000;
unsigned long min_time_enc = 1000;
unsigned long max_time_enc = 10000;
unsigned long min_time_fade = 10000;
unsigned long max_time_fade = 60000;

unsigned long last_button_int;
unsigned long last_enc;
unsigned long last_action;

bool button_pressed = false;
bool prev_encoder = false;
bool stop_motor = false;
bool reached_fade = false;



void setup() 
{
  pinMode(PIN_MOTOR_LEFT, OUTPUT);
  
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
  
  if (DEBUG) {
    if (button_pressed && buttons.readyToRead()) {
      int button = buttons.checkButtons();
      Serial.println("Button: " + String(button)); 
      assignAction(button);  
      button_pressed = false;   
    }
  }
 
  
  performAction();

  if (!stop_motor) {
    analogWrite(PIN_MOTOR_LEFT, current_speed);
  }
}

void assignAction(int button)
{
  switch (button) {
    case 0:
      setActionDelay();
      reached_fade = false;
      current_action = ACTION_FADE_MIN;
      break;
      
    case 1:
      setActionDelay();
      reached_fade = false;
      current_action = ACTION_FADE_MAX;
      break;

    // 1/8
    case 2:
      setMoveState(2);
      break;

    // 1/4
    case 3:
      setMoveState(4);
      break;

    // 1
    case 4:
      setMoveState(16);
      break;
      
    case 5:
      setMoveState(random(1, 16));
      current_action = ACTION_MOVE_RANDOM;
      break;

    default:
      break;
  }
}

void setActionDelay()
{
  unsigned long pod_val = map(analogRead(PIN_POD_TIME_FADE), 0, 1024, min_time_fade, max_time_fade);
  action_delay = (int) (pod_val / ((int) (254 - current_speed) / 5));
  
  if (DEBUG) {
    Serial.println("Total fade time: " + String(pod_val) + " Step delay: " + String(action_delay));
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
      
    case ACTION_MOVE_DISTANCE:
    case ACTION_MOVE_RANDOM:
        moveDistance();
      break;
  }
}

void fadeMin()
{  
  if (reached_fade == false && ((millis() - last_action) > action_delay)) {
    current_speed -= 5;
    
    if (current_speed < min_speed) {
      current_speed = min_speed;
    }
    
    analogWrite(PIN_MOTOR_LEFT, current_speed);
    
    if (current_speed <= min_speed) {
      if (DEBUG) {
        Serial.println("Reached max");
      }
      reached_fade = true;
    }

    if (DEBUG) {
      Serial.println("Current speed: " + String(current_speed));
    }
    
    last_action = millis();
  }
}

void fadeMax()
{  
  if (reached_fade == false && ((millis() - last_action) > action_delay)) {
    current_speed += 5;
  
    if (current_speed > 254) {
      current_speed = 254;
    }
  
    analogWrite(PIN_MOTOR_LEFT, current_speed);
  
    if (current_speed >= 254) {
      reached_fade = true;
      if (DEBUG) {
        Serial.println("Reached max");
      }
    }
    
    if (DEBUG) {
      Serial.println("Current speed: " + String(current_speed));
    }
    
    last_action = millis();
  }
}

void setMoveState(unsigned int tics)
{
    prev_encoder = (analogRead(PIN_ENCODER) < enc_comparator);
    tics_enc = 0;
    tics_enc_obj = tics;
    current_action = ACTION_MOVE_DISTANCE;
}

void moveDistance() 
{
  unsigned long wait_time_enc = map(analogRead(PIN_POD_TIME_ENCODER), 0, 1024, min_time_enc, max_time_enc);
  bool comp_enc = (analogRead(PIN_ENCODER) < enc_comparator);
  
  if (stop_motor) {
    if ((millis() - last_enc) >= wait_time_enc) {
      stop_motor = false;

      if (current_action == ACTION_MOVE_RANDOM) {
        tics_enc_obj = random(1, 16);
      }
      
      if (DEBUG) {
        Serial.println("End waiting");
      }
    }
  } else  if ((comp_enc != prev_encoder) && (millis() - last_enc) > 20) {
    last_enc = millis();
    prev_encoder = comp_enc;
    tics_enc++;
    
    if (DEBUG) {
      Serial.println("Tics encoder: " + String(tics_enc));
    }
    
    if (tics_enc >= tics_enc_obj) {
      tics_enc = 0;
      stopMotor();
    }
  }
}

void stopMotor()
{
  if (DEBUG) {
    Serial.println("Motor stoped");
  }
  
  stop_motor = true;
  analogWrite(PIN_MOTOR_LEFT, 0);
}

void buttonPress()
{
  if ((millis() - last_button_int) > 500) {
    last_button_int = millis();
    buttons.readButtons();
    button_pressed = true;
  }
}
