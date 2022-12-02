/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4
unsigned long myTime;

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Replace with your network credentials
const char* ssid     = "WifiName";
const char* password = "WifiPassword";

//ThingSpeak Credentials
const char* server = "api.thingspeak.com";
String apiKey = "ThingspeakAPIKey";  
/////////

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://LocalIP/post-esp-data.php";
   
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "DHT";
String sensorLocation = "Room";

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)
 /*
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }
  */
}

void loop() {
 myTime = millis();
  float realTime = myTime/1000 ; 
  /////

   /////
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
   
    WiFiClient client;
    HTTPClient http;
    ///-----------------///
  
 
      /////
   
    //int chk;
    //chk = DHT.read(DHT11_PIN); 

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    

  float hum=DHT.humidity;
  float cel=0;
  cel=DHT.temperature;

  float var= DHT.temperature;
  float far=0;
  float kel=0;
  far = ((cel * 1.8 ) + 32 );
  kel = (cel + 273.15);
  int chk;
  chk = DHT.read(DHT11_PIN);  
    
    //Checking for Valid Readings
  if (isnan(far) || isnan(hum)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  /////////////////////
  String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(0.0)
                            + "&value2=" + String(0.0) +  "";
  /////////////////////////
    ////////////////////
    // Prepare your HTTP POST request data
       if( (far != 32.0) && (hum != 0.0) ){
         httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(far)
                            + "&value2=" + String(hum) +  "";
       }

       
    //Serial.print("httpRequestData: ");
    Serial.println();
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
   
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    Serial.print("   --- ThingSpeak Portion--");
    if( (far != 32.0) && (hum != 0.0) ){
      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
        {  

        String postStr = apiKey;
        postStr +="&field1=";
        postStr += String(far);
        postStr +="&field2=";
        postStr += String(hum);
        postStr += "\r\n\r\n";
  
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
  
        Serial.print("Temperature: ");
        Serial.print(far);
        Serial.print(" degrees Celcius, Humidity: ");
        Serial.print(hum);
        Serial.println("%. Send to Thingspeak.");
                          
        }
    }
    client.stop();
   Serial.print("   --- END of ThingSpeak Portion--");
   Serial.println();
   Serial.print("   --- Local Host Portion--");
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(20000);  
}