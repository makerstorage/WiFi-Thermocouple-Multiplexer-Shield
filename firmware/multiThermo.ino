#include <SPI.h>
#include "Adafruit_MAX31855.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>


AsyncWebServer server(80);

const char* ssid = "Tilgin-4sATNx42WS4H"; // Your WiFi SSID
const char* password = "AKpQwcUhvJ6Sk";// Your WiFi Password



#define MAXDO   D3
#define MAXCS   D2
#define MAXCLK  D1
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
const byte common = A0;  // where the multiplexer in/out port is connected

// the multiplexer address select lines (A/B/C)
const byte addressA = D7; // low-order bit
const byte addressB = D6;
const byte addressC = D5; // high-order bit



void setup ()
{
  Serial.begin (115200);
  Serial.println ("Starting multiplexer test ...");
  pinMode (addressA, OUTPUT);
  pinMode (addressB, OUTPUT);
  pinMode (addressC, OUTPUT);

  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(1);
 
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    ESPDash.init(server);   // Initiate ESPDash and attach your Async webserver instance
    // Add Respective Cards
    ESPDash.addTemperatureCard("temp2", "Temperature Card 1",0);
    ESPDash.addTemperatureCard("temp3", "Temperature Card 2", 0);
    ESPDash.addTemperatureCard("temp4", "Temperature Card 3", 0);
    ESPDash.addTemperatureCard("temp5", "Temperature Card 4", 0);
    ESPDash.addTemperatureCard("temp6", "Temperature Card 5", 0);
    ESPDash.addTemperatureCard("temp7", "Temperature Card 6", 0);
    ESPDash.addTemperatureCard("temp8", "Temperature Card 7", 0);
    ESPDash.addTemperatureCard("temp1", "Temperature Card 8", 0);

    server.begin();
  
}  // end of setup

int readSensor (const byte which)
{
  // select correct MUX channel
  digitalWrite (addressA, (which & 1) ? HIGH : LOW);  // low-order bit
  digitalWrite (addressB, (which & 2) ? HIGH : LOW);
  digitalWrite (addressC, (which & 4) ? HIGH : LOW);  // high-order bit
  // now read the sensor
  double c = thermocouple.readCelsius();

  if (isnan(c)) {
         Serial.println("Something wrong with thermocouple! or no thermocouple");
         return -1;
  }

  return c;
       
}  // end of readSensor

void loop ()
{ 
  

    for (int i = 0; i <8; i++)
    {  
       double c = readSensor(i);    


       if(i==0){
        ESPDash.updateTemperatureCard("temp1", c);  
        }
       else if(i==1){
        ESPDash.updateTemperatureCard("temp2",c); 
        }
        else if(i==2){
        ESPDash.updateTemperatureCard("temp3",c);  
        }
        else if(i==3){
        ESPDash.updateTemperatureCard("temp4",c);  
        }
        else if(i==4){
        ESPDash.updateTemperatureCard("temp5",c);  
        }
        else if(i==5){
        ESPDash.updateTemperatureCard("temp6",c); 
        }
        else if(i==6){
        ESPDash.updateTemperatureCard("temp7",c); 
        }
        else if(i==7){
        ESPDash.updateTemperatureCard("temp8",c); 
        }
       
        delay(100);
    }//for

  
}  // end of loop
