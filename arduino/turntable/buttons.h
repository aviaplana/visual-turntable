#ifndef Buttons_h
#define Buttons_h
#include <Arduino.h>

class Buttons {
  public:
    Buttons(uint8_t buttonPin);
    int checkButtons();
    bool readyToRead();
    void readButtons();
    
    
  
  private:
    const PROGMEM int button_values[6][2] = {{594, 604},
                                             {692, 702},
                                             {765, 775},
                                             {833, 843},
                                             {875, 885},
                                             {925, 935}};
    const uint8_t debounce_time = 50;
    
    uint8_t pin_buttons;
    boolean button_pressed = false;
    unsigned int first_read;
};
#endif
