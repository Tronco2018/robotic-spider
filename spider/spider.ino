#include <Adafruit_PWMServoDriver.h>


#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

// ====== CONFIGURAZIONE ACCESS POINT ======
const char* ap_ssid = "ESP32-AP";
const char* ap_password = "12345678";   // deve essere almeno 8 caratteri

bool status = false;

const char* html = R"(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ragno: Pannello di controllo</title>
    <style>
    body{
    /*background-color: rgb(87, 87, 86);*/
    background: linear-gradient(180deg, rgb(17, 71, 233), rgb(125, 251, 255));
    background-repeat:no-repeat;
    background-attachment: fixed;
    background-size: cover;
    color: rgb(245, 245, 245);
    font-family: monospace;
    
}
#titleDiv{
    text-align: center;
}
#titleDiv h1{
    margin:1%;
    font-size: 4vh;
}
.grid{
    display: grid;
    width:100%;
    height:100%;
    
    grid-template-columns: repeat(auto-fit, minmax(min(150px, 100%), 1fr));
    grid-auto-rows: 1fr;

    gap: 1%;
}
.element{
    border-style: solid;
    border-color: rgba(0, 0, 0, 0.3);
    border-radius: 3px;
    text-align: center;
    background-color: rgba(0, 0, 0, 0.3);
}
.element p{
    font-size: 120%;
}
button{
    background-color: rgba(255, 255, 255, 0.7);
    border-radius: 10px;
    padding:1%;
    font-size: 110%;
    margin-bottom: 5%;
    border-style: solid;
    border-color: black;
    border-width: 3px;    
    transition-duration: 0.3s;
    width: 80%;
}
button:hover{
    background-color: rgba(255, 255, 255, 0.9);
    transition-duration: 0.3s;
}
button:active{
    transition-duration:0.3s;
    background-color: rgb(35, 255, 19);
}

    </style>
</head>
<body>
    <div id="titleDiv">
        <h1>Control panel</h1>
        <hr>
    </div>
    <div class="grid">
        <div class="element">
            <h1>Greet</h1>
            <form action="/greet" method="post">
            <button type="submit">Greet</button>
            </form>
        </div>
        <div class="element">
            <h1>Stand/Sit</h1>
            <form action="/sit" method="post">
            <button type="submit">Stand/Sit</button>
            </form>
        </div>
        <div class="element">
            <h1>Forward</h1>
            <form action="/for" method="post">
            <button type="submit">Go</button>
            </form>
        </div>
        <div class="element">
            <h1>Backwards</h1>
            <form action="/back" method="post">
            <button type="submit">Go</button>
            </form>
        </div>
        <div class="element">
            <h1>Status</h1>
            <p>Status: <span>Standing</span></p>
        </div>
    </div>
</body>
</html>

)";

WebServer server(80);

auto getHTML(){
  return html;
}

/*
==NOTES==
- Untested/unfinished functions: rise, down
*/


// == MOVING LOGIC ==

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
  setServoDegrees(leg[2], 35);
  Serial.println("XState: forward");
  resetLeg(leg);
}
void legBackwards(int leg[]){
  raiseLeg(leg);
  setServoDegrees(leg[2], -35);
  Serial.println("XState: backwards");
  resetLeg(leg);
}
void legNeutral(int leg[]){
  raiseLeg(leg);
  setServoDegrees(leg[2], 0);
  Serial.println("XState: neutral");
  resetLeg(leg);
}

// == END LEG FUNCTIONS ==

void rise() {
  // Range offsets
  setServoDegrees(gamba1[0], 60);
  setServoDegrees(gamba1[1], -20);

  setServoDegrees(gamba2[0], 60);
  setServoDegrees(gamba2[1], -20);

  setServoDegrees(gamba3[0], 60);
  setServoDegrees(gamba3[1], -20);

  setServoDegrees(gamba4[0], 60);
  setServoDegrees(gamba4[1], -20);
}

void down() {
  setServoDegrees(gamba1[0], 90);
  setServoDegrees(gamba1[1], 50);

  setServoDegrees(gamba2[0], 90);
  setServoDegrees(gamba2[1], 50);

  setServoDegrees(gamba3[0], 90);
  setServoDegrees(gamba3[1], 50);

  setServoDegrees(gamba4[0], 90);
  setServoDegrees(gamba4[1], 50);
}
void TESTdownSmooth(){
  for (int i = 0; i < 90; i++){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba2[0], i);
    setServoDegrees(gamba2[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba3[0], i);
    setServoDegrees(gamba3[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba4[0], i);
    setServoDegrees(gamba4[1], i >= 40 ? i-40 : 0);
    delay(10);
  }
}
void downSmooth(){
  for (int i = 90; i > -50; i--){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba2[0], i);
    setServoDegrees(gamba2[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba3[0], i);
    setServoDegrees(gamba3[1], i >= 40 ? i-40 : 0);

    setServoDegrees(gamba4[0], i);
    setServoDegrees(gamba4[1], i >= 40 ? i-40 : 0);
    delay(10);
  }
}


void riseSmooth(){
  for (int i = 90; i > -50; i--){
      if (i >= 0) setServoDegrees(gamba1[1], i-40);
      setServoDegrees(gamba1[0], i > 0 ? i : -i);
      delay(20);
  }
}
void resetFromRise(){
  for (int i = -50; i < 90; i++){
      if (i >= 0) setServoDegrees(gamba1[1], i-40);
      setServoDegrees(gamba1[0], i > 0 ? i : -i);
      delay(20);
  }
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
void saluto(int gamba[]){
  setServoDegrees(gamba[0], -15);
  setServoDegrees(gamba[1], 30);
}
void demo(){
  tpose();
  delay(2000);
  saluto(gamba1);
  saluto(gamba2);
  delay(1500);
  
  for (int i = -90; i < 90; i++){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i);
    delay(30);
  }
}
void demoSeria(){
  tpose();
  for (int i = 0; i > -60; i--){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i);
    delay(30);
  }
  for (int i = -60; i < 50; i++){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i);
    delay(20);
  }
  for (int i = 50; i > 0; i--){
    setServoDegrees(gamba1[0], i);
    setServoDegrees(gamba1[1], i);
    delay(20);
  }
}
void demoCamminata(){
  tpose();
  for (int i = 0; i < 60; i++){
    setServoDegrees(gamba1[1], i);
    if (i == 20){
      for (int j = 0; j > -20; j--){
        setServoDegrees(gamba1[0], -j);
        delay(20);
      }
    }
    
    delay(20);
  }
  tpose();
}

void setup() {
  // Begin di pca
  pca.begin();
  pca.setPWMFreq(50);

  Serial.begin(115200);

// ====== CREAZIONE ACCESS POINT ======
  Serial.println("Avvio Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("Access Point avviato: ");
  Serial.println(ap_ssid);

  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP());   // solitamente 192.168.4.1

  server.onNotFound([](){
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  server.on("/", [](){
    server.send(200, "text/html", getHTML());
  });
  server.on("/favicon.ico", []() {
    server.send(204); // 204 = No Content
  });
  Serial.println("Server web avviato.");

  // == ENDPOINTS ==
  /*server.on("/greet", [](){
    server.send(200, "text/html", getHTML());
  });
  server.on("/sit", [](){
      status = status ? false : true;
      status ? raise() : down();
  });*/


  // == SETUP ==
  downSmooth();
  riseSmooth();
  delay(2000);
  resetFromRise();

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

