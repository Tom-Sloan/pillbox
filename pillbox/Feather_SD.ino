//bool checkForFile(String dir_t, String toCheck)
//{
//  File dir = SD.open(dir_t);
//  bool isFound = false;
//  while(true) {
//
//     File entry =  dir.openNextFile();
//     if (! entry) {
//       // no more files
//       //Serial.println("**nomorefiles**");
//       break;
//     }
//     Serial.print(entry.name());
//     Serial.print("\t");
//     Serial.print(toCheck);
//     Serial.print("\t");
//     Serial.println(entry.name());
//     if (toCheck == entry.name())
//     {
//      isFound = true;
//     }
//
//
//     entry.close();
//   }
//   return isFound;
//}

bool saveData(String dir, String toCheck, String Data)
{
  bool isThere = SD.exists(toCheck);
  if (isThere)
  {
    Serial.println("IsthereSave");
  } else
  {
    Serial.println("notThereSave");
  }
}

String readData(String dir, String toCheck)
{
  bool isThere = SD.exists(toCheck);
  if (isThere)
  {
    String tmp = "TEMP";
    return tmp;
  }
  return "FAIL";
}

bool checkData(String toCheck)
{
  return toCheck == "FAIL" ;
}

void addToActions(String action) {
  File file_read = SD.open("action.txt", FILE_READ);

  // read lines from the file_read
  String prev = "";
  while (file_read.available()) {
    char line[16];
    file_read.read(line, 16);
    file_read.read(); //for EOL character
    String tmp = line;
    if (tmp > action)
      break;
    prev = line;
  }
  file_read.close();

  File file_write = SD.open("action.txt", FILE_WRITE);

  while (file_write.available()) {
    char line[16];
    file_write.read(line, 16);
    String tmp = line;
    if (tmp == prev) {
      file_write.println(action);
      file_write.close();
      Serial.println(action);
    }
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
  }
  String tmp = "Done Sending Data: ";
  tmp += comMethod;
  Serial.println(tmp);
  dataUsed = true; 
}


void set_base_setup() {
  Serial.println("BSU--HERE2");
  if (SD.exists("setup.txt")) {
    File file_read = SD.open("setup.txt", FILE_READ);
    char line[16];
    file_read.read(line, 16);
    file_read.read(); //for EOL character
    Serial.println(line);

    int rows = line[2] - '0';
    numRows = rows;
    file_read.close();
    dataUsed = true;
    Serial.println("BSU--HERE3");
    Serial.println(rows);

  } else {
    Serial.println("BSU--HERE4");
    File file_write = SD.open("setup.txt", FILE_WRITE);
    Serial.println("hello");
    String tmp = "00100T";
    tmp += rtc.now().unixtime();
    Serial.println(tmp);
    file_write.println(tmp);
    file_write.close();
    Serial.println("BSU--HERE5");
  }
  Serial.println("BSU--HERE6");
  rowChange(0);
}
