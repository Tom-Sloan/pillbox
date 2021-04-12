 // --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>
#include "Adafruit_MCP23017.h"
const int IOEX_ADDR[4] = {0x27, 0x20, 0x25, 0x24}; //0x30; // A0 = A1 = A2 = 0
Adafruit_MCP23017 mcp;
Adafruit_MCP23017 mcp_2;
void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");

  mcp.begin(IOEX_ADDR[0]);   
  for(int i = 0; i < 8; i++){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
  mcp_2.begin();   
  for(int i = 0; i < 8; i++){
    mcp_2.pinMode(i, INPUT);
    mcp_2.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

   

  for(int i = 0; i < 8; i++){
    int stat = mcp.digitalRead(i);
    Serial.print("Location: ");
    Serial.print(i);
    Serial.print("\t");
    Serial.println(stat);
  }
  for(int i = 0; i < 8; i++){
    int stat = mcp_2.digitalRead(i);
    Serial.print("Location: ");
    Serial.print(i);
    Serial.print("\t");
    Serial.println(stat);
  }
  delay(1000);           // wait 5 seconds for next scan
}
