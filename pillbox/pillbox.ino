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
const int stepsPerRevolution = 200;
int positions_stepper[4];
const int finalPositions[8] = {0, 250, 514, 771, 1029, 1286, 1543, 1800}; //Position 
#define stp2 13
#define stp 7
#define EN  8
#define dirctn 9
#define slp 10
#define MS1 11
#define MS2 12


int alarmTime = 0;
String alarmData = "";
bool moveOn = true;

// Sensor variables
//TCA9534 ioex[4];
Adafruit_MCP23017 ioex[4];
const int IOEX_ADDR[4] = {0x27, 0x26, 0x25, 0x24}; //0x30; // A0 = A1 = A2 = 0
int numRows = 1;

//Time
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//----------------------------------------END Variable Declaration----------------------------------------



//----------------------------------------Default Functions----------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\t Pillbox Serial");
  Serial.println("------------------------------\n");

  // Only wish to start ble once.
  Serial.println("-----Starting BT------");
  initBLE();
  Serial.println("-----Starting RTC------");
  RTCInit(); //Note rtc.begin calls wire.begin

  Serial.println("-----Initlizing Motors------");
  //  MotorInit(); //Note rtc.begin calls wire.begin
  Serial.println("-----Starting Player------");
  playerInit();
  Serial.println("-----SD and Volume Check------");
  //  featherPlayer.playFullFile("/track002.mp3");
  //  startAlarm("ALARM.mp3");
  Serial.println("-----Initializing Device------");
  set_base_setup();
  Serial.println("-----Setup DONE------");
  digitalWrite(stp, LOW);
}

void loop() {
  String t = "numRow1 ";
  t += numRows;
  Serial.println(t);
  digitalWrite(stp, LOW);
  if (bleuart.available()) {
    //adjust Time
    if (bleuart.peek() == 'A') {
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
        bleuart.read();
        uint32_t unixTime = 0;
        int value = 0;
        while (bleuart.available()) {
          value = bleuart.read() - 48;
          unixTime = unixTime * 10 + value;
        }
        unixTime = (unixTime - value) / 10;
        String action = unixTime + "T" + dataReceived;

        addToActions(action);
      }

    } else if (bleuart.peek() == 'R') { // Request data
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        sendEvents("BLE");
      }
    }
    else { // Recieving an instruction
      dataReceived = "";
      Serial.println("------------------------------------------");
      while ( bleuart.available()  && bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R') {

        uint8_t ch;
        ch = (uint8_t) bleuart.read();
        dataUsed = false;

        Serial.write(ch);
        Serial.print("\t");
        Serial.println(ch);

        dataReceived += ch  - 48;
      }

      if (!dataUsed) {
        Serial.println("------------------------------------------");

        // Print Data
        for (int i = 0; i <= dataReceived.length(); i++) {
          char ch = dataReceived.charAt(i);
          Serial.println(ch);
        }

        // DO EVENT
        if (bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R')
          processMessage(dataReceived);
      }
    }
  }
  t = "numRow2 ";
  t += numRows;
  Serial.println(t);
  if (Serial.available())
  {
    delay(2);
    if (Serial.peek()  == '2' || Serial.peek() == '3') {
      char user_input;
      user_input = Serial.read();
      Serial.print("User Input: ");
      Serial.println(user_input);

      if (user_input == '2')
      {
        Serial.println("GOING UP");
        StepForwardDefault();
      }
      else if (user_input == '3')
      {
        Serial.println("GOING DOWN");
        ReverseStepDefault();
      }

    } else if (Serial.find('R')) {
      sendEvents("SERIAL");
    }
    while (Serial.available()) {
      Serial.read();
    }
  }

  checkSensors();
  //  if (rtc.now().unixtime() - 1790 <= alarmTime && alarmTime <= rtc.now().unixtime() + 1790 ){
  //    if (!moveon){
  //      byte row = alarmData.charAt(3) - '0';
  //      byte coln = alarmData.charAt(4) - '0';
  //
  //      spotToOpen(row, coln);
  //      moveOn = true;
  //    }
  //  }else{
  //    if(moveOn){
  //      byte row = alarmData.charAt(3) - '0';
  //      lockedPosition(row);
  //      getNextAlarm();
  //      moveOn = false;
  //    }
  //  }
  t = "numRow ";
  t += numRows;
  Serial.println(t);
  delay(1000);
}

/*



  // Forward data from HW Serial to BLEUART
  while (Serial.available())
  {
    // Delay to wait for enough input, since we have a limited transmission buffer
    delay(2);

    uint8_t buf[64];
    int count = Serial.readBytes(buf, sizeof(buf));
    bleuart.write( buf, count );
  }

  // Forward from BLEUART to HW Serial
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    Serial.write(ch);
  }

  //  setSyncProvider( requestSync);  //set function to call when time sync required

  //      processSyncMessage(unixTime);
  //      if (timeStatus() == timeSet) {
  //        digitalWrite(13, HIGH); // LED on if synced
  //      } else {
  //        digitalWrite(13, LOW);  // LED off if needs refresh
  //      }
  //      bool toggle = false;
  //      while(!toggle){
  //          if (timeStatus()!= timeNotSet) {
  //            Serial.print("time set to: ");
  //            digitalClockDisplay();
  //            toggle = true;
  //          }
  //      }
*/
