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

// For Motors 
#include <TCA9534.h>

//----------------------------------------Variable Declaration----------------------------------------

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

// BLE Sim Variables

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
const int M1_step = 27;
const int M2_step = 30;
const int M3_step = 15;
const int M_enable = 7;
const int M_dir = 31;
const int M_nsleep = 16;



// Motor Sim Variables

// Sensor variables

TCA9534 ioex[4];
const uint8_t IOEX_ADDR = 0x30; // A0 = A1 = A2 = 0


// Sensor Sim Variables
 
//----------------------------------------END Variable Declaration----------------------------------------

//----------------------------------------Default Functions----------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("\t Pillbox Serial");
  Serial.println("------------------------------\n");

  // Only wish to start ble once.
  
  Wire.begin();
  initMotorIC();
  initBLE();
  
}

void loop()
{
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    Serial.write(ch);
  }
  checkSensors();
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
  */
 
