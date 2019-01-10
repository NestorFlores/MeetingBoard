#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <sstream>
#include <string>

WiFiUDP ntpUDP;
WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, "192.168.40.64", 3000);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char *ssid = "VP_AR_PAR_D";
const char *pass = "VeloPar.2k17";
const char meetingSeparator = '|';
const char dateSeparator = '*';
const short mettingSize = 5;
String meetings[mettingSize];

void autoScroll()
{
  delay(1000);
  lcd.setCursor(16, 1);
  lcd.autoscroll();
  lcd.print(" ");
}

void handleGestureSensor()
{
  int result = analogRead(A0);
  Serial.print(result);
}

void SetupWifi()
{
  //Connect to WIFI

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

void SetupScreen()
{
  // Should match the number provided to the constructor.
  lcd.begin(16, 2);
  lcd.init();
  // Turn on the backlight.
  lcd.backlight();

  lcd.setCursor(1, 0);
}

void SetupTime()
{
  timeClient.begin();
}

void FetchTodayMeetings()
{
  // Make a HTTP request:
  httpClient.get("/");

  Serial.println("After http get");

  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();
  short r, t = 0;
  for (int i = 0; i < response.length(); i++)
  {
    if (response.charAt(i) == meetingSeparator)
    {
      meetings[t] = response.substring(r, i);
      r = (i + 1);
      t++;
    }
  }

  Serial.println(response);
  Serial.println(statusCode);

  // The begin call takes the width and height. This
  // lcd.print(response);
}

void setup()
{
  Serial.begin(9600);
  delay(10);
  Serial.println("whatever");
  SetupScreen();
  SetupWifi();
  SetupTime();
  FetchTodayMeetings();
}

void displayNextMeeting()
{
  
}

void loop()
{
  timeClient.update();
  Serial.println(timeClient.getEpochTime());
  delay(1000);
  displayNextMeeting();
  // serialtimeClient.getEpochTime();
  //autoScroll();
  //handleGestureSensor();
}