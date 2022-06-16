#include <SoftwareSerial.h>

#define RXD2 14
#define TXD2 12

SoftwareSerial mySerial(RXD2, TXD2);

String content = "";

void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {

  if (mySerial.available()) {
    String incomming = mySerial.readString();
    if (incomming.length() <= 10)
      Serial.println(incomming);
    else {
      //Serial.println(incomming);
      String channel_ID = incomming.substring(incomming.indexOf('=') + 1, incomming.indexOf(','));
//      Serial.println("Channel ID : " + channel_ID);

      String str = incomming.substring(incomming.indexOf(',') + 1);

      String msgLength = str.substring(0, str.indexOf(','));
//      Serial.println("Message Length : " + msgLength);

      String str2 = str.substring(str.indexOf(',') + 1);

      String message = str2.substring(0, msgLength.toInt());
      char buf[21];
      message.toCharArray(buf, 21)
      Serial.write(buf);
      serial.flush();
      free(buf);

    }
  }
}
