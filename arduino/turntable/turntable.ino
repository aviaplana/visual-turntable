#define DEBUG 1

// Analog pins
#define PIN_POD_SPEED 0

// Digital pins
#define PIN_MOTOR_SPEED 3 

// Speed value in which the motor stops.
uint8_t min_speed = 45;


void setup() 
{
  pinMode(PIN_MOTOR_SPEED, OUTPUT);
  
  if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() 
{
  int motor_speed = map(analogRead(PIN_POD_SPEED), 0, 1024, min_speed, 254);
  
  if (DEBUG)
  {
    Serial.println(motor_speed);
  }
  
  analogWrite(PIN_MOTOR_SPEED, motor_speed);
  
}
