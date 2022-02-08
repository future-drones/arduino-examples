#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP-01S Relay";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

ESP8266WebServer server(80);

uint8_t LEDPin = 2; //onboard LED GPIO2 - is inverted so LOW is on and HIGH is off
bool LEDStatus = HIGH;

uint8_t RelayPin = 0; //relay GPIO0 - is inverted so LOW is on and HIGH is off
bool RelayStatus = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(LEDPin, OUTPUT);
  pinMode(RelayPin, OUTPUT);

  digitalWrite(LEDPin, LEDStatus);
  digitalWrite(RelayPin, RelayStatus);

  WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.on("/relayon", handle_relayon);
  server.on("/relayoff", handle_relayoff);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LEDStatus!=digitalRead(LEDPin))
  {digitalWrite(LEDPin, LEDStatus);}

  
  if(RelayStatus!=digitalRead(RelayPin))
  {digitalWrite(RelayPin, RelayStatus);}

}

void handle_OnConnect() {

  //Serial.println("LED Status: OFF | Relay Status: OFF");
  server.send(200, "text/html", SendHTML(!LEDStatus,!RelayStatus)); 
}

void handle_ledon() {
  LEDStatus = LOW;
  Serial.println("LED Status: ON");
  server.send(200, "text/html", SendHTML(true,!RelayStatus)); 
}

void handle_ledoff() {
  LEDStatus = HIGH;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", SendHTML(false,!RelayStatus)); 
}

void handle_relayon() {
  RelayStatus = LOW;
  Serial.println("Relay Status: ON");
  server.send(200, "text/html", SendHTML(!LEDStatus,true)); 
}

void handle_relayoff() {
  RelayStatus = HIGH;
  Serial.println("Relay Status: OFF");
  server.send(200, "text/html", SendHTML(!LEDStatus,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t LEDstat,uint8_t Relaystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP-01S Relay Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(LEDstat)
  {ptr +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";}
  else
  {ptr +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";}

  if(Relaystat)
  {ptr +="<p>Relay Status: ON</p><a class=\"button button-off\" href=\"/relayoff\">OFF</a>\n";}
  else
  {ptr +="<p>Relay Status: OFF</p><a class=\"button button-on\" href=\"/relayon\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
