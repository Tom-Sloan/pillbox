void initSensorIC()
{
    Wire.begin();
    ioex.attach(Wire);
    ioex.setDeviceAddress(IOEX_ADDR);
    ioex.config(TCA9534::Config::IN); // set all port to input
    ioex.polarity(TCA9534::Polarity::INVERSE); // set all port polarity to inverse

    Serial.print("config  : ");
    Serial.println(ioex.config(), HEX);
    Serial.print("polarity: ");
    Serial.println(ioex.polarity(), HEX);
}


int readSensors()
{
  
}
