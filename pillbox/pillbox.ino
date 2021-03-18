//----------------------------------------NOTES----------------------------------------
//1. must remove tx pin from feather connection
//2.

//----------------------------------------Libraries----------------------------------------
// For Bluetooth controls
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

// For Music player include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// For sensors
#include "Adafruit_MCP23017.h"

//For Motors
#include <Servo.h>

//For time
#include "RTClib.h"
//----------------------------------------Variable Declaration----------------------------------------

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

// BLE Sim Variables
bool dataUsed = true;
String dataReceived = "";

// Player (noise and sd card)
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

#if defined(ARDUINO_NRF52832_FEATHER )
#define VS1053_CS       30     // VS1053 chip select pin (output)
#define VS1053_DCS      11     // VS1053 Data/command select pin (output)
#define CARDCS          27     // Card chip select pin
#define VS1053_DREQ     31     // VS1053 Data request, ideally an Interrupt pin

// Feather M4, M0, 328, nRF52840 or 32u4
#else
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin
#endif

// SD card directory
String baseDir = "/";

// Music player object
Adafruit_VS1053_FilePlayer featherPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);


// Motor Variables (IC and Motor)
bool isServo = false;

//Servo
float positions_servo[8] = {0, 25.7, 51.4, 77.1, 102.9, 128.6, 154.3, 180.0};
Servo myservo;
#define servoPin 15

//Stepper
int positions_stepper[4];

int finalPositions_long[8] = {0, 815, 1630, 2445, 3260, 4075, 4890, 5705}; //Position 
int finalPositions_short[8] = {0, 228, 456, 684, 912,1140, 1368, 1605}; //Position 
int* finalPositions = finalPositions_short;

#define stp 7
#define EN  8
#define dirctn 9
#define slp 10
#define MS1 11
#define MS2 12


int alarmTime = 0;
String alarmData = "";
bool moveOn = true;
bool playedAlarm = true;

// Sensor variables
#define NUM_SENSORS 7

Adafruit_MCP23017 ioex[4];
const int IOEX_ADDR[4] = {0x27, 0x26, 0x25, 0x24};  // A0 = A1 = A2 = 0
int numRows = 1;
bool lastBtnStates[4][7] = {
  {false, false, false, false},
  {false, false, false, false},
  {false, false, false, false},
  {false, false, false, false}
};
bool currentBtnStates[4][7] = {
  {false, false, false, false},
  {false, false, false, false},
  {false, false, false, false},
  {false, false, false, false}
};


long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50; // the debounce time; increase if the output flickers

//Time
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//----------------------------------------END Variable Declaration----------------------------------------



//----------------------------------------Default Functions----------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\t Pillbox Serial");
  Serial.println("------------------------------\n");
  Serial.println("-----Starting Player------");
  playerInit();
  Serial.println("-----SD and Volume Check------");
  startAlarm(3);
  // Only wish to start ble once.
  Serial.println("-----Starting BT------");
  initBLE();
  Serial.println("-----Starting RTC------");
  RTCInit(); //Note rtc.begin calls wire.begin
  Serial.println("-----Initializing Device------");
  set_base_setup();
  Serial.println("-----Setup DONE------");
}

void loop() {
  Serial.print("Time");
  Serial.println(rtc.now().unixtime());
  if (bleuart.available()) {
    
    //adjust Time
    if (bleuart.peek() == 'A') {
      Serial.println("A Section");
      bleuart.read();
      uint32_t unixTime = 0; 
      int value = 0;
      while (bleuart.available()) {
        value = bleuart.read() - 48;
        unixTime = unixTime * 10 + value;
      }
      unixTime = (unixTime - value) / 10;
      rtc.adjust(DateTime(unixTime));

    } else if (bleuart.peek() == 'T') { // Recieving an alarm time
      if (!dataUsed) {
        char a = bleuart.read();
        Serial.println("T Section");
        uint32_t unixTime = 0;
        int value = 0;
        while (bleuart.available()) {
          int val = bleuart.read() - 48;
          if (val >=0 && val <= 9){
            value = val;
            unixTime = unixTime * 10 + value;
          }
        }

        String action =  "";
        action += dataReceived;
        action += "T";
        action += unixTime;
        Serial.println(action);
        addToActions(action);
      }

    } else if (bleuart.peek() == 'R') { // Request data
      if (!dataUsed) {
        Serial.println("R Section");
        while (bleuart.available()){bleuart.read();}
        sendEvents("BLE");
      }
    } else if (bleuart.peek() == 'U') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3)-'0';
        if (row < numRows){
          openAllRow(row);
        }
        Serial.print("U Section => Unlocked row: ");
        Serial.println(row);
      }
    }else if (bleuart.peek() == 'M') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3)-'0';
        byte coln = dataReceived.charAt(4) - '0';
        if (row < numRows){
          spotToOpen(row, coln);
        }
        Serial.print("M Section => Unlocked row: ");
        Serial.print(row);
        Serial.print("\t Coln: ");
        Serial.println(coln);
      }
    } else if (bleuart.peek() == 'L') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3)-'0';
        if (row < numRows){
          lockedPosition(row);
        }
        Serial.print("L Section => locked row: ");
        Serial.println(row);
      }
    }  else if (bleuart.peek() == 'P') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte alarm = dataReceived.charAt(1)-'0';
        startAlarm(alarm);
        Serial.print("P Section => Played alarm: ");
        Serial.println(alarm);
      }
    }  
    else { // Recieving an instruction
      dataReceived = "";
      Serial.println("Recieved Message");
      while ( bleuart.available()  && bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R' && bleuart.peek() != 'U' && bleuart.peek() != 'M' && bleuart.peek() != 'L' && bleuart.peek() != 'P') {

        uint8_t ch;
        ch = (uint8_t) bleuart.read();
        if (ch > 11){
          dataUsed = false;
          dataReceived += ch  - 48;
        }
      }

      if (!dataUsed) {
        Serial.println("Processing message");

        // DO EVENT
        if (bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R' && bleuart.peek() != 'U' && bleuart.peek() != 'M' && bleuart.peek() != 'L' && bleuart.peek() != 'P')
          processMessage(dataReceived);
      }
    }
  }

  if (Serial.available())
  {
    delay(2);
    if (Serial.peek()-'0'  >= 1 && Serial.peek()-'0' <= NUM_SENSORS) {
      char user_input;
      user_input = Serial.read();
      Serial.print("User Input: ");
      Serial.println(user_input - '0');
      Serial.println("GOING UP");
      spotToOpen(0, user_input - '0');

    } else if (Serial.peek() == 'B') {
      Serial.println("Setting Long");
      finalPositions = finalPositions_long; 
      for (int i = 0; i < sizeof(finalPositions); i++){
        Serial.println(finalPositions[i]);
      }
      
    } else if (Serial.peek() == 'S') {
      Serial.println("Setting Short");
      finalPositions = finalPositions_short;
      for (int i = 0; i < sizeof(finalPositions); i++){
        Serial.println(finalPositions[i]);
      } 
    } else if (Serial.find('R')) {
      sendEvents("SERIAL");
      
    } 
    while (Serial.available()) {
      Serial.read();
    }
  }

  checkSensors();
  if (rtc.now().unixtime() - 20 <= alarmTime && alarmTime <= rtc.now().unixtime() + 20 ){ //1790
    if (!moveOn){
      byte row = alarmData.charAt(3) - '0';
      byte coln = alarmData.charAt(4) - '0';
      
      Serial.print("Opening for alarm at: ");
      Serial.print(row);
      Serial.print("\t");
      Serial.println(coln);

      spotToOpen(row, coln);
      moveOn = true;
      playedAlarm = false;
    }
    
// Uncomment for music control
//    if (!playedAlarm && (rtc.now().unixtime() - 10 <= alarmTime && alarmTime <= rtc.now().unixtime() + 10)){
//      byte alarmNoise = alarmData.charAt(1) - '0';
//      startAlarm(alarmNoise);
//      playedAlarm = true;
//    }

  }else{
    if(moveOn){
      if (alarmData != ""){
        Serial.println("Starting Alarm Set");
        byte row = alarmData.charAt(3) - '0';
        Serial.println("Got row");
        lockedPosition(row); //Locking previous row
        Serial.println("setLocked Pos");
//        alarmTime = 0;//REMOVE
        getNextAlarm();
        Serial.println("got next alarm");
        Serial.println(alarmData);     
        moveOn = false;
        
      }
    }
  }
  delay(1000);
}
