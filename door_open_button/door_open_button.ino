#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

const char* ssid = "yourSSID";  // <---- SSID des W-Lans
const char* password = "yourPASS";  // <---- PW des W-Lans
int rled = D3;
int button = D1;
int buttonState = 0;
// int yled = D3;
int gled = D8;
int value = LOW;
int lasttime;
int pause = 60; // 60 Sekunden warten wir, bevor wir und sicher sind, dass die Verbindung fehlt
int timer;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(gled, HIGH);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
  digitalWrite(rled, HIGH);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  digitalWrite(gled, LOW);
  digitalWrite(rled, LOW);
  
  lasttime = millis();
  timer = pause * 1000;
}

void loop() {


if ( millis() - lasttime > timer ) {
  
   digitalWrite(rled, HIGH);
 
   if (WiFi.isConnected()) {
  Serial.println(WiFi.localIP().toString() + " (" + WiFi.macAddress() + ") (" + WiFi.SSID() + ")");
} else {
  Serial.println(WiFi.status());
  ESP.restart();
}

}

buttonState = digitalRead(button);

if (buttonState == HIGH) {
  digitalWrite(gled, HIGH);
  f_sendRequest();
}

f_getRequest();
 
  delay(100);
}

void f_getRequest() {

    WiFiClient client = server.available();
  if (!client) {
    return;
  }

 Serial.println("new client");
  while(!client.available()){
    delay(10);
  }
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  if (request.indexOf("/LED=PING") != -1) {
  f_ledping();
  }
  if (request.indexOf("/LED=ON") != -1) {
  f_ledon();
  return;
  } 
  if (request.indexOf("/LED=OFF") != -1) {
  f_ledoff();
  return;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("ETV Button");
  client.println("</html>");

  }

  void f_ledon() {
    digitalWrite(gled, HIGH);
    digitalWrite(rled, HIGH);
  }

  void f_ledoff() {
    digitalWrite(gled, LOW);
    digitalWrite(rled, LOW);
  }

  void f_ledping() {
    digitalWrite(gled, HIGH);
    delay(100);
    digitalWrite(gled, LOW);
    digitalWrite(rled, LOW);
    lasttime = millis();
  }

void f_sendRequest() {

Serial.println("Sending Request");
HTTPClient http_open;
http_open.begin("http://10.0.31.4/OPEN");
int httpCode = http_open.GET();
delay(50);
/* if (httpCode > 0) {
    digitalWrite(rled, HIGH);
}
else {
  Serial.print("HttpCode: ");
  Serial.println(httpCode);
  digitalWrite(rled, LOW);
  */
  http_open.end();
}


  
