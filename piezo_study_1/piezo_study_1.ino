// http://www.arduino.cc/en/Tutorial/PlayMelody
/* Play Melody
 * -----------
 *
 * Program to play melodies stored in an array, it requires to know
 * about timing issues and about how to play tones.
 *
 * The calculation of the tones is made following the mathematical
 * operation:
 *
 *       timeHigh = 1/(2 * toneFrequency) = period / 2
 *
 * where the different tones are described as in the table:
 *
 * note         frequency       period  PW (timeHigh)   
 * c            261 Hz          3830    1915    
 * d            294 Hz          3400    1700    
 * e            329 Hz          3038    1519    
 * f            349 Hz          2864    1432    
 * g            392 Hz          2550    1275    
 * a            440 Hz          2272    1136    
 * b            493 Hz          2028    1014    
 * C            523 Hz          1912    956
 *
 * (cleft) 2005 D. Cuartielles for K3
 */

int ledPin = 13;
int speakerOut = 10;               

void setup() {
 pinMode(ledPin, OUTPUT); 
 pinMode(speakerOut, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  float one_second = 1000000.0;
  float f = 440;
  float period = one_second / f;
  float tempo = 60;
  float duration = one_second / tempo;
  int vol = 255;
  Serial.print("period="); Serial.println(period);
  Serial.print("duration="); Serial.println(duration);
  for (float i = 0; i < duration; i += (float)((long)period)) {
    digitalWrite(speakerOut, HIGH);
    delayMicroseconds(period/2);
    digitalWrite(speakerOut, LOW);
    delayMicroseconds(period/2);
  }
  delay(2000);
}
