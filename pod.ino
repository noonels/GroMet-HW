#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define SIZE (255)

const int HUMID_PIN  = 0;
const int LIGHT_PIN  = 0;
const char* ssid     = "GroMet Network";
const char* passwd   = "wallace"
const IPAddress host = "GroMetHub.local";
const int    port    = 80;
const string ID         = WiFi.macAddress();

WiFiUDP udp;
byte serdata    = 0;
byte fromserver = 0;
char json[SIZE];

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
}

void loop() {
  string humidity = (string)analogRead(HUMID_PIN);
  string light    = (string)analogRead(LIGHT_PIN);
  strcpy(json, "{\"ID\":");
  strcpy(json, ID);
  strcpy(json, ",\"humidity\":");
  strcpy(json, humidity);
  strcpy(json, ",\"lightExposure\":");
  strcpy(json, light);
  strcpy(json, "}");

  // SEND DATA
  if(udp.beginPacket(host, port)) {
    udp.write(json);
    udp.endPacket();
  }
}