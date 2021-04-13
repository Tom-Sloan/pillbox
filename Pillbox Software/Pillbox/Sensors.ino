//Sets up the sensors and the motor driver/servo
void initSensorIC(int location)
{
  
  Adafruit_MCP23017 mcp;

  // the breakout boards bought on amazon have a bug, remove this if not using them. 
  // or maybe its the adafruit library. Anyways this lets you use multiple modules
  if(location  != 1){
    mcp.begin(IOEX_ADDR[location]);      
  }else{
    mcp.begin();  
  }

  // sets up the pullups so i2c works, you can probably remove this if 
  for(int i = 0; i < NUM_SENSORS; i++){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }
  
  //motor driver 
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

  //Servo 
  }else{
    myservo.attach(servoPin); 
  }

  //Adding sensors
  for(int i = 0; i < NUM_SENSORS; i++){
    lastBtnStates[location][i] = mcp.digitalRead(i);
    currentBtnStates[location][i] = mcp.digitalRead(i);
  }
  
  ioex[location] = mcp;
  Serial.println("Done add");
}

// for when the number of module changes
// calls to to initlize the new sensors and motor driver and sets the row to unlock 
void rowChange(int oldRowNum){

  if (oldRowNum<numRows){
    
    // add 
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
        Serial.println("Here");
        openAllRow(oldRowNum+i);
      }
    }
  }
}

// Checks the sensors for a slot unlocking event, debouncing method
void checkSensors()
{
  bool changedState = false;
  for(int row = 0; row<numRows; row++){
    for(int i = 0; i < NUM_SENSORS; i++){

      int stat = ioex[row].digitalRead(i);
      Serial.print("Location: ");
      Serial.print(row);
      Serial.print("\t");
      Serial.print(i);
      Serial.print("\t");
      Serial.println(stat);
      if (lastBtnStates[row][i] != stat){
        lastDebounceTime = millis();
      }
      lastBtnStates[row][i] = stat;
      
      if ((millis() - lastDebounceTime) < debounceDelay) {
        return;
      }

      if(currentBtnStates[row][i] != stat){
        currentBtnStates[row][i] = stat;
        if (!stat){
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
}
