/*Developed by M V Subrahmanyam - https://www.linkedin.com/in/veera-subrahmanyam-mediboina-b63997145/
Project: AWS | NodeMCU ESP32 Tutorials
Electronics Innovation - www.electronicsinnovation.com

GitHub - https://github.com/VeeruSubbuAmi
YouTube - http://bit.ly/Electronics_Innovation

Upload date: 07 October 2019

AWS Iot Core

This example needs https://github.com/esp8266/arduino-esp8266fs-plugin

It connects to AWS IoT server then:
- publishes "hello world" to the topic "outTopic" every two seconds
- subscribes to the topic "inTopic", printing out any messages
*/

#include <SPI.h>
#include "Adafruit_MAX31855.h"


#include "FS.h"

#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <NTPClient.h>

#include <WiFiUdp.h>

// Update these with values suitable for your network.

const char * ssid = "Tilgin-4sATNx42WS4H"; // Your WiFi SSID
const char * password = "AKpQwcUhvJ6Sk"; // Your WiFi Password


#define MAXDO   D3
#define MAXCS   D2
#define MAXCLK  D1
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
const byte common = A0;  // where the multiplexer in/out port is connected

// the multiplexer address select lines (A/B/C)
const byte addressA = D7; // low-order bit
const byte addressB = D6;
const byte addressC = D5; // high-order bit



WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char * AWS_endpoint = "ad788dbyue7l5-ats.iot.us-east-1.amazonaws.com"; //MQTT broker ip

void callback(char * topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();

}
WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[150];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  espClient.setBufferSizes(512, 512);
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESPthing")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "{\"message\": \"yeah connected\"}");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      espClient.getLastSSLError(buf, 256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


int readSensor (const byte which)
{
  // select correct MUX channel
  digitalWrite (addressA, (which & 1) ? HIGH : LOW);  // low-order bit
  digitalWrite (addressB, (which & 2) ? HIGH : LOW);
  digitalWrite (addressC, (which & 4) ? HIGH : LOW);  // high-order bit
  // now read the sensor
  double c = thermocouple.readCelsius();

  if (isnan(c)) {
        // Serial.println("Something wrong with thermocouple! or no thermocouple");
         return -1;
  }

  return c;
       
}  // end of readSensor


void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.println ("Starting multiplexer test ...");
  pinMode (addressA, OUTPUT);
  pinMode (addressB, OUTPUT);
  pinMode (addressC, OUTPUT);


  
  setup_wifi();
  delay(1000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());

  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  } else
    Serial.println("Success to open cert file");

  delay(1000);

  if (espClient.loadCertificate(cert))
    Serial.println("cert loaded");
  else
    Serial.println("cert not loaded");

  // Load private key file
  File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  } else
    Serial.println("Success to open private cert file");

  delay(1000);

  if (espClient.loadPrivateKey(private_key))
    Serial.println("private key loaded");
  else
    Serial.println("private key not loaded");

  // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  } else
    Serial.println("Success to open ca");

  delay(1000);

  if (espClient.loadCACert(ca))
    Serial.println("ca loaded");
  else
    Serial.println("ca failed");

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {

int temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;

//  {"state": {"reported" : {"color" : { "r" :255, "g": 255, "b": 0 } }}}  



    
       temp1 = readSensor(0);  
       delay(100);
       temp2 = readSensor(1);  
       delay(100);
       temp3 = readSensor(2);  
       delay(100);
       temp4 = readSensor(3);  
       delay(100);
       temp5 = readSensor(4);  
       delay(100);
       temp6 = readSensor(5);  
       delay(100);
       temp7 = readSensor(6);  
       delay(100);
       temp8 = readSensor(7);  




 

     

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf(msg, 150, "{\"temperatures\":{\"t1\":%d,\"t2\":%d,\"t3\":%d,\"t4\":%d,\"t5\":%d,\"t6\":%d,\"t7\":%d,\"t8\":%d}}", temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8);

    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.print("Heap: ");
    Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(100); // wait for a second
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  delay(100); // wait for a second
}
