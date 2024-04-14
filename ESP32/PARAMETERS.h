// CONSTANTS
//------BUZZER------//
const int TONE_OUTPUT_PIN = 27; // piezo buzzer
const int TONE_PWM_CHANNEL = 0; // PWM channel

//------WIRES------//
const int numWires = 4;

const int RED_WIRE  = 5; //Red WIRE on pin 5
const int BLUE_WIRE = 18; //Green WIRE on pin 18
const int GREEN_WIRE  = 19; //Blue WIRE on Pin 19
const int YELLOW_WIRE  = 21; //Yellow WIRE on Pin 21

const int wirePins[numWires] = {RED_WIRE, BLUE_WIRE, GREEN_WIRE, YELLOW_WIRE};
const char* wireColors[numWires] = {"Red", "Blue", "Green", "Yellow"};
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
