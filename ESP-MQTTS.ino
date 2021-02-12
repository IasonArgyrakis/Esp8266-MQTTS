#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>
#include <Servo.h>

/* 
 *  secrets.h defines things that are - secret :-)
 * The real secrets.h will not be in the repository
 * obviously, but you can see fake values that it 
 * defines below.
 * 
 */

#include "secrets.h"

// MQTT Test Topics
#define in_topic "testo"
#define out_topic "test/sensor"

#define SERIAL_BAUD_RATE 115200

WiFiClientSecure espClient;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char receivedChar;
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
    Serial.print(receivedChar);
    Serial.println();
  }
  Serial.println(msg);

  if (msg == "1")
  {
    pressButton();
  }
  if (msg == "2")
  {
    query();
  }
}

PubSubClient client(MQTT_SERVER, MQTT_PORT, callback, espClient);
String clientId = "ESP8266-fab9";

Servo button_push_servo;

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();

  pinMode(A0, INPUT);
  button_push_servo.attach(D6);

  Serial.print("setup: This device MQTT client ID is ");
  Serial.println(clientId);

  // Configure essential network things - connect the wifi
  // set the system time and setInsecure is the magic option
  // to be able to connect to PubSubClients with username/password
  setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  //setup_time();
  espClient.setInsecure();
  Serial.println();
}
String json = "{ Armed :";

bool lastT = 0;
String stateJson() { return json + boolAsString(isArmed()) + "}"; }
boolean isArmed()
{
  if (analogRead(A0) < 850)
  {
    return true;
  }
  else
  {
    return false;
  }
}
String boolAsString(boolean current)
{
  if (current)
  {
    return "true";
  }
  else
  {
    return "false";
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (lastT != isArmed())
  {
    client.publish("outTopic", stateJson().c_str());
  }
  lastT = isArmed();
  delay(1000);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID

    // Attempt to connect
    if (client.connect("testing2"))
    {

      Serial.println("connected");
      Serial.println("Attempt Subscriptions");
      Serial.println(client.subscribe("testo", 1));
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
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

void setup_wifi(const char *ssid, const char *pwd)
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.print("setup_wifi: Connecting to ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("setup_wifi: WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}

int pos = 0;
void pressButton()
{
  client.publish("test/started_alarm_arm", "{ staterd :true }");
  if (!isArmed())
  {

    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      button_push_servo.write(pos); // tell servo to go to position in variable 'pos'
      delay(15);                    // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {                               // goes from 180 degrees to 0 degrees
      button_push_servo.write(pos); // tell servo to go to position in variable 'pos'
      delay(15);                    // waits 15ms for the servo to reach the position
    }
  }
}
