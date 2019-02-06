#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <sstream>
#include <string>
#include <Meeting.h>
using namespace std;

WiFiUDP ntpUDP;
WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, "192.168.40.67", 3000);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char *ssid = "VP_AR_PAR_D";
const char *pass = "VeloPar.2k17";
const char meetingSeparator = '|';
const char dateSeparator = '*';
const short mettingSize = 10;
Meeting meetings[mettingSize];

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

Meeting deserializeMeeting(String fullMeetingInfo){
  Meeting m;
  String value = fullMeetingInfo.substring(0, fullMeetingInfo.indexOf(dateSeparator));
  value.trim();
  Serial.println(value);
  m.time = atol(value.c_str());
  Serial.println(m.time);
  m.name = fullMeetingInfo.substring(fullMeetingInfo.indexOf(dateSeparator) + 1);
  m.name.replace("|", "");
  Serial.println(m.name);
  
  return m;
}

void FetchTodayMeetings()
{
  // Make a HTTP request:
  httpClient.get("/");

  Serial.println("After http get");

  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();
    Serial.println(response);
  Serial.println(statusCode);
  short r, t = 0;
  for (int i = 0; i < response.length(); i++)
  {
    if (response.charAt(i) == meetingSeparator && i != response.length())
    {
      Serial.println("deserializing meeting");
      meetings[t] = deserializeMeeting(response.substring(r, i));
      r = (i + 1);
      t++;
    }
  }
  
}



void setup()
{
  Serial.begin(9600);

  SetupScreen();
  SetupWifi();
  SetupTime();
  FetchTodayMeetings();
}

Meeting getNextMeeting()
{
  int currentTime = timeClient.getEpochTime();
  Meeting m;

 for (int i=0; i<mettingSize; i++){
    if (meetings[i].time > currentTime){
        m = meetings[i];
        break;
    }
  }

  return m;
}

void loop()
{
  timeClient.update();
 // Serial.println(timeClient.getEpochTime());
  Meeting m = getNextMeeting();
  lcd.setCursor(0,0);
  lcd.print(m.name);
  //Serial.println(m.name);
  delay(500);
  lcd.setCursor(0, 1);
  int secondsToNextMeeting = m.time - timeClient.getEpochTime();
  String timeForNextMeeting = String(secondsToNextMeeting / 60 ) +":"+ String(secondsToNextMeeting % 60);

  lcd.print(timeForNextMeeting + "                            ");

}