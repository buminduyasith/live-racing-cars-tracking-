#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(15, 13); /* (Rx, Tx) (4, 5) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;

volatile float minutes, seconds;
volatile int degree, secs, mins;

//Variables
int i = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;

int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;
String dateStr, timeStr;

String deviceID = "1";
String deviceSecret = "001JdX670M43";
String url = "https://live-racing-cars-tracking-api.onrender.com/";

//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void ReadGps(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);         //Initialising if(DEBUG)Serial Monitor
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512);  //Initialasing EEPROM
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i) {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi()) {
    Serial.println("Succesfully Connected!!!");

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Welcome !!");
    delay(100);

    return;
  } else {
    Serial.println("AP setting up");

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("AP setting up");

    launchWeb();
    setupAP();  // Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");

  while ((WiFi.status() != WL_CONNECTED)) {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((WiFi.status() == WL_CONNECTED)) {
    ReadGps();
    Serial.println("Running");
  } else {
    Serial.println("Connection lost");
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connection");
    lcd.setCursor(6, 1);
    lcd.print("lost..");
    delay(1000);
    lcd.clear();
  }
}
//----------------------------------------------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change
bool testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while (c < 20) {
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb() {
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("RTRTS AP 01", "");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}

void createWebServer() {
  {
    server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content += "<!DOCTYPE html>\n";
      content += "<html>\n";
      content += "<head>\n";
      content += "<title> The Real Time Racing Tracking System </title>\n";
      content += "<style>\n";
      content += "    @import url(https://fonts.googleapis.com/css?family=Roboto:300);\n";
      content += "header .header{\n";
      content += "  background-color: #fff;\n";
      content += "  height: 45px;\n";
      content += "}\n";
      content += "header a img{\n";
      content += "  width: 134px;\n";
      content += "margin-top: 4px;\n";
      content += "}\n";
      content += ".login-page {\n";
      content += "  width: 360px;\n";
      content += "  padding: 8% 0 0;\n";
      content += "  margin: auto;\n";
      content += "}\n";
      content += ".login-page .form .login{\n";
      content += "  margin-top: -31px;\n";
      content += "margin-bottom: 26px;\n";
      content += "}\n";
      content += ".form {\n";
      content += "  position: relative;\n";
      content += "  z-index: 1;\n";
      content += "  background: #FFFFFF;\n";
      content += "  max-width: 360px;\n";
      content += "  margin: 0 auto 100px;\n";
      content += "  padding: 45px;\n";
      content += "  text-align: center;\n";
      content += "  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);\n";
      content += "}\n";
      content += ".form input {\n";
      content += "  font-family: \"Roboto\", sans-serif;\n";
      content += "  outline: 0;\n";
      content += "  background: #f2f2f2;\n";
      content += "  width: 100%;\n";
      content += "  border: 0;\n";
      content += "  margin: 0 0 15px;\n";
      content += "  padding: 15px;\n";
      content += "  box-sizing: border-box;\n";
      content += "  font-size: 14px;\n";
      content += "}\n";
      content += ".form button {\n";
      content += "  font-family: \"Roboto\", sans-serif;\n";
      content += "  text-transform: uppercase;\n";
      content += "  outline: 0;\n";
      content += "  background-color: #328f8a;\n";
      content += "  background-image: linear-gradient(45deg,#328f8a,#08ac4b);\n";
      content += "  width: 100%;\n";
      content += "  border: 0;\n";
      content += "  padding: 15px;\n";
      content += "  color: #FFFFFF;\n";
      content += "  font-size: 14px;\n";
      content += "  -webkit-transition: all 0.3 ease;\n";
      content += "  transition: all 0.3 ease;\n";
      content += "  cursor: pointer;\n";
      content += "}\n";
      content += ".form .message {\n";
      content += "  margin: 15px 0 0;\n";
      content += "  color: #b3b3b3;\n";
      content += "  font-size: 12px;\n";
      content += "}\n";
      content += ".form .message a {\n";
      content += "  color: #4CAF50;\n";
      content += "  text-decoration: none;\n";
      content += "}\n";
      content += "\n";
      content += ".container {\n";
      content += "  position: relative;\n";
      content += "  z-index: 1;\n";
      content += "  max-width: 300px;\n";
      content += "  margin: 0 auto;\n";
      content += "}\n";
      content += "\n";
      content += "body {\n";
      content += "  background-color: #328f8a;\n";
      content += "  background-image: linear-gradient(45deg,#328f8a,#08ac4b);\n";
      content += "  font-family: \"Roboto\", sans-serif;\n";
      content += "  -webkit-font-smoothing: antialiased;\n";
      content += "  -moz-osx-font-smoothing: grayscale;\n";
      content += "}\n";
      content += "</style>\n";
      content += "</head>\n";
      content += "<body>\n";
      content += "  <body>\n";
      content += "    <div class=\"login-page\">\n";
      content += "      <div class=\"form\">\n";
      content += "        <div class=\"login\">\n";
      content += "          <div class=\"login-header\">\n";
      content += "            <h3>Real Time Racing Tracking System</h3>\n";
      content += "            <p>Please enter your credentials to connect to router.</p>\n";
      content += "          </div>\n";
      content += "        </div>\n";
      content += "        <form class=\"login-form\"form method='get' action='setting'>\n";
      content += "          <input type=\"text\" placeholder=\"SSID\" id=\"ssid\" name=\"ssid\"/>\n";
      content += "          <input type=\"password\" placeholder=\"Password\" id=\"pass\" name=\"pass\"/>\n";
      content += "          <button type=\"submit\" value=\"connect\">Connect</button>\n";
      content += "          <p class=\"message\">© 2023 Real Time Racing Tracking System by Group-08</p>\n";
      content += ipStr;
      content += st;
      content += "        </form>\n";
      content += "      </div>\n";
      content += "    </div>\n";
      content += "</body>\n";
      content += "</body>\n";
      content += "</html>";
      content += "";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i) {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i) {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
    });
  }
}

void ReadGps() {
  smartDelay(1000); /* Generate precise delay of 1ms */
  unsigned long start;
  int spd_val;
  double lat_val, lng_val, alt_m_val;
  uint8_t hr_val, min_val, sec_val;
  bool loc_valid, alt_valid, time_valid, spd_valid;
  lat_val = gps.location.lat();       /* Get latitude data */
  loc_valid = gps.location.isValid(); /* Check if valid location data is available */
  lng_val = gps.location.lng();       /* Get longtitude data */
  alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
  alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
  spd_valid = gps.speed.isValid();    /*check if valid speed data is available*/
  spd_val = gps.speed.kmph();         /*Get speed data*/

  if (!loc_valid) {
    Serial.print("Latitude : ");
    Serial.println("*****");
    Serial.print("Longitude : ");
    Serial.println("*****");
    ConSat();
  } else {
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees : ");
    Serial.println(lat_val, 6);
    Serial.print("Latitude in Degrees Minutes Seconds : ");
    Serial.print(degree);
    Serial.print("\t");
    Serial.print(mins);
    Serial.print("\t");
    Serial.println(secs);
    DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
    Serial.print("Longitude in Decimal Degrees : ");
    Serial.println(lng_val, 6);
    Serial.print("Longitude in Degrees Minutes Seconds : ");
    Serial.print(degree);
    Serial.print("\t");
    Serial.print(mins);
    Serial.print("\t");
    Serial.println(secs);
    timeStr = GetTme();
    dateStr = GetDte();
  }
  if (!alt_valid) {
    Serial.print("Altitude : ");
    Serial.println("*****");
    ConSat();
  } else {
    Serial.print("Altitude : ");
    Serial.println(alt_m_val, 6);
    timeStr = GetTme();
    dateStr = GetDte();
  }
  if (!spd_valid) {
    Serial.print("Speed : ");
    Serial.println("*****");
    ConSat();
  } else {
    spd_val = gps.speed.kmph(); /*Get speed data*/
    Serial.print("Speed :");
    Serial.println(spd_val);
    timeStr = GetTme();
    dateStr = GetDte();
    lcd.setCursor(0, 0);
    lcd.print(time_str);
    lcd.setCursor(5, 1);
    lcd.print(spd_val);
    lcd.setCursor(9, 1);
    lcd.print("kmph");
    CallApi(lat_val, lng_val, spd_val, timeStr, dateStr);
  }

  Serial.println(dateStr);
  Serial.println(timeStr);
  //CallApi(lat_val, lng_val, spd_val, timeStr, dateStr);
}

String GetTme() {
  if (gps.time.isValid())  //check whether gps time is valid
  {
    time_str = "";
    hour = gps.time.hour();
    minute = gps.time.minute();
    second = gps.time.second();
    minute = (minute + 30);  // converting to IST
    if (minute > 59) {
      minute = minute - 60;
      hour = hour + 1;
    }
    hour = (hour + 5);
    if (hour > 23)
      hour = hour - 24;  // converting to IST
    if (hour >= 12)      // checking whether AM or PM
      pm = 1;
    else
      pm = 0;
    hour = hour % 12;
    if (hour < 10)
      time_str = '0';
    time_str += String(hour);  //values of hour,minute and time are stored in a string
    time_str += " : ";
    if (minute < 10)
      time_str += '0';
    time_str += String(minute);  //values of hour,minute and time are stored in a string
    time_str += " : ";
    if (second < 10)
      time_str += '0';
    time_str += String(second);  //values of hour,minute and time are stored in a string
    if (pm == 1)
      time_str += " PM ";
    else
      time_str += " AM ";
  }
  return time_str;
}

String GetDte() {
  if (gps.date.isValid())  //check whether gps date is valid
  {
    date_str = "";
    date = gps.date.day();
    month = gps.date.month();
    year = gps.date.year();
    if (date < 10)
      date_str = '0';
    date_str += String(date);  // values of date,month and year are stored in a string
    date_str += " / ";

    if (month < 10)
      date_str += '0';
    date_str += String(month);  // values of date,month and year are stored in a string
    date_str += " / ";
    if (year < 10)
      date_str += '0';
    date_str += String(year);  // values of date,month and year are stored in a string
  }
  return date_str;
}
void DegMinSec(double tot_val) /* Convert data in decimal degrees into degrees minutes seconds form */
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (GPS_SoftSerial.available()) /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}


void CallApi(double lat_val, double lng_val, int spd_val, String timeStr, String dateStr) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;

  String fullUrl = url + "?DeviceId=" + deviceID + "&DeviceSecret=" + deviceSecret + "&latitude=" + String(lat_val) + "&longitude=" + String(lng_val) + "&speed=" + String(spd_val) + "&time=" + timeStr + "&date=" + dateStr;
  Serial.println(deviceID);
  Serial.println(deviceSecret);
  Serial.println(String(lat_val));
  Serial.println(String(lng_val));
  Serial.println(String(spd_val));
  Serial.println(timeStr);
  Serial.println(dateStr);

  Serial.println("Requesting " + fullUrl);
  if (https.begin(client, fullUrl)) {
    int httpCode = https.POST("");
    Serial.println("============== Response code: " + String(httpCode));
    if (httpCode > 0) {
      Serial.println(https.getString());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  delay(500);
}

void ConSat() {
  lcd.setCursor(3, 0);
  lcd.print("Setting up");
  lcd.setCursor(2, 1);
  lcd.print("sat connection");
  delay(1000);
  lcd.clear();
}

