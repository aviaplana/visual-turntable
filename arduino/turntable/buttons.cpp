#include "buttons.h"

Buttons::Buttons(uint8_t pin_buttons)
{
  this->pin_buttons = pin_buttons;
}

int Buttons::checkButtons() 
{
  int val_button = analogRead(this->pin_buttons);
  
  if ((val_button > button_values[0][0]) && (val_button < button_values[0][1])) {
    return 0;
  } else if ((val_button > button_values[1][0]) && (val_button < button_values[1][1])) {
    return 1;    
  } else if ((val_button > button_values[2][0]) && (val_button < button_values[2][1])) {
    return 2;    
  } else if ((val_button > button_values[3][0]) && (val_button < button_values[3][1])) {
    return 3;
  } else if ((val_button > button_values[4][0]) && (val_button < button_values[4][1])) {
    return 4;
  } else if ((val_button > button_values[5][0]) && (val_button < button_values[5][1])) {
    return 5;
  }

  if (Serial) {
    Serial.println("Unknown button pressed. Got value " + String(val_button));
  }
  
  return -1;
}

bool Buttons::readyToRead()
{
  return (millis() - this->first_read) > this->debounce_time;
}

void Buttons::readButtons()
{
  this->first_read = millis();
}

