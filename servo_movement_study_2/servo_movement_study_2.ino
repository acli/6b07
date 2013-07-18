// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 


// Brown = Black = Ground
// Orange = Red = Positive
// Yellow = White = Signal
// http://helifreak.com/showthread.php?t=151527

// +5V to 1kΩ, (1kΩ, LDR) to signal, LDR to GND
// http://hobbybotics.com/tutorials/tutorial-use-a-light-dependent-resistor-ldr-to-measure-light-levels/

#include <Servo.h> 

const int ldr_pin = A0;
const int servo_pin = 9;
 
Servo myservo;  // create servo object to control a servo 
 
 
void setup() 
{ 
  myservo.attach(servo_pin); 
} 


void loop() 
{
  static int j;
  static int dir = 1;
  int light_level = analogRead(ldr_pin);
  int rate = light_level/100;
  myservo.write(j);
  if (dir > 0) {
    j += dir*rate;
    if (j > 180) {
      dir = -1;
    }
  } else {
    j += dir*rate;
    if (j < 0) {
      dir = +1;
    }
  }
  delay(10);
} 
