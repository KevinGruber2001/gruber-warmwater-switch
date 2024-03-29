#include <Arduino.h>
#include <WifiClient.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <ww_client/Led.h>

#define BUTTON 5
#define LED 4

const char *ssid = WIFI_SSID;
const char *password = WIFI_SECRET;

const char *mqtt_server = MQTT_BROKER;

const char *clientTopic = "warmwasser/client";
const char *deviceTopic = "warmwasser/device";

const char *deviceId = "Client-1";

WiFiClient espClient;
PubSubClient client(espClient);

Led led(LED);

long lastMsg = 0;
bool oldStatusButton = HIGH;

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  Serial.println(messageTemp);

  if (String(topic) == deviceTopic)
  {
    if (messageTemp == "RUN")
    {
      led.toggleOn();
    }
    if (messageTemp == "STOP")
    {
      led.toggleOff();
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceId))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe(deviceTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{

  pinMode(BUTTON, 5);
  Serial.begin(115200);

  led.toggleOff();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 50)
  {
    lastMsg = now;

    if (digitalRead(BUTTON) == LOW)
    {
      if (oldStatusButton == HIGH)
      {
        client.publish(clientTopic, "START");
        oldStatusButton = LOW;
      }
    }
    else
    {
      oldStatusButton = HIGH;
    }
  }
}