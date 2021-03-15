
void initSensorIC(int location)
{
  Adafruit_MCP23017 mcp;
  mcp.begin(IOEX_ADDR[location]);      
  for(int i = 0; i < 8; i++){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
  if(!isServo){

    pinMode(stp, OUTPUT);
    
    mcp.pinMode(MS1, OUTPUT);
    mcp.pinMode(MS2, OUTPUT);
    mcp.pinMode(EN, OUTPUT);
    mcp.pinMode(slp, OUTPUT);
    mcp.pinMode(dirctn, OUTPUT);
    
    digitalWrite(stp, LOW);
    
    mcp.digitalWrite(dirctn, LOW);
    mcp.digitalWrite(MS1, LOW);
    mcp.digitalWrite(MS2, LOW);
    mcp.digitalWrite(EN, HIGH);
    mcp.digitalWrite(slp, LOW);

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
      String tmp = "index: ";
      tmp += i;
      tmp += "\t numRows: ";
      tmp += numRows;
      tmp += "\t oldrows: ";
      tmp += oldRowNum;
      Serial.println(tmp);
      if (oldRowNum+i < 4){
        initSensorIC(oldRowNum+i);
        openAllRow(oldRowNum+i);
      }
    }
  }
}

int checkSensors()
{
  for(int row = 0; row<numRows; row++){
    for(int i = 0; i < 7; i++){
        int stat = ioex[row].digitalRead(i);

        if (stat){
          digitalWrite(LED_BUILTIN, HIGH);
          delay(1000);
          digitalWrite(LED_BUILTIN, LOW);
          Serial.print("Location: ");
          Serial.print(row);
          Serial.print("\t");
          Serial.print(i);
          Serial.print("\t");
          Serial.println(stat);
          addToEvents(row, i);
        }
    }
  }
}
