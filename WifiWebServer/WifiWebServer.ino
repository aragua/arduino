/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include <ESP8266WiFi.h>

char ssid[] = "Lahoudere";      // your network SSID (name)
char pass[] = "putyourkey";   // your network password

int ledPin = D0;
WiFiServer server(80);

#define USE_LCD 1

#if USE_LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);
#endif

void setup() {
#if USE_LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Wifi: ");
  lcd.print(ssid);
  lcd.setCursor(0, 1);
#endif
  
  pinMode(ledPin, OUTPUT);  
  digitalWrite(ledPin, LOW);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if USE_LCD
  lcd.print("-");
#endif
  }
 
  server.begin();
 
#if USE_LCD
  lcd.clear();
  lcd.print("Wifi: ");
  lcd.print(ssid);
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
#endif
  
}

// the loop function runs over and over again forever
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  client.flush();
 
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
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  int temperature = analogRead(A0);
  client.print("Temperature is: ");
  client.print(temperature);
  client.println("deg. C");
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
}
