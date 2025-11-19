#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

/*
==NOTES==
- Untested/unfinished functions: rise, down
*/

// Motion range

#define SERVOMAX 150
#define SERVOMIN 550

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

// SERVO
int gamba1[] = {0, 1, 2};
int gamba2[] = {3, 4, 5};
int gamba3[] = {6, 7, 8};
int gamba4[] = {9, 10, 11};


// MOVING FUNCTIONS



void setServoDegrees(uint8_t channel, int degrees) {
  // Limita i gradi al range -90 / +90
  if (degrees > 90) degrees = 90;
  if (degrees < -90) degrees = -90;

  // Mappa i gradi in un valore PWM relativo al centro = 400
  // -90° → SERVOMIN
  //   0° → 400
  // +90° → SERVOMAX
  int pulse = map(degrees, -90, 90, SERVOMIN, SERVOMAX);

  pca.setPWM(channel, 0, pulse);
}

// == START LEG FUNCTIONS ==

void raiseLeg(int leg[]){
  setServoDegrees(leg[0], 50);
  setServoDegrees(leg[1], 0);
  Serial.println("YState: raised");
}

void resetLeg(int leg[]){
  setServoDegrees(leg[0], 60);
  setServoDegrees(leg[1], -20);
  Serial.println("YState: neutral");
}

void legForward(int leg[]){
  raiseLeg(leg);
  delay(500);
  setServoDegrees(leg[2], 35);
  delay(1000);
  Serial.println("XState: forward");
  resetLeg(leg);
}
void legBackwards(int leg[]){
  raiseLeg(leg);
  delay(500);
  setServoDegrees(leg[2], -35);
  Serial.println("XState: backwards");
  delay(1000);
  resetLeg(leg);
}
void legNeutral(int leg[]){
  raiseLeg(leg);
  delay(500);
  setServoDegrees(leg[2], 0);
  Serial.println("XState: neutral");
  delay(1000);
  resetLeg(leg);
}

// == END LEG FUNCTIONS ==

void rise() {
  // Range offsets
  setServoDegrees(gamba1[0], 60);
  setServoDegrees(gamba1[1], -20);
}

void down() {
  setServoDegrees(gamba1[0], 90);
  setServoDegrees(gamba1[1], 30);
}

void tpose(){
    legNeutral(gamba1);
    setServoDegrees(gamba1[0], 0);
    setServoDegrees(gamba1[1], 0);
    Serial.println("YState: T-POSE");

    setServoDegrees(gamba1[2], 0);
    Serial.println("XState: T-POSE");

    setServoDegrees(gamba2[0], 0);
    setServoDegrees(gamba2[1], 0);
    setServoDegrees(gamba2[2], 0);
}

void setup() {
  // Begin di pca
  pca.begin();
  pca.setPWMFreq(50);

  Serial.begin(9600);

  /*
  legNeutral(gamba1);
  delay(1000);
  legForward(gamba1);
  */
  legNeutral(gamba1);
  Serial.println("=============");
  delay(2000);
  legForward(gamba1);
  Serial.println("=============");
  delay(2000);
  tpose();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
