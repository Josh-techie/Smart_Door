#include <ThingSpeak.h>               // add librery
# include<ESP8266WiFi.h>
#include <SoftwareSerial.h>



WiFiClient  client;
unsigned long counterChannelNumber = 1733887;                // Channel ID
const char * myCounterReadAPIKey = "6SPQMPCA6KG8KOQR";      // Read API Key
const int FieldNumber1 = 1;    
   
//String presentValue = " ";
//String changeValue = " ";

// The field you wish to read
 uint8_t LEDpin = D6;

void setup()
{
  // servo.attach(D9);
  
  pinMode(LEDpin, OUTPUT);

  Serial.begin(9600);
 

  WiFi.begin("Galaxy A30", "youssef30002");                 // write wifi name & password
delay(2000);
  
  /*while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
   // Serial.print(".");
  }*/
 // Serial.println();
 // Serial.print("Connected, IP address: ");
 //  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}
void loop()
{
  
  String A = ThingSpeak.readStringField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  Serial.println(A);
/*  presentValue = A;
  if (presentValue != changeValue)
  {
    Serial.println(A);
    changeValue = presentValue;
    if(A == "1")                   // Password
    {
      digitalWrite(LEDpin , HIGH);
    
    }
    else if(A == "0")
    {
      digitalWrite(LEDpin , LOW);
  
    }
  }*/
}

