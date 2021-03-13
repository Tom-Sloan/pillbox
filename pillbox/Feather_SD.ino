void addToActions(String action) {
  File actionFile = SD.open("action.txt", FILE_READ);
  if (actionFile){
    actionFile.println(action);
    actionFile.close();
    Serial.println(action);
    actionFile.close(); 
  }
}
void addToEvents(int row, int column) {
  Serial.print("Datafile: ");
  File dataFile = SD.open("events.txt", FILE_WRITE);
  Serial.println(dataFile);
  if (dataFile) {
    String data = "00";
    data += numRows;
    data += row;
    data += column;
    data += "T";
    data += rtc.now().unixtime();
    dataFile.println(data);
    dataFile.close();
    // print to the serial port too:
    Serial.println(data);
  }
}

void sendEvents(String comMethod) {
  Serial.println("Sending Data");
  if (SD.exists("events.txt")) {
    File file_read = SD.open("events.txt", FILE_READ);
    char line[17];
    String line_str ="";
    byte indx = 0;
    while (file_read.available()) {
      
      char rc = file_read.read();
      
      if (rc != '\n'){
        line[indx] = rc;
        indx += 1;
        line_str += rc;
      }else{
        if (comMethod == "BLE"){
          bleuart.write(line, 17);
        } else if (comMethod == "SERIAL"){
          Serial.println(line);
        }
        indx = 0;
        line_str = "";
      }
    }
    SD.remove("events.txt");
    file_read.close();
  }else{
    Serial.println("----------------NO EVENTS.txt--------------------------");
  }
  String tmp = "Done Sending Data: ";
  tmp += comMethod;
  Serial.println(tmp);
  dataUsed = true; 
}


void set_base_setup() {
  
  if (SD.exists("setup.txt")) {
    File file_read = SD.open("setup.txt", FILE_READ);
    char line[16];
    file_read.read(line, 16);
    file_read.read(); //for EOL character

    int rows = line[2] - '0';
    numRows = rows;
    file_read.close();
    dataUsed = true;

  } else {
    File file_write = SD.open("setup.txt", FILE_WRITE);
    
    String tmp = "00100T";
    tmp += rtc.now().unixtime();
    file_write.println(tmp);
    file_write.close();
  }
  rowChange(0);
}

void getNextAlarm(){
  Serial.println("Setting next alarm");
  if (SD.exists("actions.txt")) {
    File file_read = SD.open("alarms.txt", FILE_READ);
    
    String line_str ="";
    
    while (file_read.available()) {
      char rc = file_read.read();
      
      if (rc != '\n'){
        line_str += rc;
      }else{
        int subTime = line_str.substring(6, 17).toInt();
        Serial.print("Sub String Val: ");
        Serial.println(subTime);
        
        if (alarmTime == 0 || alarmTime>subTime){
          alarmTime = subTime;
          alarmData = line_str;
          Serial.print("Next alarm has been changed to \t");
          Serial.println(alarmData);
        }
        line_str = "";
      }
    }
    file_read.close();
   
  }else{
    Serial.println("----------------NO actions.txt--------------------------");
  }
  String tmp = "Done alarming device: ";
  tmp += alarmData;
  Serial.println(tmp);
  dataUsed = true; 
}
