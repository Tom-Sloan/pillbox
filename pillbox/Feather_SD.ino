bool checkForFile(File dir, String toCheck)
{
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
     Serial.println(strcmp(toCheck, entry.name()));
     if (strcmp(toCheck, entry.name()))
     {
      isFound = true;
     }
     
     
     entry.close();
   }
   return isFound;
}

bool saveData(File dir, String toCheck, String Data)
{
  bool isThere = checkForFile(File dir, String toCheck);
  if (isThere)
  {
    
  } else 
  {
    
  }
}

String readData(File dir, String toCheck)
{
  bool isThere = checkForFile(File dir, String toCheck);
  if (isThere)
  {
    String tmp = "TEMP";
    return tmp;
  }
  return "FAIL";
}

bool checkData(String toCheck)
{
  return strcmp(toCheck,"FAIL") ;
}
