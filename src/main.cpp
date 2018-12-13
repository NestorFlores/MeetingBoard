#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid =  "VP_AR_PAR_D";     // replace with your wifi ssid and wpa2 key
const char *pass =  "VeloPar.2k17";

WiFiClient client;


void setup() {

  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(5, 0);

  // Print HELLO to the screen, starting at 5,0.
  lcd.print("HELLO");

  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(5, 1);      
  lcd.print("WORLD");

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
  
}

void loop() {
}