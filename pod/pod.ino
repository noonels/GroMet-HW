#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define SIZE (255)

const int HUMID_PIN  = 0;
const int LIGHT_PIN  = 0;
const char* ssid     = "GroMet Network";
const char* passwd   = "wallace";
const IPAddress host = (192, 168, 0, 1);
const int    port    = 80;
const String ID      = WiFi.macAddress();

WiFiUDP udp;
byte serdata    = 0;
byte fromserver = 0;
char json[SIZE];

void setup() {
  pinMode(HUMID_PIN, INPUT); // not on ESP01
  pinMode(LIGHT_PIN, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  delay(100); // give serial time to begin working

  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  char data[ID.length()];
  ID.toCharArray(data, ID.length());
}

void loop() {
  int a = analogRead(HUMID_PIN);
  char* humidity;
  itoa(a, humidity, 10);
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
