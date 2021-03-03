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
//#include <TCA9534.h>
#include "Adafruit_MCP23017.h"


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

//Player Sim Variables


// Motor Variables (IC and Motor)
const int stepsPerRevolution = 200; 
#define stp 8
#define dir 9

// Motor Sim Variables


// Sensor variables
//TCA9534 ioex[4];
Adafruit_MCP23017 ioex[4];
const int IOEX_ADDR = 0x27; //0x30; // A0 = A1 = A2 = 0
int numRows = 1;

// Sensor Sim Variables


//Time
//#define TIME_HEADER  "T"   // Header tag for serial time sync message
//#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//----------------------------------------END Variable Declaration----------------------------------------

//----------------------------------------Default Functions----------------------------------------
void setup(){
  Serial.begin(115200);
  Serial.println("\t Pillbox Serial");
  Serial.println("------------------------------\n");

  // Only wish to start ble once.
  Serial.println("--HEREA");
  initBLE();
  Serial.println("--HEREB");
  RTCInit(); //Note rtc.begin calls wire.begin
  Serial.println("--HERE");
  set_base_setup();
  Serial.println("---HERE2");
  initMotorIC();
  Serial.println("PLAYERSTART");
  playerInit();
  Serial.println("--HERE3");
  Serial.println(F("Playing track 002"));
//  featherPlayer.playFullFile("/track002.mp3");
  startAlarm("ALARM.mp3");
  
  Serial.println("--HERE4");
}

void loop(){
  
  if (bleuart.available()){
    //adjust Time
    if (bleuart.peek() == 'A') {
      bleuart.read();
      uint32_t unixTime = 0;
      int value = 0;
      while(bleuart.available()){
        value = bleuart.read() - 48;
        unixTime = unixTime * 10 + value;
      }
      unixTime = (unixTime - value)/ 10;
      Wire.beginTransmission(PCF8523_ADDRESS);
      if (Wire.endTransmission() == 0)
        rtc.adjust(DateTime(unixTime));
    
    } else if (bleuart.peek() == 'T'){ // Recieving an alarm time
      if (!dataUsed){
        bleuart.read();
        uint32_t unixTime = 0;
        int value = 0;
        while(bleuart.available()){
          value = bleuart.read() - 48;
          unixTime = unixTime * 10 + value;
        }
        unixTime = (unixTime - value)/ 10;
        String action = unixTime + "T"+ dataReceived;
  
        addToActions(action);      
      }
      
    } else if (bleuart.peek() == 'R'){ // Request data
       if (!dataUsed){
          while(bleuart.available())
            bleuart.read();
          sendEvents("BLE");
       }
    }    
    else { // Recieving an instruction
        dataReceived = "";
        Serial.println("------------------------------------------");
        while ( bleuart.available()  && bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R'){
          
            uint8_t ch;
            ch = (uint8_t) bleuart.read();
            dataUsed = false;
            
            Serial.write(ch);
            Serial.print("\t");
            Serial.println(ch);

            dataReceived += ch  - 48;
        }
      
        if (!dataUsed){
          Serial.println("------------------------------------------");
          
          // Print Data
          for (int i = 0; i <= dataReceived.length(); i++){
            char ch = dataReceived.charAt(i);
            Serial.println(ch);
          }

          // DO EVENT
          if(bleuart.peek() != 'T' && bleuart.peek() != 'A' && bleuart.peek() != 'R')
            processMessage(dataReceived);
       }
    }
  }
  
  if (Serial.available())
  {
    delay(2);
    if (Serial.find('R')){
      sendEvents("SERIAL");
    }
    while(Serial.available()){
      Serial.read();
    }
  }

  checkSensors();
  for (int i = 0; i< 10000; i++){
    if(i%2){
      
      ioex[0].digitalWrite(8, HIGH);
      delay(1);
    }else{
      ioex[0].digitalWrite(8, LOW);
      delay(1);
    }
  }
  Serial.println(rtc.now().unixtime());
  String t = "numRow ";
  t += numRows;
  Serial.println(t);
  delay(1000);
}

/*
 *   
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
 
