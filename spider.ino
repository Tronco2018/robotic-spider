#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// Motion range

#define SERVOMAX 150
#define SERVOMIN 600

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

// SERVO
const short gamba1[] = {0, 1, 2};
const short gamba2[] = {3, 4, 5};
const short gamba3[] = {6, 7, 8};
const short gamba4[] = {9, 10, 11};


// MOVING FUNCTIONS

void rise() {
  // Range offsets
  const short offsetPiede = 55;
  const short offsetTibia = 60;

  for (int angle = 150; angle >= 30; angle -= 5){
    // Tibia
    uint16_t pulse = map(angle + offsetTibia, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(gamba1[1], 0, pulse);
    pca.setPWM(gamba2[1], 0, pulse);
    pca.setPWM(gamba3[1], 0, pulse);
    pca.setPWM(gamba4[1], 0, pulse);

    // Piede
    pulse = map(angle + offsetPiede, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(gamba1[0], 0, pulse);
    pca.setPWM(gamba2[0], 0, pulse);
    pca.setPWM(gamba3[0], 0, pulse);
    pca.setPWM(gamba4[0], 0, pulse);

    delay(20);
  }
}

void down() { //IMPORTANT: motore piede fa extra movimento
  // Range offsets
  const short offsetPiede = 55;
  const short offsetTibia = 60;

  for (int angle = 30; angle <= 150; angle += 5){
    // Tibia
    uint16_t pulse = map(angle + offsetTibia, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(gamba1[1], 0, pulse);
    pca.setPWM(gamba2[1], 0, pulse);
    pca.setPWM(gamba3[1], 0, pulse);
    pca.setPWM(gamba4[1], 0, pulse);

    // Piede
    pulse = map(angle + offsetPiede, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(gamba1[0], 0, pulse);
    pca.setPWM(gamba2[0], 0, pulse);
    pca.setPWM(gamba3[0], 0, pulse);
    pca.setPWM(gamba4[0], 0, pulse);

    delay(20);
  }
}

void setup() {
  // Begin di pca
  pca.begin();
  pca.setPWMFreq(50);

  rise();
  delay(100);
  down();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
