boolean playerInit(void)
{
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     return false;
  }
  Serial.println(F("VS1053 found"));
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    return false;
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open(baseDir), 0);

  setVolume(10);
}

// Set volume for left, right channels. lower numbers == louder volume!
boolean setVolume(int volume)
{
  musicPlayer.setVolume(volume,volume);
}


void playerTest(void)
{
  musicPlayer.sineTest(0x44, 1000);
  delay(500);
  musicPlayer.sineTest(0x44, 1000);
  delay(500);
  musicPlayer.sineTest(0x44, 1000);
}

boolean startAlarm(String song)
{
  if (checkForFile(song))
  {
    musicPlayer.startPlayingFile(song);
  }else
  {
    playerTest();
  }
  
}

boolean checkForFile(File dir, String toCheck)
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

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
