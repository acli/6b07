// Brown = Black = Ground
// Orange = Red = Positive
// Yellow = White = Signal
// http://helifreak.com/showthread.php?t=151527

// +5V to 1kΩ, (1kΩ, LDR) to signal, LDR to GND
// http://hobbybotics.com/tutorials/tutorial-use-a-light-dependent-resistor-ldr-to-measure-light-levels/

#include <Servo.h> 

const int ldr_pin = A0;
const int led_pin = 3;
const int servo_pin = 9;

const int servo_low = 0;
const int servo_high = 80;
 
Servo myservo;  // create servo object to control a servo 

 
void setup() 
{ 
  pinMode(led_pin, OUTPUT);
  myservo.attach(servo_pin); 
} 

void do_servo_action () {
  static int j;
  static int dir = 1;
  int light_level = analogRead(ldr_pin);
  int rate = light_level/105;
  myservo.write(j);
  if (dir > 0) {
    j += dir*rate;
    if (j > servo_high) {
      j = servo_high;
      dir = -1;
    }
  } else {
    j += dir*rate;
    if (j < servo_low) {
      j = servo_low;
      dir = +1;
    }
  }
}

void do_led_action () {
  const int countdown_initial = 5;
  static int countdown;
  countdown -= 1;
  if (countdown <= 0) {
    static int lighting_level = 128;
    lighting_level += random(-4, 8);
    if (lighting_level < 0) {
      lighting_level = 0;
    } else if (lighting_level > 255) {
      lighting_level = 255;
    }
    analogWrite(led_pin, lighting_level);
    countdown = countdown_initial;
  }
}

void loop() 
{
  do_servo_action();
  do_led_action();
  delay(10);
} 
