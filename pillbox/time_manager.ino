void RTCInit() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");

    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

bool compareTimes() {
  return true;
}




//void printDigits(int digits) {
//  // utility function for digital clock display: prints preceding colon and leading 0
//  Serial.print(":");
//  if (digits < 10)
//    Serial.print('0');
//  Serial.print(digits);
//}


//void digitalClockDisplay() {
//  // digital clock display of the time
//  Serial.print(hour());
//  printDigits(minute());
//  printDigits(second());
//  Serial.print(" ");
//  Serial.print(day());
//  Serial.print(" ");
//  Serial.print(month());
//  Serial.print(" ");
//  Serial.print(year());
//  Serial.println();
//}

//Send mesagase with seconds since jan 1 1970 as T1357041600 for Jan 1 2013
//void processSyncMessage(int unixTime) {
//  unsigned long pctime;
//  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
//  pctime = unixTime;
//
//  if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
//    setTime(pctime); // Sync Arduino clock to the time received on the serial port
//    Serial.print("Time set to: ");
//    Serial.println(pctime);
//  } else {
//    setTime(pctime); // Sync Arduino clock to the time received on the serial port
//    Serial.print("Time set to: Default");
//  }
//}

//time_t requestSync()
//{
//  Serial.write(TIME_REQUEST);
//  return 0; // the time will be sent later in response to serial mesg
//}
