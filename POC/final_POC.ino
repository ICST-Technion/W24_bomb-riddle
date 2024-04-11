<<<<<<< HEAD
#include <WiFi.h>
#include <PubSubClient.h>
#include "LedController.hpp"  //timer
#include <cstdio>

// CONSTANTS
//------BUZZER------//
const int TONE_OUTPUT_PIN = 27; // piezo buzzer
const int TONE_PWM_CHANNEL = 0; // PWM channel

//------WIRES------//
const int numWires = 4;

const int RED_WIRE  = 18; //Red WIRE on pin 5
const int GREEN_WIRE = 19; //Green WIRE on pin 18
const int BLUE_WIRE  = 33; //Blue WIRE on Pin 19
const int YELLOW_WIRE  = 32; //Yellow WIRE on Pin 26

const int wirePins[numWires] = {RED_WIRE, GREEN_WIRE, BLUE_WIRE, YELLOW_WIRE};
const char* wireColors[numWires] = {"Red", "Green", "Blue", "Yellow"};
int lastState[numWires];
int wiresToCut[numWires] = {1, 2, 3, 4};
int curCutOrder[4];
int wiresCutCounter;
int correctCounter = 0;
int numConnected = 4;
char numConnectedArray[2]; // Make sure it's large enough to hold the converted string
//------LED------//
const int PIN_RED   = 14; //Red LED on pin 9
const int PIN_GREEN = 12; //Green LED on pin 10
const int PIN_BLUE  = 13; //Blue LED on Pin 11
//variables to hold our color intensities
int red;
int green;
int blue;

//------LED_TIMER------//
#define DIN 4
#define CS 16
#define CLK 17
LedController<1,1> lc;

//------WIFI------//
const char* ssid = "Dan";
const char* password = "12345678";
WiFiClient networkClient;

//------MQTT------//
const char* IPAddress = "172.20.10.13";
const int port = 1883;
char* deviceID = "100";
PubSubClient pc(IPAddress, port, networkClient);

//------GAME------//
enum State {Inactive, Active, Defused, Exploded};
State state = State::Inactive;
unsigned long detonationTime;
int seconds;
int minutes;
char timer[6];
int minGameDur = 2;
unsigned long delaytime=500;
bool played = false;


//------FUNCTIONS------//
void Activate(){
  state = State::Active;
  wiresCutCounter = 0;
  correctCounter = 0;
  setColor(0, 0, 255); //set LED to Blue
  detonationTime = millis() + (unsigned long)minGameDur*60*1000;
  Serial.println("\n Bomb activated!");
  played = false;
}

void ReActivate(){
  state = State::Active;
  setColor(0, 0, 255); //set LED to Blue
  wiresCutCounter = 0;
  correctCounter = 0;
  numConnected = 4;
  Serial.println("\n Bomb RE-Activated!");
  pc.publish("ToHost", "READY AGAIN");
  played = false;
  PlayReConnect();
}

void Deactivate(){
  state = State::Inactive;
  setColor(0, 255, 0); //set LED to Green
  Serial.println("\n Bomb defused! \n");
  pc.publish("ToHost", "WON");
}

void Detonate(){
  state = State::Exploded;
  setColor(255, 0, 0); //set LED to Red
  Serial.println("\n Bomb exploded! \n");
  pc.publish("ToHost", "BOOM");
}

void DisplayTimer(unsigned long timeRemaning){
  seconds = (timeRemaning / 1000)%60;
  minutes = timeRemaning / 60000;

  snprintf(timer, 6, "%02d:%02d", minutes, seconds);
  pc.publish("Timer", timer);      

  lc.setDigit(0,3,(byte)minutes/10,false);
  lc.setDigit(0,2,(byte)minutes%10,true);
  lc.setDigit(0,1,(byte)seconds/10,false);
  lc.setDigit(0,0,(byte)seconds%10,false);
}

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}

void PlayHint(int numCorrect) {
  for(int i=0; i<numCorrect; i++){
    ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
    delay(250);
    ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
    delay(250);
  }
}

void PlayReConnect(){
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_A, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_F, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_E, 4);
  delay(130);
  ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
}

void setup() {
  Serial.begin(9600);

  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);

  detonationTime = millis() + (unsigned long)minGameDur*60*1000;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  lc=LedController<1,1>(DIN,CLK,CS);
  lc.setIntensity(10); /* Set the brightness to a medium values */
  lc.clearMatrix(); /* and clear the display */

  for(int i=0; i<numWires; i++){
    pinMode(wirePins[i], INPUT_PULLUP);
    lastState[i] = digitalRead(wirePins[i]);
    Serial.print("WIRE: ");
    Serial.print(i);
    Serial.print("pin(");
    Serial.print(wirePins[i]);
    Serial.print(") :");
    numConnected -= digitalRead(wirePins[i]);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 1);
    delay(delaytime);
    digitalWrite(LED_BUILTIN, 0);
    delay(delaytime);
    Serial.print(".");
  } 
  Serial.println("WIFI STATUS!");
  Serial.println(WiFi.status());

  while(!pc.connected()){
    if(pc.connect(deviceID, "bomb", "bomb")){
      char topic[32];
      snprintf(topic, 32, "ToDevice/%s/#", deviceID);
      pc.subscribe("ToDevice");
      Serial.println(F("Connceted to MQTT server"));
      digitalWrite(LED_BUILTIN, 1);
      
      for(int i=0; i<numWires; i++){
        pc.publish(wireColors[i], digitalRead(wirePins[i])? "Disconnected" : "Connected");
      }
      itoa(numConnected, numConnectedArray, 10);
      Serial.print(numConnectedArray);
      pc.publish("ConnectedWires", numConnectedArray);
    }
    else{
      Serial.print(F("Could not connect to MQTT server, rc="));
      Serial.println(pc.state());
      digitalWrite(LED_BUILTIN, 0);
      delay(delaytime);
    }
  }

  pc.setCallback([](char* topic, byte* payload, unsigned int length){
    char msg[200];
    memcpy(msg, payload, length);
    msg[length] = '\0';
    Serial.print("Recieved over MQTT:");
    Serial.println(msg);

    if(strstr("RESET", msg) != NULL){
      Serial.print("\n Activateing \n");
      numConnected = 4;
      for(int i=0; i<numWires; i++){
        numConnected -= digitalRead(wirePins[i]);
      }
      Activate();
    }
    if(strstr("SOLVE", msg) != NULL){
      Serial.print("\n Deactivating \n");
      Deactivate();
    } 
    if(strstr("DELAY", msg) != NULL){
      pc.publish("GotDelay", "START");
      delay(2000);
      Serial.print("\n DELAYING TIMER \n");
      detonationTime += 30000;
      pc.publish("GotDelay", "END");
    } 
  });

  Activate();
};


void loop() {
  // WIFI loop for unstable connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WIFI");
    digitalWrite(LED_BUILTIN, 1);
    delay(delaytime);
    digitalWrite(LED_BUILTIN, 0);
    delay(delaytime);
    Serial.print(".");
    ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone just in case
  } 
  // MQTT loop for unstable connection
  while(!pc.connected()){
    if(pc.connect(deviceID, "bomb", "bomb")){
      char topic[32];
      snprintf(topic, 32, "ToDevice/%s/#", deviceID);
      pc.subscribe("ToDevice");
      Serial.println(F("Connceted to MQTT server in LOOP"));
      digitalWrite(LED_BUILTIN, 1);
      itoa(numConnected, numConnectedArray, 10);
      Serial.print(numConnectedArray);
      pc.publish("ConnectedWires", numConnectedArray);
      for(int i=0; i<numWires; i++){
        pc.publish(wireColors[i], digitalRead(wirePins[i])? "Disconnected" : "Connected");
      }
    }
    else{
      Serial.print(F("Could not connect to MQTT server, rc="));
      Serial.println(pc.state());
      digitalWrite(LED_BUILTIN, 0);
      ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone just in case
      delay(delaytime);
    }
  }

  //-HeartBeat and loop-//
  pc.publish("HB", "Connected");
  pc.loop();

  for(int i=0; i<numWires; i++){
    if(digitalRead(wirePins[i]) == 1 && lastState[i] == 0){   //wire i was cut!
      Serial.print(wireColors[i]);
      Serial.print("Wire: ");
      Serial.println(" cut \n");
      pc.publish(wireColors[i], "Disconnected");
      numConnected--;
      itoa(numConnected, numConnectedArray, 10);
      pc.publish("ConnectedWires", numConnectedArray);
      lastState[i] = 1;

      if(i+1 == wiresToCut[wiresCutCounter]){
        wiresCutCounter++;
        correctCounter++;
        Serial.print("\n correct wire was cut! \n");
        Serial.print(correctCounter);
        Serial.print(wiresCutCounter);

      }
      else{
        Serial.print("\n INcorrect wire!");
        wiresCutCounter++;
        Serial.print(wiresCutCounter);
      }    
    }
    else if(digitalRead(wirePins[i]) == 0 && lastState[i] == 1){  //wire i was recconected!
      Serial.print("Wire: ");
      Serial.print(i);
      Serial.println(" recconected");
       Serial.print(wiresCutCounter);
      pc.publish(wireColors[i], "Connected");
      numConnected++; 
      itoa(numConnected, numConnectedArray, 10);
      pc.publish("ConnectedWires", numConnectedArray);      
      lastState[i] = 0;
      // checking that all wires are reconnected to try again!
      bool allWiresConnected = true;
      for(int i=0; i<numWires; i++){
        if (lastState[i] != 0){
          allWiresConnected = false;
        }
      }
      if(allWiresConnected == true){
        ReActivate();
      }
    }
  }

  if(state == State::Active){
    for(int i=0; i<numWires; i++){
      if(digitalRead(wirePins[i]) == 1 && lastState[i] == 0){   //wire i was cut!
        Serial.print(wireColors[i]);
        Serial.print("Wire: ");
        Serial.println(" cut \n");
        pc.publish(wireColors[i], "Disconnected");
        numConnected--;
        itoa(numConnected, numConnectedArray, 10);
        pc.publish("ConnectedWires", numConnectedArray);
        lastState[i] = 1;

        if(i+1 == wiresToCut[wiresCutCounter]){
          wiresCutCounter++;
          correctCounter++;
          Serial.print("\n correct wire was cut! \n");
          Serial.print(correctCounter);
          Serial.print(wiresCutCounter);

        }
        else{
          Serial.print("\n INcorrect wire!");
          wiresCutCounter++;
          Serial.print(wiresCutCounter);
          // Detonate();
        }    
      }
      else if(digitalRead(wirePins[i]) == 0 && lastState[i] == 1){  //wire i was recconected!
        Serial.print("Wire: ");
        Serial.print(i);
        Serial.println(" recconected");
        Serial.print(wiresCutCounter);
        pc.publish(wireColors[i], "Connected");
        numConnected++; 
        itoa(numConnected, numConnectedArray, 10);
        pc.publish("ConnectedWires", numConnectedArray);      
        lastState[i] = 0;
        // checking that all wires are reconnected to try again!
        bool allWiresConnected = true;
        for(int i=0; i<numWires; i++){
          if (lastState[i] != 0){
            allWiresConnected = false;
          }
        }
        if(allWiresConnected == true){
          ReActivate();
        }
      }
    }
    unsigned long currentTime = millis();
    unsigned long timeRemaning = detonationTime - currentTime;

    Serial.println(wiresCutCounter);

    if(currentTime > detonationTime){
      Serial.print("\n GOING TO DETONATE!");
      Detonate();
    }
    else if(wiresCutCounter == numWires){
      Serial.print("\n ALL WIRES ARE CUT!");
      Serial.print(correctCounter);
      if (correctCounter == numWires){
        Deactivate();
      }
      else if (played == false){
        PlayHint(correctCounter);
        played = true;
      }
      DisplayTimer(timeRemaning);
    }
    else{
      DisplayTimer(timeRemaning); 
    }
    delay(delaytime);
  }
}
=======
#include <WiFi.h>
#include <PubSubClient.h>
#include "LedController.hpp"  //timer
#include <cstdio>

// CONSTANTS
//------BUZZER------//
const int TONE_OUTPUT_PIN = 27; // piezo buzzer
const int TONE_PWM_CHANNEL = 0; // PWM channel

//------WIRES------//
const int numWires = 4;

const int RED_WIRE  = 18; //Red WIRE on pin 5
const int GREEN_WIRE = 19; //Green WIRE on pin 18
const int BLUE_WIRE  = 33; //Blue WIRE on Pin 19
const int YELLOW_WIRE  = 32; //Yellow WIRE on Pin 26

const int wirePins[numWires] = {RED_WIRE, GREEN_WIRE, BLUE_WIRE, YELLOW_WIRE};
const char* wireColors[numWires] = {"Red", "Green", "Blue", "Yellow"};
int lastState[numWires];
int wiresToCut[numWires] = {1, 2, 3, 4};
int curCutOrder[4];
int wiresCutCounter;
int correctCounter = 0;
int numConnected = 4;
char numConnectedArray[2]; // Make sure it's large enough to hold the converted string
//------LED------//
const int PIN_RED   = 14; //Red LED on pin 9
const int PIN_GREEN = 12; //Green LED on pin 10
const int PIN_BLUE  = 13; //Blue LED on Pin 11
//variables to hold our color intensities
int red;
int green;
int blue;

//------LED_TIMER------//
#define DIN 4
#define CS 16
#define CLK 17
LedController<1,1> lc;

//------WIFI------//
const char* ssid = "Dan";
const char* password = "12345678";
WiFiClient networkClient;

//------MQTT------//
const char* IPAddress = "172.20.10.13";
const int port = 1883;
char* deviceID = "100";
PubSubClient pc(IPAddress, port, networkClient);

//------GAME------//
enum State {Inactive, Active, Defused, Exploded};
State state = State::Inactive;
unsigned long detonationTime;
int seconds;
int minutes;
char timer[6];
int minGameDur = 2;
unsigned long delaytime=500;
bool played = false;


//------FUNCTIONS------//
void Activate(){
  state = State::Active;
  wiresCutCounter = 0;
  correctCounter = 0;
  setColor(0, 0, 255); //set LED to Blue
  detonationTime = millis() + (unsigned long)minGameDur*60*1000;
  Serial.println("\n Bomb activated!");
  played = false;
}

void ReActivate(){
  state = State::Active;
  setColor(0, 0, 255); //set LED to Blue
  wiresCutCounter = 0;
  correctCounter = 0;
  numConnected = 4;
  Serial.println("\n Bomb RE-Activated!");
  pc.publish("ToHost", "READY AGAIN");
  played = false;
  PlayReConnect();
}

void Deactivate(){
  state = State::Inactive;
  setColor(0, 255, 0); //set LED to Green
  Serial.println("\n Bomb defused! \n");
  pc.publish("ToHost", "WON");
}

void Detonate(){
  state = State::Exploded;
  setColor(255, 0, 0); //set LED to Red
  Serial.println("\n Bomb exploded! \n");
  pc.publish("ToHost", "BOOM");
}

void DisplayTimer(unsigned long timeRemaning){
  seconds = (timeRemaning / 1000)%60;
  minutes = timeRemaning / 60000;

  snprintf(timer, 6, "%02d:%02d", minutes, seconds);
  pc.publish("Timer", timer);      

  lc.setDigit(0,3,(byte)minutes/10,false);
  lc.setDigit(0,2,(byte)minutes%10,true);
  lc.setDigit(0,1,(byte)seconds/10,false);
  lc.setDigit(0,0,(byte)seconds%10,false);
}

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}

void PlayHint(int numCorrect) {
  for(int i=0; i<numCorrect; i++){
    ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
    delay(250);
    ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
    delay(250);
  }
}

void PlayReConnect(){
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_A, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_F, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_E, 4);
  delay(130);
  ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
}

void setup() {
  Serial.begin(9600);

  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);

  detonationTime = millis() + (unsigned long)minGameDur*60*1000;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  lc=LedController<1,1>(DIN,CLK,CS);
  lc.setIntensity(10); /* Set the brightness to a medium values */
  lc.clearMatrix(); /* and clear the display */

  for(int i=0; i<numWires; i++){
    pinMode(wirePins[i], INPUT_PULLUP);
    lastState[i] = digitalRead(wirePins[i]);
    Serial.print("WIRE: ");
    Serial.print(i);
    Serial.print("pin(");
    Serial.print(wirePins[i]);
    Serial.print(") :");
    numConnected -= digitalRead(wirePins[i]);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 1);
    delay(delaytime);
    digitalWrite(LED_BUILTIN, 0);
    delay(delaytime);
    Serial.print(".");
  } 
  Serial.println("WIFI STATUS!");
  Serial.println(WiFi.status());

  while(!pc.connected()){
    if(pc.connect(deviceID, "bomb", "bomb")){
      char topic[32];
      snprintf(topic, 32, "ToDevice/%s/#", deviceID);
      pc.subscribe("ToDevice");
      Serial.println(F("Connceted to MQTT server"));
      digitalWrite(LED_BUILTIN, 1);
      
      for(int i=0; i<numWires; i++){
        pc.publish(wireColors[i], digitalRead(wirePins[i])? "Disconnected" : "Connected");
      }
      itoa(numConnected, numConnectedArray, 10);
      Serial.print(numConnectedArray);
      pc.publish("ConnectedWires", numConnectedArray);
    }
    else{
      Serial.print(F("Could not connect to MQTT server, rc="));
      Serial.println(pc.state());
      digitalWrite(LED_BUILTIN, 0);
      delay(delaytime);
    }
  }

  pc.setCallback([](char* topic, byte* payload, unsigned int length){
    char msg[200];
    memcpy(msg, payload, length);
    msg[length] = '\0';
    Serial.print("Recieved over MQTT:");
    Serial.println(msg);

    if(strstr("RESET", msg) != NULL){
      Serial.print("\n Activateing \n");
      numConnected = 4;
      for(int i=0; i<numWires; i++){
        numConnected -= digitalRead(wirePins[i]);
      }
      Activate();
    }
    if(strstr("SOLVE", msg) != NULL){
      Serial.print("\n Deactivating \n");
      Deactivate();
    } 
    if(strstr("DELAY", msg) != NULL){
      pc.publish("GotDelay", "START");
      delay(2000);
      Serial.print("\n DELAYING TIMER \n");
      detonationTime += 30000;
      pc.publish("GotDelay", "END");
    } 
  });

  Activate();
};


void loop() {
  // WIFI loop for unstable connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WIFI");
    digitalWrite(LED_BUILTIN, 1);
    delay(delaytime);
    digitalWrite(LED_BUILTIN, 0);
    delay(delaytime);
    Serial.print(".");
    ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone just in case
  } 
  // MQTT loop for unstable connection
  while(!pc.connected()){
    if(pc.connect(deviceID, "bomb", "bomb")){
      char topic[32];
      snprintf(topic, 32, "ToDevice/%s/#", deviceID);
      pc.subscribe("ToDevice");
      Serial.println(F("Connceted to MQTT server in LOOP"));
      digitalWrite(LED_BUILTIN, 1);
      itoa(numConnected, numConnectedArray, 10);
      Serial.print(numConnectedArray);
      pc.publish("ConnectedWires", numConnectedArray);
      for(int i=0; i<numWires; i++){
        pc.publish(wireColors[i], digitalRead(wirePins[i])? "Disconnected" : "Connected");
      }
    }
    else{
      Serial.print(F("Could not connect to MQTT server, rc="));
      Serial.println(pc.state());
      digitalWrite(LED_BUILTIN, 0);
      ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone just in case
      delay(delaytime);
    }
  }

  //-HeartBeat and loop-//
  pc.publish("HB", "Connected");
  pc.loop();

  for(int i=0; i<numWires; i++){
    if(digitalRead(wirePins[i]) == 1 && lastState[i] == 0){   //wire i was cut!
      Serial.print(wireColors[i]);
      Serial.print("Wire: ");
      Serial.println(" cut \n");
      pc.publish(wireColors[i], "Disconnected");
      numConnected--;
      itoa(numConnected, numConnectedArray, 10);
      pc.publish("ConnectedWires", numConnectedArray);
      lastState[i] = 1;

      if(i+1 == wiresToCut[wiresCutCounter]){
        wiresCutCounter++;
        correctCounter++;
        Serial.print("\n correct wire was cut! \n");
        Serial.print(correctCounter);
        Serial.print(wiresCutCounter);

      }
      else{
        Serial.print("\n INcorrect wire!");
        wiresCutCounter++;
        Serial.print(wiresCutCounter);
      }    
    }
    else if(digitalRead(wirePins[i]) == 0 && lastState[i] == 1){  //wire i was recconected!
      Serial.print("Wire: ");
      Serial.print(i);
      Serial.println(" recconected");
       Serial.print(wiresCutCounter);
      pc.publish(wireColors[i], "Connected");
      numConnected++; 
      itoa(numConnected, numConnectedArray, 10);
      pc.publish("ConnectedWires", numConnectedArray);      
      lastState[i] = 0;
      // checking that all wires are reconnected to try again!
      bool allWiresConnected = true;
      for(int i=0; i<numWires; i++){
        if (lastState[i] != 0){
          allWiresConnected = false;
        }
      }
      if(allWiresConnected == true){
        ReActivate();
      }
    }
  }

  if(state == State::Active){
    for(int i=0; i<numWires; i++){
      if(digitalRead(wirePins[i]) == 1 && lastState[i] == 0){   //wire i was cut!
        Serial.print(wireColors[i]);
        Serial.print("Wire: ");
        Serial.println(" cut \n");
        pc.publish(wireColors[i], "Disconnected");
        numConnected--;
        itoa(numConnected, numConnectedArray, 10);
        pc.publish("ConnectedWires", numConnectedArray);
        lastState[i] = 1;

        if(i+1 == wiresToCut[wiresCutCounter]){
          wiresCutCounter++;
          correctCounter++;
          Serial.print("\n correct wire was cut! \n");
          Serial.print(correctCounter);
          Serial.print(wiresCutCounter);

        }
        else{
          Serial.print("\n INcorrect wire!");
          wiresCutCounter++;
          Serial.print(wiresCutCounter);
          // Detonate();
        }    
      }
      else if(digitalRead(wirePins[i]) == 0 && lastState[i] == 1){  //wire i was recconected!
        Serial.print("Wire: ");
        Serial.print(i);
        Serial.println(" recconected");
        Serial.print(wiresCutCounter);
        pc.publish(wireColors[i], "Connected");
        numConnected++; 
        itoa(numConnected, numConnectedArray, 10);
        pc.publish("ConnectedWires", numConnectedArray);      
        lastState[i] = 0;
        // checking that all wires are reconnected to try again!
        bool allWiresConnected = true;
        for(int i=0; i<numWires; i++){
          if (lastState[i] != 0){
            allWiresConnected = false;
          }
        }
        if(allWiresConnected == true){
          ReActivate();
        }
      }
    }
    unsigned long currentTime = millis();
    unsigned long timeRemaning = detonationTime - currentTime;

    Serial.println(wiresCutCounter);

    if(currentTime > detonationTime){
      Serial.print("\n GOING TO DETONATE!");
      Detonate();
    }
    else if(wiresCutCounter == numWires){
      Serial.print("\n ALL WIRES ARE CUT!");
      Serial.print(correctCounter);
      if (correctCounter == numWires){
        Deactivate();
      }
      else if (played == false){
        PlayHint(correctCounter);
        played = true;
      }
      DisplayTimer(timeRemaning);
    }
    else{
      DisplayTimer(timeRemaning); 
    }
    delay(delaytime);
  }
}
>>>>>>> 21a185f (Before final presentation)
