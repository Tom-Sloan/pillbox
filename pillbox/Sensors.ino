
void initSensorIC(int location)
{
  Adafruit_MCP23017 mcp;
  mcp.begin(IOEX_ADDR-location+1);      // use default address 0x20
  for(int i = 0; i < 8; i++){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
  mcp.pinMode(stp, OUTPUT);
  mcp.pinMode(dirctn, OUTPUT);
  mcp.digitalWrite(stp, LOW);
  mcp.digitalWrite(dirctn, LOW);
  
  ioex[location] = mcp;
  Serial.println("Done add");
}

void rowChange(int oldRowNum){
  if (oldRowNum<numRows){
    //add
    Serial.println("Pre add");
    for (int i = 0; i<numRows-oldRowNum;i++){
      Serial.println("adding");
      initSensorIC(oldRowNum+i);
    }
  }
}

int checkSensors()
{
  for(int row = 0; row<numRows; row++){
    for(int i = 0; i < 10; i++){
        int stat = ioex[row].digitalRead(i);
        digitalWrite(LED_BUILTIN, stat);
        Serial.print("Location: ");
        Serial.print(row);
        Serial.print("\t");
        Serial.print(i);
        Serial.print("\t");
        Serial.println(stat);
//        if (stat){
//          addToEvents(row, i);
//        }
    }
  }
}

/*
 * 
 * 
 *     int counter = 0;
//    try
//    {
//      for (byte i = 0; i < (sizeof(myValues) / sizeof(myValues[0])); i++) {
//        counter++;
//        
//      }
//    } catch
//    {
//       
//    }
//    */


//void initSensorIC(int location)
//{
//    TCA9534 ioex_temp;
//    ioex_temp.attach(Wire);
//    ioex_temp.setDeviceAddress(IOEX_ADDR);
//    ioex_temp.config(TCA9534::Config::IN); // set all port to input
//    ioex_temp.polarity(TCA9534::Polarity::ORIGINAL); // set all port polarity to inverse
//
//    Serial.print("config  : ");
//    Serial.println(ioex_temp.config(), HEX);
//    Serial.print("polarity: ");
//    Serial.println(ioex_temp.polarity(), HEX);
//    ioex[location] = ioex_temp;
//
//}
