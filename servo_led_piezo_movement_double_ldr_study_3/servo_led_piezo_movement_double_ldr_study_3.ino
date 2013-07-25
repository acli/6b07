// Brown = Black = Ground
// Orange = Red = Positive
// Yellow = White = Signal
// http://helifreak.com/showthread.php?t=151527

// +5V to 1kΩ, (1kΩ, LDR) to signal, LDR to GND
// http://hobbybotics.com/tutorials/tutorial-use-a-light-dependent-resistor-ldr-to-measure-light-levels/

// LDR values are low = light, high = dark
// The large LDR gives readings from say 80 to 200

#include <Servo.h> 

const int ldr_1_pin = A0;
const int ldr_2_pin = A1;
const int led_1_pin = 3;
const int led_2_pin = 4;
const int led_3_pin = 5;
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

 
void setup() { 
  pinMode(led_1_pin, OUTPUT);
  pinMode(led_2_pin, OUTPUT);
  pinMode(led_3_pin, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
  myservo.attach(servo_pin); 
  Serial.begin(9600);
} 

int front_light_level;
int reference_light_level;
int estimated_light_level;

const int LIGHT_LEVEL_HISTORY = 300;
int light_level_history[LIGHT_LEVEL_HISTORY];
int actual_history_size = 0;
int average_light_level_5s;
int average_light_level_60s;
int average_light_level_300s;

void read_sensors() {
  front_light_level = analogRead(ldr_1_pin);
  reference_light_level = analogRead(ldr_2_pin);
  estimated_light_level = front_light_level - reference_light_level;
  
  static unsigned long second_start;
  static int fractional_total = 0;
  static float fractional_readings = 0;
  unsigned long t = millis();
  if (!second_start || t >= second_start + 1000) {
    if (actual_history_size >= LIGHT_LEVEL_HISTORY) {
      for (int i = 1; i < LIGHT_LEVEL_HISTORY; i += 1) {
        light_level_history[i - 1] = light_level_history[i];
      } /* for */
      actual_history_size -= 1;
    } /* if */
    int fractional_average = fractional_readings? int(fractional_readings/fractional_total + 0.5): front_light_level;
    light_level_history[actual_history_size] = fractional_average;
    if (!second_start) {
      second_start = t;
    } else {
      second_start += 1000;
    } /* if */
    fractional_readings = 0;
  } else {
    fractional_readings += 1;
    fractional_total += front_light_level;
  }
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
  int light_level = estimated_light_level;
  if (light_level < 0) { // in case the box is lying sideways
    light_level = -light_level;
  }
  float rate = pow(light_level, 1.618)/2000.0;
  Serial.print(front_light_level);
  Serial.print(" ");
  Serial.print(reference_light_level);
  Serial.print(" -> ");
  Serial.print(light_level);
  Serial.print(" -> ");
  Serial.print(rate);
  Serial.println("");
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
  static unsigned long last_blinked;
  unsigned long t = millis();
  if (t - last_blinked > 1000) {
    digitalWrite(led_1_pin, random(2));
    digitalWrite(led_2_pin, random(2));
    digitalWrite(led_3_pin, random(2));
    last_blinked = t;
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

void make_sound() {
  static int i;
  static int j = 0;
  static int sound_on = 0;
  static unsigned long last_sound_toggle;
  if (sound_on) {
    make_tone(front_light_level, 0.005);
    make_tone(reference_light_level, 0.005);
  } else {
    delay(10);
  }
  if (random(100) < 1000 && millis() - last_sound_toggle > 5) {
    sound_on = !sound_on;
    last_sound_toggle = millis();
  }
}

void loop() {
  read_sensors();
  do_servo_action();
  do_led_action();
  //make_sound();
}
