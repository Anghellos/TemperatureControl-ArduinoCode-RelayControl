//==============================Libraries=============================//

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

//==============================Set Pins===============================//

const int RELAY_PIN = 4;          // pin Realy is connected to

//============================Set Constants============================//

const int USER_TEMP = 28;
//const char* SSID = "FRITZ!Box 3490";    // WiFi-name
//const char* PASS = "04061984";          // WiFi-password
const char* SSID = "Free Wi-Fi";    // WiFi-name
const char* PASS = "jojolinul";          // WiFi-password

const int WIFI_DELAY = 15 * 1000;            // connect to Wifi delay 5 seconds (1 second = 1000 ms)
const int READ_DELAY = 15 * 1000;            // read temperature from ThinkSpeak MQTT delay (1 second = 1000 ms)

const char* READ_APIKEY = "AHVAOB5BO0DYLP90";     // read APIKey for ThingSpeak MQTT server
const int CHANNEL_NUM = 376531;                   // server channel on ThingSpeak MQTT server
const int READ_FIELD = 1;                         // field that temperature is stored on ThingSpeak MQTT Server

//============================Set Variables============================//

//============================Create Objects===========================//

WiFiClient  client;     // create client object for WiFi connection

//============================Initial Setup============================//

void setup(){
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // turn off realy at the beginning  for safety
  
  ThingSpeak.begin(client);     // ThingSpeak MQTT server setup
  WiFi.hostname("NodeMCU");     // set WiFi hostname
}

//===============================Main Loop==============================//

void loop(){
  if (!WiFiConnect()){
    Serial.print("Can't connect to WiFi. Retry in 15 seconds!");
    delay(WIFI_DELAY);      // delay if not connected to WiFi
    return;
  }
  
  float temp = ThingSpeak.readFloatField(CHANNEL_NUM, READ_FIELD, READ_APIKEY);    // load temperature from ThingSpeak MQTT server

  Serial.print("Temperature: ");  // temperature print for debugging purpose
  Serial.print(temp);             // temperature print for debugging purpose
  Serial.println(" °C");          // temperature print for debugging purpose

  ControlRelay(temp);
 
  delay(READ_DELAY);     // set delay
}

//=========================Connect WiFi Procedure=======================//

bool WiFiConnect(){
  if (WiFi.status() == WL_CONNECTED) return true;
  
  WiFi.begin(SSID, PASS);       // connect to WiFi with credencials
  return (WiFi.status() == WL_CONNECTED);
}

//=======================Control Radiator Procedure====================//

void ControlRelay(int temp){
  if (temp <= USER_TEMP){
    digitalWrite(RELAY_PIN, HIGH);  // turn on realy at the beginning  for safety
    Serial.println("Calorifer pornit");
  }else{
    digitalWrite(RELAY_PIN, LOW);   // turn off realy at the beginning  for safety
    Serial.println("Calorifer stins");
  }
}

