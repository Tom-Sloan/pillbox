void initSensorIC()
{
    TCA9534 ioex_temp;
    ioex_temp.attach(Wire);
    ioex_temp.setDeviceAddress(IOEX_ADDR);
    ioex_temp.config(TCA9534::Config::IN); // set all port to input
    ioex_temp.polarity(TCA9534::Polarity::INVERSE); // set all port polarity to inverse

    Serial.print("config  : ");
    Serial.println(ioex_temp.config(), HEX);
    Serial.print("polarity: ");
    Serial.println(ioex_temp.polarity(), HEX);

    int counter = 0;
//    try
//    {
//      for (byte i = 0; i < (sizeof(myValues) / sizeof(myValues[0])); i++) {
//        counter++;
//        
//      }
//    } catch
//    {
//       
//    }
//    
}


int checkSensors()
{
  Serial.println("asdf");
}
