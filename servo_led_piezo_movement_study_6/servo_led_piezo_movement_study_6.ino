// Brown = Black = Ground
// Orange = Red = Positive
// Yellow = White = Signal
// http://helifreak.com/showthread.php?t=151527

// +5V to 1kΩ, (1kΩ, LDR) to signal, LDR to GND
// http://hobbybotics.com/tutorials/tutorial-use-a-light-dependent-resistor-ldr-to-measure-light-levels/

// LDR values are low = light, high = dark
// The large LDR gives readings from say 80 to 200

#include <Servo.h> 

const int ldr_pin = A0;
const int led_pin = 3;
const int servo_pin = 9;
const int speaker_pin = 10;               

const int servo_low = 0;
const int servo_high = 80;

const float one_second = 1000000.0;
float tempo = 60.0;

// frequencies from http://www.phy.mtu.edu/~suits/notefreqs.html
#define b3 246.94
#define c4 261.63
#define cis4 277.18
#define d4 293.66
#define dis4 311.13
#define e4 329.63
#define f4 349.23
#define fis4 369.99
#define g4 392.00
#define gis4 415.30
#define a4 440.00
#define ais4 466.16
#define b4 493.88
#define c5 523.25

#define hz1 440.00
#define dl1 469.86
#define dc1 495.00
#define jz1 528.64
#define gx1 556.88
#define zl1 594.39
#define rb1 626.48
#define lz1 660.00
#define yz1 704.79
#define nl1 742.50
#define wy1 792.86
#define yz2 835.31

#define SCALE 12
int scale[SCALE] = {hz1, dl1, dc1, jz1, gx1, zl1, rb1, lz1, yz1, nl1, wy1, yz2};

Servo myservo;  // create servo object to control a servo 

 
void setup() 
{ 
  pinMode(led_pin, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
  myservo.attach(servo_pin); 
  Serial.begin(9600);
} 

void make_tone(float f, float len) {
  float period = one_second / f;
  float duration = (60*one_second / tempo) * len;
  unsigned long d1 = period/2;
  unsigned long d2 = period - d1;
  unsigned long t_i = micros();
  unsigned long t_f = t_i + duration;
  for (; micros() < t_f;) {
    digitalWrite(speaker_pin, HIGH);
    delayMicroseconds(d1);
    digitalWrite(speaker_pin, LOW);
    delayMicroseconds(d2);
  }
}

void do_servo_action () {
  static float j0 = 0;
  static int j = 0;
  static int j_last;
  static int dir = 1;
  int light_level = analogRead(ldr_pin);
  float rate = pow(light_level, 1.414)/2000.0;
  Serial.println(light_level); Serial.print(" "); Serial.println(sqrt(light_level));
  j = int(j0);
  if (j != j_last) {
    myservo.write(j);
    j_last = j;
  }
  if (dir > 0) {
    j0 += dir*rate;
    if (j0 > servo_high) {
      j0 = servo_high;
      dir = -1;
    }
  } else {
    j0 += dir*rate;
    if (j0 < servo_low) {
      j0 = servo_low;
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

void make_random_sound() {
  static int i;
  static int j = 440;
  static int sound_on = 0;
  static unsigned long last_sound_toggle;
  if (sound_on) {
    make_tone(j, 0.01);
    i += 1;
    i %= SCALE;
    j += random(-16, 16);
    if (j < 440) {
      j = 440;
    } else if (j > 1000) {
      j = 1000;
    }
  } else {
    delay(10);
  }
  if (random(100) < 1000 && millis() - last_sound_toggle > 5) {
    sound_on = !sound_on;
    last_sound_toggle = millis();
  }
}

void make_random_note() {
  static int i;
  static int j = 0;
  static int sound_on = 0;
  static unsigned long last_sound_toggle;
  static unsigned long last_note_change;
  if (sound_on) {
    make_tone(scale[j], 0.01);
    if (millis() - last_note_change > 1000) {
      i += 1;
      i %= SCALE;
      j += random(-4, 4);
      if (j < 0) {
        j = 0;
      } else if (j > SCALE - 1) {
        j = SCALE - 1;
      }
      last_note_change = millis();
    }
  } else {
    delay(10);
  }
  if (random(100) < 1000 && millis() - last_sound_toggle > 5) {
    sound_on = !sound_on;
    last_sound_toggle = millis();
  }
}

void loop() 
{
  do_servo_action();
  do_led_action();
  //make_random_sound();
  //make_random_note();
}
