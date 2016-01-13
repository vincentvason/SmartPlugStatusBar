/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h> 


const char* ssid = "CE-ESL";
const char* password = "ceeslonly";

const char* host = "api.anto.io";
const char* host_time = "api.timezonedb.com";
const int httpsPort = 80;

int timeh;
int timem;
int modes;
int lights;
int settimeh;
int settimem;

//---------------------------------------------------------------------------------------------
void blinkgreen () { digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, LOW); }
void blinkblue() { digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, LOW); }
void blinkwarnblue(int x) {
  digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, LOW); 
  delay(x);
  digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, HIGH);
  }
void blinkwarnpink(int x) {
  digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, LOW); 
  delay(x);
  digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, HIGH);
  }
void blinkpink () { digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, HIGH); }
void blinkred () { digitalWrite(12, LOW);digitalWrite(13, LOW);digitalWrite(14, HIGH); }

//---------------------------------------------------------------------------------------------
void gettime()
{
   WiFiClient client;
  //Serial.print("connecting to ");
  Serial.print("GET TIME ");
  Serial.print(" STATUS :  ");
  if (!client.connect(host, httpsPort)) {
    Serial.print("F");
    return;
  }

  String url = "/?zone=Asia/Bangkok&format=json&key=BU3CDABKAG68";
  //Serial.print("requesting URL: ");
  Serial.print("P");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host_time + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  //Serial.println("headersrequest sent");
  String line = "";
  while (!client.available());
  while (client.available()) {
  line+=client.readStringUntil('\n');

  uint32_t index = line.indexOf('{');
  line.remove(0, index);

//  Serial.println("reply was:");
//  Serial.println("==========");
//  Serial.println(line);
//  Serial.println("==========");
//  Serial.println("closing connection");
    if (line == "\r") {
      Serial.print("P");
      break;
    }
  }
  if (line.startsWith("{\"status\":\"OK\"")) {
    //Serial.println("esp8266/Arduino CI successfull!");
    Serial.print("P");
  } else {
    Serial.print("F");
  }

    unsigned long long int value_time = 0;
    char num1_time = 0;
    int count_time = 0;
    int cha_time = 0;
   while (count_time < 30)
    {
        if(line[cha_time] == '\"') count_time++;
        cha_time++;
    }
    cha_time++;

    while(value_time < 1000000000)
    {

      value_time = value_time*10;
      num1_time = line[cha_time];
      value_time = value_time + (num1_time - '0');
      cha_time++;
      if(line[cha_time] == '\"') count_time++;
    }
    //printf("%d",value_time);

  time_t time = value_time;
  timeh = hour(time);
  timem = minute(time);
  Serial.print("TIME GET : ");
  Serial.print(timeh);
  Serial.print(" ");
  Serial.println(timem);
     
   }
//-------------------------------------------------------------
void getdata(String data) {
  WiFiClient client;
  //Serial.print("connecting to ");
  Serial.print("GET MODE  ");
  Serial.print(data);
  Serial.print(" STATUS :  ");
  if (!client.connect(host, httpsPort)) {
    Serial.print("F");
    return;
  }

 
  String url = "/channel/get/iNpvCwemTctHOjx3F84nL6fUbuSg0zEyGdK2ZP97/SmartPlug/";
  //Serial.print("requesting URL: ");
  Serial.print("P");

  client.print(String("GET ") + url + data + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  //Serial.println("headersrequest sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.print("P");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"result\":\"true\"")) {
    //Serial.println("esp8266/Arduino CI successfull!");
    Serial.print("P");
  } else {
    Serial.print("F");
  }
//  Serial.println("reply was:");
//  Serial.println("==========");
//  Serial.println(line);
//  Serial.println("==========");
//  Serial.println("closing connection");

  int value = 0;
  char num1 = 0;
  int count = 0;
  int cha = 0;
  while (count < 7)
  {
    if(line[cha] == '\"') count++;
    cha++;
  }

  if(count == 7)
  {

      num1 = line[cha];
      value = num1 - '0';
  }

  cha++;
  if(line[cha] == '\"') count++;

  while(count == 7)
  {

      value = value*10;
      num1 = line[cha];
      value = value + (num1 - '0');
      cha++;
      if(line[cha] == '\"') count++;


  }
  if(data = "ModeSet") modes = value;
  if(data = "LightSet") lights = value;
  if(data = "TimerH") settimeh = value;
  if(data = "TimerM") settimem = value;
  Serial.print(" Mode Get : "); 
  Serial.println(modes);
}
//----------------------------------------------------------------------------------------
void setdata(String data, int valueset) {
  WiFiClient client;
  //Serial.print("connecting to ");
  Serial.print("SET MODE  ");
  Serial.print(data);
  Serial.print(" TO  ");
  Serial.print(valueset);
  Serial.print("STATUS :  ");
  if (!client.connect(host, httpsPort)) {
    Serial.print("F");
    return;
  }

 
  String url = "/channel/set/iNpvCwemTctHOjx3F84nL6fUbuSg0zEyGdK2ZP97/SmartPlug/";

  client.print(String("GET ") + url + data + "/" + valueset + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.print("P");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.print("P");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"result\":\"true\"")) {
    //Serial.println("esp8266/Arduino CI successfull!");
    Serial.print("P");
  } else {
    Serial.print("F");
  }
//  Serial.println("reply was:");
//  Serial.println("==========");
//  Serial.println(line);
//  Serial.println("==========");
//  Serial.println("closing connection");

  int value = 0;
  char num1 = 0;
  int count = 0;
  int cha = 0;
  while (count < 6)
  {
    if(line[cha] == '\"') count++;
    cha++;
  }

  if(count == 6)
  {

      cha++;
      num1 = line[cha];
      value = num1 - '0';
  }

  cha++;
  if(line[cha] == '\"') count++;

  while(count == 6)
  {

      value = value*10;
      num1 = line[cha];
      value = value + (num1 - '0');
      cha++;
      if(line[cha] == '\"') count++;


  }
  modes = value;
  Serial.print("Mode: "); 
  Serial.println(modes);
}
//--------------------------------------------------------------------
void setup() {
  //D5 = 14 = RED
  //D6 = 12 = GREEN
  //D7 = 13 = BLUE
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
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


}
void loop() {
  getdata("ModeSet");
  gettime();
  if(modes == 1) {
    getdata("NormalLight");
    if(lights == 1) blinkgreen();
    if(lights == 0) blinkred();
    }
  if(modes == 2) {
    blinkpink();
    }
  if(modes == 3) {
    blinkblue();
    
    }
}
