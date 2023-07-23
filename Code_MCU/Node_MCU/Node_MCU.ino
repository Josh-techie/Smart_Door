#include <ThingSpeak.h>               // add librery
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Galaxy A30"
#define STAPSK  "youssef30002"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
   // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  ThingSpeak.begin(Client);
}

void loop()
{
  String A = ThingSpeak.readStringField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  presentValue = A;
  if (presentValue != changeValue)
  {
    Serial.println(A);
    changeValue = presentValue;
    if(A == "1234")                   // Password
    {
      digitalWrite(LEDpin , HIGH);
    }
    else
    {
      digitalWrite(LEDpin , LOW);
    }
  }
}
