#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "";
const char *password = "";

// Domain Name with full URL Path for HTTP POST Request
const char *serverName = "http://api.thingspeak.com/update";

// write API Key provided by thingspeak
String apiKey = "";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    delay(10000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(t) || isnan(h))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    http.begin(client, serverName);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}
