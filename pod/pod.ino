#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define SIZE (255)

const int HUMID_PIN  = 0;
const int LIGHT_PIN  = 0;
const char* ssid     = "GroMet Network";
const char* passwd   = "wallace";
const IPAddress host = (192, 168, 0, 1);
const int    port    = 80;
String ID      = WiFi.macAddress();

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

  randomSeed(analogRead(0)); // seed random for debug mode
}

void loop() {
  char* humidity;
  char* light;
  //itoa(analogRead(HUMID_PIN), humidity, 10);
  //itoa(analogRead(LIGHT_PIN), humidity, 10);
  itoa(random(500,900), humidity, 10);
  itoa(random(0,256), light, 10);
  
  strcat(json, "{\"ID\":");
  strcat(json, ID.c_str());
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
  // TODO make less clock synchronous [healym 2/26/19]
  delay(60 * 1000 * 5); // publish every 5 minutes
}
