#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

unsigned long requestTimer;
unsigned long lastRequestTimer = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int trigPin = 16;  // D0
const int echoPin = 5;  // D1
const int pumpPin = 4;  // D2
// SDA - D4, SCL - D3

// defines variables
long duration;
int distance;
 
const char* ssid = "NETWORK";
const char* password = "PASSWORD";

String payload;

void getData()
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) 
  {
 
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://192.168.100.11/MLSA/record");  //Specify request destination
    int httpResponseCode = http.GET();                //Send the request
 
    if (httpResponseCode > 0) 
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      payload = http.getString();
      Serial.println(payload);
    }
    else 
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("Not connected to server.");
  }
}
 
void setup () {
 
  

  Wire.begin(2,0); // SDA and SCL (D4 and D3)
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(pumpPin, OUTPUT); // Sets pumpPin as an Output
  digitalWrite(pumpPin, HIGH);
  
 
  WiFi.begin(ssid, password);
  
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
  
  Serial.println("\n.\n.\n.\nConnected\n.\n.\n.\n");
  getData();
}
 
void loop() {
  requestTimer = millis();
  if (requestTimer - lastRequestTimer >= 5000)
  {
    getData();
    lastRequestTimer = millis();
  }

  if (distance < 5)
  {
    digitalWrite(pumpPin, LOW);
    delay(1000);
    digitalWrite(pumpPin, HIGH);
  }
  
  lcd.clear();
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
  Serial.print("Distance (cm): ");
  Serial.println(distance);



  lcd.setCursor(0, 0);
  lcd.print("Incidenta judet:");
  lcd.setCursor(0, 1);
  lcd.print(payload);
  lcd.setCursor(7, 1);
  lcd.print("/1000 loc");
  delay(200);
}
