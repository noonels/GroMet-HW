#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define SIZE (255)

const int HUMID_PIN  = 0;
const int LIGHT_PIN  = 0;
const char* ssid     = "GroMet Network";
const char* passwd   = "wallace";
//const IPAddress host = "GroMetHub.local";   //IPAdress type cannot be String
const IPAddress host = (192, 168, 0, 1);
const int    port    = 80;
const String ID      = WiFi.macAddress();

char* data;

WiFiUDP udp;
byte serdata    = 0;
byte fromserver = 0;
char json[SIZE];
//String json = "";

void setup() {
  pinMode(HUMID_PIN, INPUT_PULLUP);
  pinMode(LIGHT_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  delay(10); // give serial time to begin working

  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  char buf[ID.length()];
  ID.toCharArray(buf, ID.length());   //convert ID from String to char*
  data = buf;                         //since udp.write can't take String
}

void loop() {
  //Method using Strings
  /*String humidity = String(analogRead(HUMID_PIN));
  String light    = String(analogRead(LIGHT_PIN));
  json += "{\"ID\":";
  json += ID;
  json += ",\"humidity\":";
  json += humidity;
  json += ",\"lightExposure\":";
  json += light;
  json += "}";

  char buf[json.length()];
  json.toCharArray(buf, json.length());
  char* data = buf;*/

  //Method using char*
  int a = analogRead(HUMID_PIN);
  char* humidity;
  itoa(a, humidity, 10);  //convert int to char*, base 10
  a = analogRead(LIGHT_PIN);
  char* light;
  itoa(a, light, 10);
  
  strcat(json, "{\"ID\":");
  strcat(json, data);
  strcat(json, ",\"humidity\":");
  strcat(json, humidity);
  strcat(json, ",\"lightExposure\":");
  strcat(json, light);
  strcat(json, "}");

  // SEND DATA
  if(udp.beginPacket(host, port)) {
    udp.write(json);
    udp.endPacket();
  }
}
