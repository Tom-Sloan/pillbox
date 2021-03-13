
void initSensorIC(int location)
{
  Adafruit_MCP23017 mcp;
  mcp.begin(IOEX_ADDR[location]);      
  for(int i = 0; i < 8; i++){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
  if(!isServo){
    mcp.pinMode(stp2, OUTPUT);
    pinMode(stp, OUTPUT);
    
    mcp.pinMode(MS1, OUTPUT);
    mcp.pinMode(MS2, OUTPUT);
    mcp.pinMode(EN, OUTPUT);
    mcp.pinMode(dirctn, OUTPUT);
    
    mcp.digitalWrite(stp2, LOW);
    digitalWrite(stp, LOW);
    
    mcp.digitalWrite(dirctn, LOW);
    mcp.digitalWrite(MS1, LOW);
    mcp.digitalWrite(MS2, LOW);
    mcp.digitalWrite(EN, HIGH);

  }else{
    myservo.attach(servoPin); 
  }
  
  ioex[location] = mcp;
  Serial.println("Done add");
}

void rowChange(int oldRowNum){
  if (oldRowNum<numRows){
    //add
    Serial.println("Pre add");
    for (int i = 0; i<numRows-oldRowNum;i++){
      Serial.println("adding");
      if (oldRowNum+i < sizeof(IOEX_ADDR)){
        initSensorIC(oldRowNum+i);
//        openAllRow(oldRowNum+i);
      }
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
