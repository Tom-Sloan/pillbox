bool playerInit(void)
{
  if (! featherPlayer.begin()) { // initialise the music player
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
  printDirectory(baseDir, 0);

  setVolume(10);
}

// Set volume for left, right channels. lower numbers == louder volume!
bool setVolume(int volume)
{
  featherPlayer.setVolume(volume,volume);
}


void playerTest(void)
{
  featherPlayer.sineTest(0x44, 2000);
  setVolume(100);
  featherPlayer.sineTest(0x44, 2000);
  setVolume(10);
  featherPlayer.sineTest(0x44, 2000);
  setVolume(100);
  featherPlayer.sineTest(0x44, 2000);
  setVolume(10);
  featherPlayer.sineTest(0x44, 2000);
  setVolume(100);
  featherPlayer.sineTest(0x44, 2000);
  featherPlayer.stopPlaying();
}

bool startAlarm(String song)
{
  if (SD.exists(song))
  {
//    String t = "/" + song;
//    featherPlayer.startPlayingFile(song);
    Serial.println("IsthereSong");
  }else
  {
    playerTest();
  }
  
}


/// File listing helper
void printDirectory(String dir_t, int numTabs) 
{
   File dir = SD.open(dir_t);
   printDirectoryHelper(dir, numTabs);
}

void printDirectoryHelper(File dir, int numTabs) 
{
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
       printDirectoryHelper(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
