bool checkForFile(String dir_t, String toCheck)
{
  File dir = SD.open(dir_t);
  isFound = false;
  while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     Serial.print(entry.name());
     Serial.print("\t");
     Serial.print(toCheck);
     Serial.println(toCheck, entry.name());
     if (toCheck == entry.name())
     {
      isFound = true;
     }
     
     
     entry.close();
   }
   return isFound;
}

bool saveData(String dir, String toCheck, String Data)
{
  bool isThere = checkForFile(dir, toCheck);
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
  bool isThere = checkForFile(dir, toCheck);
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
