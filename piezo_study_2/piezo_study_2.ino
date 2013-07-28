
int ledPin = 13;
int speakerOut = 10;               

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

const float one_second = 1000000.0;
float tempo = 120.0;

void setup() {
  pinMode(ledPin, OUTPUT); 
  pinMode(speakerOut, OUTPUT);
  Serial.begin(9600);
}

void make_tone(float f, float len) {
  float period = one_second / f;
  float duration = (60*one_second / tempo) * len;
  Serial.print("period="); Serial.println(period);
  Serial.print("duration="); Serial.println(duration);
  unsigned long d1 = period/2;
  unsigned long d2 = period - d1;
  unsigned long t_i = micros();
  unsigned long t_f = t_i + duration;
  for (; micros() < t_f;) {
    digitalWrite(speakerOut, HIGH);
    delayMicroseconds(d1);
    digitalWrite(speakerOut, LOW);
    delayMicroseconds(d2);
  }
}

void loop() {
  make_tone(c4, 2);
  make_tone(d4, 0.5);
  make_tone(e4, 0.5);
  make_tone(f4, 0.5);
  make_tone(g4, 0.5);
  make_tone(a4, 0.333333);
  make_tone(b4, 0.333333);
  make_tone(c5, 2.333333);
  delay(2000);
}
