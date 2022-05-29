#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

static const int RXPin = 14, TXPin = 12;
static const int LRXPin = 13, LTXPin = 15; //d7, d8
static const uint32_t serialBaud = 115200;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial gpsSerial(RXPin, TXPin);

// The serial connection to the LoRa device
SoftwareSerial loraSerial(LRXPin, LTXPin);

String incoming = "";

void setup()
{
  Serial.begin(serialBaud);
  gpsSerial.begin(serialBaud);
  loraSerial.begin(serialBaud);

  delay(1000);

  writeDataToGPS("@GSTP");
  delay(100);
  writeDataToGPS("@GSOP 1 10000 5000");
  delay(100);
  writeDataToGPS("@GSR");
  delay(100);

  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    } else {
      sendLoraData("41.032532,21.339915|", 2);
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void writeDataToGPS(String data) 
{
  String content = "";
  content = data + "\r\n";
  char* bufc = (char*) malloc(sizeof(char) * content.length() + 1);
  content.toCharArray(bufc, content.length() + 1);
  gpsSerial.write(bufc);
  free(bufc);
}

void sendLoraData(String data, int address) {
  String myString = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data + "\r\n";
  char* buf = (char*) malloc(sizeof(char) * myString.length() + 1);
  Serial.println(myString);
  myString.toCharArray(buf, myString.length() + 1);
  loraSerial.write(buf);
  free(buf);
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    char data[20];
    sprintf(data, "%lf,%lf|", gps.location.lat(), gps.location.lng());
    sendLoraData(data, 2);
    //Serial.write(data);
    //Serial.flush();
    delay(100);
  }
}
