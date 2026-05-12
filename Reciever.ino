#include <SoftwareSerial.h> //Create software serial object to communicate with SIM800L

SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

String inputString = "";         // a String to hold incoming data

 void setup()
{
 //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
 Serial.begin(300);
 
 //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);
  mySerial.println("AT"); //Check whether it has registered in the network
  //updateSerial();
  delay(3000);
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r");
  updateSerial();
  delay(3000);
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"mcinet\"\r");
  updateSerial();
  delay(3000);
  mySerial.println("AT+SAPBR=1,1");
  updateSerial();
  delay(3000);
  mySerial.println("AT+SAPBR=2,1");
  updateSerial();
  delay(3000);
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  delay(3000);
  mySerial.println("AT+HTTPPARA=\"CID\",1\r");
  updateSerial();
  delay(3000);
  //mySerial.println("AT+HTTPREAD");
  updateSerial(); 

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}


void loop()
{
  mySerial.println("AT+HTTPPARA=\"URL\",\"62.108.34.92:8000\"\r");
  updateSerial();
  delay(3000);
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"text/plain\"\r");
  updateSerial();
  delay(3000);
  mySerial.println("AT+HTTPDATA=1000,20000");
  updateSerial();
  delay(3000);

  if (Serial.available()) {
    // get the new byte:
    inputString = (char)Serial.read();
    Serial.print(inputString);
  }
  
  Serial.println(inputString);
  mySerial.println(inputString);
  delay(1000);
  
  mySerial.println("AT+HTTPACTION=1");
  delay(30000);
  updateSerial();
  mySerial.println("AT+HTTPREAD");
  delay(6000);
}




void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
