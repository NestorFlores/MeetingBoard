#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid =  "xxx";     // replace with your wifi ssid and wpa2 key
const char *pass =  "xxx";

WiFiClient wifiClient;

void autoScroll() {
  delay(1000);
  lcd.setCursor(16,1);
  lcd.autoscroll();
  lcd.print(" ");
}

void handleGestureSensor() {
  int result = analogRead(A0);
  Serial.print(result);
}

void setup() {

  

  //Connect to WIFI
  Serial.begin(9600);
      delay(10);

      Serial.println("Connecting to ");
      Serial.println(ssid);

      WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED)
         {
           delay(500);
           Serial.print(".");
         }
     Serial.println("");
     Serial.println("WiFi connected");

     HttpClient httpClient = HttpClient(wifiClient,"192.168.40.55",3000);

    // Make a HTTP request:
    httpClient.get("/");

    Serial.println("After http get");
  
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();

    Serial.println(response);
    Serial.println(statusCode);

    // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  lcd.setCursor(1, 0);
  
  lcd.print("");

  lcd.print(response);

}

void loop() {

    autoScroll();
    handleGestureSensor();
}