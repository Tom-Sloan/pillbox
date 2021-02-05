
#include <bluefruit.h>
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>


BLEService        hrms = BLEService(0x6666);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);
BLECharacteristic control = BLECharacteristic(0x0001); //0000 -> can't be used

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

bool canOutput = true;

// Button Variables will change:
int buttonState = 0;
float buttonPressedTimer = 0;
float recordingTime = .5;
bool buttonPressed = false;
bool canSendAgain = true;
bool hasCalibrated = false;
float timeBetween = 1.5;

double timeBetweenAccelRead = 0;
double lastReadingTime = 0;

bool wo_IMU = true; //set to false if imu is present

//Step counter variables
int stepCounter = 0;
float threshold = 1.00;

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 27;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;         // the number of the LED pin
const int scale = 3;

uint8_t  bps = 0;

// Advanced function prototypes
void startAdv(void);
void setupHRM(void);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  outputln("SmartBand Serial");
  outputln("-----------------------\n");

  // Initialise the Bluefruit module
  outputln("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  outputln("Setting Device Name to 'SmartBand'");
  Bluefruit.setName("Smartband");

  // Set the connect/disconnect callback handlers
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  outputln("Configuring the Device Information Service");
  bledis.setManufacturer("XTech");
  bledis.setModel("Universal 1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  outputln("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

  // Setup the Heart Rate Monitor service using
  // BLEService and BLECharacteristic classes
  outputln("Configuring the Heart Rate Monitor Service");
  setupHRM();

  // Setup the advertising packet(s)
  outputln("Setting up the advertising payload(s)");
  startAdv();

  outputln("\nAdvertising");
  outputln("Ending setup");
}

void loop()
{

  if ( Bluefruit.connected() ) {
    if (millis() - lastReadingTime > 10) {
      outputln("RUNNING");
//      if (!wo_IMU) {
//        recordData();
//        getMeans();
//        sendData_imu(control);
//        timeBetweenAccelRead = 0;
//        lastReadingTime = millis();
//      }else{
//        static int val = 0;
//        sendData(control, val);
//        outputln(String(val));
//        val +=1;
//        
//      }
    }


  }

}
