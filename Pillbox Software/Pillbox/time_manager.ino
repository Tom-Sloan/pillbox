//Sets up the rtc device
void RTCInit() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  Serial.println("Found RTC");
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  Serial.println("Setup RTC done");
}
