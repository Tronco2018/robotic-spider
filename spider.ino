#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SERVOMAX 150
#define SERVOMIN 600

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

// SERVO
const short servoPiede = 0;
const short servoTibia = 1;

const short offsetPiede = 55;
const short offsetTibia = 60;

void setup() {
  // put your setup code here, to run once:
  pca.begin();
  pca.setPWMFreq(50);

  
  
  for (int angle = 150; angle >= 30; angle -= 5){
    uint16_t pulse = map(angle + offsetTibia, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(servoTibia, 0, pulse);
    pulse = map(angle + offsetPiede, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(servoPiede, 0, pulse);
    delay(70);
  }
  for (int angle = 30; angle <= 150; angle += 5){
    uint16_t pulse = map(angle + offsetTibia, 0, 180, SERVOMIN, SERVOMAX);
    
    pca.setPWM(servoTibia, 0, pulse);
    pulse = map(angle + offsetPiede, 0, 180, SERVOMIN, SERVOMAX);
    pca.setPWM(servoPiede, 0, pulse);
    delay(70);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
