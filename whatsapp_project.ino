#include <ESP8266WiFi.h>
#include <DHT.h>

/*** Sensor Model ***/
#define DHTTYPE DHT11

/*** Pin GPIO2 ***/
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE, 27);

/*** Variables for Humidity and Temperature ***/
float temperature;
float humidity;

// WiFi Parameters
const char* ssid = "Redmi";
const char* password = "22121995";

const char* host = "13.59.0.161";

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  dht.begin();

  Serial.printf ("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void loop() {
  WiFiClient client;
  Serial.printf ("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80)) {
    Serial.println("connected]");
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();  
    Serial.println("[Sending a request]");

    String data = "temperature=" + String(temperature, 2) +"&humidity=" + String(humidity, 2);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    client.println("POST / HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("Accept: */*");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    Serial.println("[Response:]");
    while (client.connected()) {
      if (client.available ()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  } else {
    Serial.println("connection failed !] ");
    client.stop();
  }
  delay(5000);
}
