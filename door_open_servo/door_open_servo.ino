#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

// Werte zum anspassen


const char* ssid = <ssidhere>;  // <---- SSID des W-Lans
const char* password = <passhere>;  // <---- PW des W-Lans
int pause = 30;  // <---- Pausenlänge in Sekunden
int relax = 70;  // <---- Servo Pos wenn nicht gedrückt
int push = 20;   // <---- Servo Pos wenn gedrückt




// Hier muss eigentlich nichts angepasst werden, nur wenn etwas an der Funktionalität geändert wird

Servo servo;
int rled = D0;
int servoPin = D4;
String payload;
unsigned long timer;
unsigned long lasttime;
WiFiServer server(80);
int printpause = pause;

void setup() {

Serial.print("Servopos :");
Serial.println(servo.read());
  Serial.begin(115200);
  delay(10);
  pinMode(rled, OUTPUT);
  digitalWrite(rled, LOW);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(rled, LOW);
    delay(100);
    Serial.print(".");
    digitalWrite(rled, HIGH);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

Serial.println("----------------------");
Serial.println("ETV Door Receiver V0.1");
Serial.println("----------------------");
Serial.print("Warte auf Verbindung...");
timer = 1000 * pause;
lasttime = millis();

// Servo in richtige pos bringen

 for ( int i = servo.read(); i > relax; ) {
servo.write(i);
Serial.print("Servopos :");
Serial.println(servo.read());
i = i-1;
delay(10);
  }
  delay(100);
  for ( int i = servo.read(); i < relax; ) {
servo.write(i);
Serial.print("Servopos :");
Serial.println(servo.read());
i= i+1;
delay(10);
  }
servo.attach(servoPin);
}
void loop() {


// alle 30 (bzw. der wert von 'pause') Sekunden schauen wir, ob es einen Sender (Button) gibt.

int countdown = millis() / 1000 - lasttime / 1000;

if (countdown - printpause != 0) {

Serial.print("Time until next Ping comes at: ");
Serial.println(pause - countdown);
printpause = millis() / 1000 - lasttime / 1000;
}

if (millis() - lasttime > timer) {

f_ping();  // Wir pingen den Sender
lasttime = millis();
  
}

f_getRequest();

delay(100);

}


// Link 1 Schaltet LEDs EIN (debug)

void f_on() {

HTTPClient http_on;
http_on.begin("http://10.0.30.238/LED=ON");
int httpCode = http_on.GET();
http_on.end();   //Close connection

}



// Link 2 Schaltet LEDs AUS (debug)

void f_off() {


HTTPClient http_off;
http_off.begin("http://10.0.30.238/LED=OFF");
int httpCode = http_off.GET();
http_off.end();   //Close connection

}

// Link 3 Schaltet LEDs kurz ein (Ping), falls es failed schaltet er die interne rote LED ein

void f_ping() {

HTTPClient http_on;
http_on.begin("http://10.0.30.238/LED=PING");
int httpCode = http_on.GET();
if (httpCode > 0) {
    digitalWrite(rled, HIGH);
}
else {
  digitalWrite(rled, LOW);
}
    http_on.end();   //Close connection

}

// Wartet auf Requests vom Button und verarbeitet sie

void f_getRequest() {

    WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  /* Wenn der Open Befehl kommt, 1. dreh den Servo und 2. schalte beim Sender die LED aus.
   * Damit können wir einigermaßen sichergehen, dass die LED beim Sender erst aus geht
   * wenn das Signal hier auch angekommen und verarbeitet wurde.
   */
   
  if (request.indexOf("/OPEN") != -1) {
  Serial.println("Receiving Request");
  f_servo();
  delay(100);
  f_off();

  }
  

  }

void f_servo() {
  // SERVO DREHEN

Serial.println("Moving Servo...");

for ( int i = servo.read(); i > push; ) {
servo.write(i);
i = i-1;
delay(10);
  }
  delay(100);
  for ( int i = servo.read(); i < relax; ) {
servo.write(i);
i= i+1;
delay(10);
  }

  
/*  digitalWrite(rled, LOW);
  delay(100);
  digitalWrite(rled, HIGH);
  */
}

