// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 


// Brown = Black = Ground
// Orange = Red = Positive
// Yellow = White = Signal
// http://helifreak.com/showthread.php?t=151527

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 


void loop() 
{
  static int j;
  static int dir = 1;
  myservo.write(j);
  if (dir > 0) {
    j += dir;
    if (j > 180) {
      dir = -1;
    }
  } else {
    j += dir;
    if (j < 0) {
      dir = +1;
    }
  }
  delay(5);
} 
