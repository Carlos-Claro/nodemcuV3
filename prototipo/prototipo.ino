#include <HCSR04.h>
#include <ESP8266WiFi.h>

const char* ssid = "CarlosS2lu_2.4Ghz"; //your WiFi Name
const char* password = "12345678";  //Your Wifi Password
int ledPin = 00; 
HCSR04 hc(4, 5);
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(10); 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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
}

void loop() {
  
  Serial.print("Distancia: "); // Escreve texto na tela
  Serial.print(hc.dist());// distância medida em cm
  Serial.println("cm");
  delay(1000);
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); 
  //sensor
  
  
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  
  client.println("");
  client.print("{");
  client.print("\"led\":"); 
  if(value == HIGH) {
    client.print("\"on\"");
  } else {
    client.print("\"off\"");
  } 
  client.print(",\"ultrasonic\":"); 
  client.print(hc.dist());  
  client.print("}");  
  client.println("");  
  delay(1); 
  Serial.println("Client disconnected");
  Serial.println("");

  
  }
