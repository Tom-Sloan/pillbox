void addToActions(String action) {
  File actionFile = SD.open("actions.txt", FILE_WRITE);
  if (actionFile){
    actionFile.println(action);
    actionFile.close();
    Serial.println(action);
    actionFile.close(); 
    getNextAlarm();
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
    Serial.println("Old SBS");
    File file_read = SD.open("setup.txt", FILE_READ);
    
    char a = file_read.read();
    char b = file_read.read();
    char line = file_read.read();
    Serial.println(a);
    Serial.println(b);
    Serial.println(line);
    int rows = line - '0';
    numRows = rows;
    file_read.close();
    dataUsed = true;

  } else {
    Serial.println("New SBS");
    File file_write = SD.open("setup.txt", FILE_WRITE);
    
    String tmp = "00100T";
    tmp += rtc.now().unixtime();
    file_write.println(tmp);
    file_write.close();
  }
  Serial.println("Done: SBS");
  rowChange(0);
  getNextAlarm();
}

void getNextAlarm(){
  Serial.println("Setting next alarm");
  if (SD.exists("actions.txt")) {
    File file_read = SD.open("actions.txt", FILE_READ);
    String line_str ="";
    
    while (file_read.available()) {
      char rc = file_read.read();
      if (rc != '\n'){
        line_str += rc;
      }else{
        int subTime = line_str.substring(6, 17).toInt();
        Serial.print("Sub String Val: ");
        Serial.println(subTime);
        byte row = line_str.charAt(3) - '0';
        if ((alarmTime == 0 || alarmTime>subTime) && subTime >= rtc.now().unixtime() && row < numRows){
          alarmTime = subTime;
          alarmData = line_str;
          Serial.print("Next alarm has been changed to \t");
          Serial.println(alarmData);
        }
//        else if(alarmTime == 0 && subTime <= rtc.now().unixtime() && row < numRows){
//          subTime  =  rtc.now().unixtime() + 40 ;
//          int ran = random(0,7);
//          
//          line_str = "1010";
//          line_str += ran;
//          line_str += "T";
//          line_str += subTime;
//          alarmTime = subTime;
//          alarmData = line_str;
//          Serial.print("Next alarm has been changed to \t");
//          Serial.println(subTime);
//        }
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
