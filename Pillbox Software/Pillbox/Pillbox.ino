//----------------------------------------TODO----------------------------------------
//1. turn special characters into array
//2. give serial more programming ability
//3. change to interupts on slot pin detection
//4. add support for multiple rows to have a slot unlocked at once (max theorically is 1 per row)
//5. change alarm to non-blocking
//6. move row detection to loop if to it always runs not only when there is a non special character in byte 5 (byte 6 in report)
//7. change event recording to include the interval and alarm if there was one (currently records as 00)
//8. make the use of the variable dataUsed more consistant
//9. I removed the calls to the restart function (default function 1) for debugging perposes

//----------------------------------------Notes----------------------------------------
//1. Servo mode is not modular
//2. There are a lot of serial.print calls. These are not required

/* References
 * https://www.ti.com/lit/ds/symlink/drv8834.pdf?ts=1618235438904
 * https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
 * https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleuart
 * https://github.com/adafruit/Adafruit_VS1053_Library/blob/master/Adafruit_VS1053.h
 */

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
//This project was built to support a servo motor and a stepper, therefore use this var to toggle
bool isServo = false;

//Servo
float positions_servo[8] = {0, 25.7, 51.4, 77.1, 102.9, 128.6, 154.3, 180.0};
Servo myservo;
#define servoPin 15

//Stepper
int positions_stepper[4];

int finalPositions_long[8] = {0, 815, 1630, 2445, 3260, 4075, 4890, 5705}; //Position
int finalPositions_short[8] = {0, 228, 456, 684, 912, 1140, 1368, 1605}; //Position
int* finalPositions = finalPositions_short;

//Control variables for motor driver
#define stp 7 //on going high, motor moves 1 step
#define EN  8 //if off then the motor doesn't move
#define dirctn 9 //High goes 1 way, low another
#define slp 10 //if low the motor driver is off and uses much less power
#define MS1 11 //to control step size
#define MS2 12 // to control step size

//Alarm controling variables
int deltaTime = 1790; //Variable for control byte 1 (time to open in seconds)
int alarmTime = 0; // The time of the next alarm
String alarmData = ""; // Used to keep track of control bytes 0-5
bool moveOn = true; // Used to let the system know that the next alarm is known and to not look for another unless a message is recieved.
bool playedAlarm = true; // lets the system know if the slot has been unlocked

// Sensor variables
#define NUM_SENSORS 7 //bc i made a prototype with 3 sensors

//bc there is a max of 4 modules I hard coded this stuff, but if you make the arrays dynamic then the entire system will still work. I did set a hard coded limit on number of rows somewhere too (just search for a 4)
Adafruit_MCP23017 ioex[4]; //array that contains multiplexer objects
const int IOEX_ADDR[4] = {0x27, 0x26, 0x25, 0x24};  // possible addresses for the mux that this system supports
int numRows = 1; // current number of rows, used to see if there had been a change.

//contains the last btn states for the slots. See the btn debouncing arduino example for how it works.
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

//Time object for RTC
RTC_DS3231 rtc;

//----------------------------------------END Variable Declaration----------------------------------------



//----------------------------------------Default Functions----------------------------------------
//Reset function. Call it with 'resetFunc();' and the device restarts.
void(* resetFunc) (void) = 0;

void setup() {
  //Comments used to make debugging easier. You can remove to make interaction with desktop easier
  Serial.begin(115200);
  Serial.println("\t Pillbox Serial");
  Serial.println("------------------------------\n");
  Serial.println("-----Starting Player------");
  playerInit(); //Sets up music and sd card
  Serial.println("-----SD and Volume Check------");
  startAlarm(3); //test beep to show that its working. Can remove

  Serial.println("-----Starting RTC------");
  RTCInit(); //Sets up the rtc

  Serial.println("-----Starting BT------");
  initBLE(); //Sets up ble

  Serial.println("-----Initializing Device------");
  set_base_setup(); //Sets up the required defaults (like next alarm, number of modules etc)
  Serial.println("-----Setup DONE------");
}

void loop() {
  Serial.print("Time");
  Serial.println(rtc.now().unixtime());

  //For ble communication. The arduino checks to see if there is a message
  if (bleuart.available()) {

    //adjust Time
    if (bleuart.peek() == 'A') {
      Serial.println("A Section");
      bleuart.read(); // peek() doesn't remove the letter from the buffer so we remove it here
      uint32_t unixTime = 0;
      int value = 0;
      //gets the values from bytes 7-15 into the unix time stamp interger format. this doesn't use dataREcieved bc I wrote it first
      while (bleuart.available()) {
        value = bleuart.read() - 48;
        unixTime = unixTime * 10 + value;
      }
      unixTime = (unixTime - value) / 10;
      rtc.adjust(DateTime(unixTime));

      // Recieving an alarm time
    } else if (bleuart.peek() == 'T') {
      if (!dataUsed) {
        char a = bleuart.read();
        Serial.println("T Section");
        uint32_t unixTime = 0;
        int value = 0;
        while (bleuart.available()) {
          int val = bleuart.read() - 48;
          if (val >= 0 && val <= 9) {
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
      // Request data
    } else if (bleuart.peek() == 'R') {
      if (!dataUsed) {
        Serial.println("R Section");
        while (bleuart.available()) {
          bleuart.read();
        }
        sendEvents("BLE");
      }

      //Unlock a row
    } else if (bleuart.peek() == 'U') {
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3) - '0';
        if (row < numRows) {
          openAllRow(row);
        }
        Serial.print("U Section => Unlocked row: ");
        Serial.println(row);
      }

      //Unlock a slot
    } else if (bleuart.peek() == 'M') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3) - '0';
        byte coln = dataReceived.charAt(4) - '0';
        if (row < numRows) {
          spotToOpen(row, coln);
        }
        Serial.print("M Section => Unlocked row: ");
        Serial.print(row);
        Serial.print("\t Coln: ");
        Serial.println(coln);
      }

      //Lock a row
    } else if (bleuart.peek() == 'L') { // Recieving an alarm time
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte row = dataReceived.charAt(3) - '0';
        if (row < numRows) {
          lockedPosition(row);
        }
        Serial.print("L Section => locked row: ");
        Serial.println(row);
      }

      //Play an alarm
    }  else if (bleuart.peek() == 'P') {
      if (!dataUsed) {
        while (bleuart.available())
          bleuart.read();
        byte alarm = dataReceived.charAt(1) - '0';
        startAlarm(alarm);
        Serial.print("P Section => Played alarm: ");
        Serial.println(alarm);
      }
    }
    //
    else { // Recieving an instruction
      dataReceived = "";
      Serial.println("Recieved Message");
      while ( bleuart.available()  && bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R' && bleuart.peek() != 'U' && bleuart.peek() != 'M' && bleuart.peek() != 'L' && bleuart.peek() != 'P') {

        uint8_t ch;
        ch = (uint8_t) bleuart.read();
        if (ch > 11) {
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

  //For serial communication. The arduino checks to see if there is a message
  if (Serial.available())
  {
    delay(2);
    if (Serial.peek() - '0'  >= 1 && Serial.peek() - '0' <= NUM_SENSORS) {
      char user_input;
      user_input = Serial.read();
      Serial.print("User Input: ");
      Serial.println(user_input - '0');
      Serial.println("GOING UP");
      spotToOpen(1, user_input - '0');

    } else if (Serial.peek() == 'B') {
      Serial.println("Setting Long");
      finalPositions = finalPositions_long;
      for (int i = 0; i < sizeof(finalPositions); i++) {
        Serial.println(finalPositions[i]);
      }

    } else if (Serial.peek() == 'S') {
      Serial.println("Setting Short");
      finalPositions = finalPositions_short;
      for (int i = 0; i < sizeof(finalPositions); i++) {
        Serial.println(finalPositions[i]);
      }
    } else if (Serial.find('R')) {
      sendEvents("SERIAL");

    }
    while (Serial.available()) {
      Serial.read();
    }
  }

  //Checks to see if the a slot has been opened
  checkSensors();

  //Checks to see which/if slot should be unlocked
  if (rtc.now().unixtime() - deltaTime <= alarmTime && alarmTime <= rtc.now().unixtime() + deltaTime ) { //1790
    if (!moveOn) {
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
    //play alarm
    if (!playedAlarm && (rtc.now().unixtime() - 10 <= alarmTime && alarmTime <= rtc.now().unixtime() + 10)) {
      byte alarmNoise = alarmData.charAt(1) - '0';
      startAlarm(alarmNoise);
      playedAlarm = true;
    }

  } else {
    //used to make the system only look for an alarm if the next one is not known
    if (moveOn) {
      //used so if there is no alarm in actions.txt it doesn't go through this check needlessly
      if (alarmData != "") {
        Serial.println("Starting Alarm Set");
        byte row = alarmData.charAt(3) - '0';
        Serial.println("Got row");
        lockedPosition(row); //Locking previous row
        Serial.println("setLocked Pos");
        getNextAlarm();
        Serial.println("got next alarm");
        Serial.println(alarmData);
        moveOn = false;

      }
    }
  }
}
