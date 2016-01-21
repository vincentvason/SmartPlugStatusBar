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

int flash = 0;
int timeh;
int timem;
int modes;
int lights;
int settimeh;
int settimem;
int starth;
int startm;
int endh;
int endm;
int totalstart;
int totalend;
int totaltime;
int pretimer;
int prestart;
int preend;

//---------------------------------------------------------------------------------------------
void blinkgreen () { digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, LOW); }
void blinkblue() { digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, LOW); }
void blinkwarnblue() {
  flash++;
  if (flash%2 == 1){digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, LOW);} 
  if (flash%2 == 0){digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, HIGH);}
  if (flash > 1000) flash = 0;
  }
void blinkwarnpink() {
  flash++;
  if (flash%2 == 1){digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, HIGH);}
  if (flash%2 == 0){digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, HIGH);}
  if (flash > 1000) flash = 0;
  }
void blinkpink () { digitalWrite(12, LOW);digitalWrite(13, HIGH);digitalWrite(14, HIGH); }
void blinkred () { digitalWrite(12, LOW);digitalWrite(13, LOW);digitalWrite(14, HIGH); }
void blinkwarnred() {
  flash++;
  if (flash%2 == 1){digitalWrite(12, LOW);digitalWrite(13, LOW);digitalWrite(14, HIGH);}
  if (flash%2 == 0){digitalWrite(12, HIGH);digitalWrite(13, LOW);digitalWrite(14, HIGH);}
  if (flash > 1000) flash = 0;
  }

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
  Serial.print(" TIME GET : ");
  Serial.print(timeh);
  Serial.print(" ");
  Serial.println(timem);

  totaltime = timeh * 60;
  totaltime += timem;
     
   }
//-------------------------------------------------------------
int getdata(String data) {
  WiFiClient client;
  //Serial.print("connecting to ");
  Serial.print("GET MODE  ");
  Serial.print(data);
  Serial.print(" STATUS :  ");
  if (!client.connect(host, httpsPort)) {
    Serial.print("F");
    return 0;
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
  Serial.print(" Mode Get : "); 
  Serial.println(value);
  return value;
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
//------------------------------------------------------------------
void scherange()
{
  if (modes == 2)
  {
    gettime();
    starth = timeh;
    startm = timem;
    endh = timeh + settimeh;
    endm = timem + settimem;
  }
  totalstart = starth*60;
  totalstart += startm;
  totalend = endh*60;
  totalend += endm;
  if (modes == 2 && totalend >= 1440)
  {
    totalend = totalend - 1440;
  }

  if(totalstart < 9) prestart = 1431+totalstart;
  else prestart = totalstart-10;
  if(totalend < 9) preend = 1431+totalend;
  else preend = totalend-10;
  
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
  modes = getdata("ModeSet");
  gettime();
  if(modes == 1) {
    lights = getdata("NormalLight");
    if(lights == 1) blinkgreen();
    if(lights == 0) blinkred();
    }
  if(modes == 2) {
    gettime();
    settimeh = getdata("TimerH");
    settimem = getdata("TimerM");
    scherange();
     while(modes == 2)
    {
      modes = getdata("ModeSet");
      gettime();
      if(totalstart <= totalend);
      { 
        if(totaltime <= totalend && totaltime > totalstart) {blinkpink(); Serial.print ("Light : On ");}
        else if(totaltime <= totalend && totaltime > preend) {blinkwarnpink(); Serial.print ("Light : Pre-Off ");}
        else {
          blinkred(); Serial.print ("Light : Off ");
          setdata("NormalLight",0);
          setdata("ModeSet",1);
        }
      }
      if(totalstart > totalend);
      {  
        if (totaltime >= totalstart && totaltime < 1440 && totaltime > 0 && totaltime <= preend) {blinkpink(); Serial.print ("Light : On ");}
        else if (preend <= 1430 && totaltime <= totalend && totaltime > preend) {blinkwarnpink(); Serial.print ("Light : Pre-Off ");}
        else if (prestart > 1430 && totaltime <= totalend && (totaltime > preend || totaltime < (preend-1431))) {blinkwarnpink(); Serial.print ("Light : Pre-Off ");}
        else {blinkred(); Serial.print ("Light : Off ");
        delay(100);
        setdata("NormalLight",0);
        setdata("ModeSet",1);}
      }
      Serial.print ("START : ");
      Serial.print (totalstart);
      Serial.print (" NOW : ");
      Serial.print (totaltime);
      Serial.print (" END : ");
      Serial.println (totalend);
    }
  }
  if(modes == 3) {
    starth = getdata("ScheduleH");
    startm = getdata("ScheduleM");
    endh = getdata("FinishH");
    endm = getdata("FinishM");
    scherange();
    while(modes == 3)
    {
      modes = getdata("ModeSet");
      gettime();
      if(totalstart <= totalend);
      {  
        if(totaltime <= totalstart && totaltime > prestart) {blinkwarnred(); Serial.print ("Light : Pre-On ");}
        else if(totaltime <= preend && totaltime > totalstart) {blinkblue(); Serial.print ("Light : On ");}
        else if(totaltime <= totalend && totaltime > preend) {blinkwarnblue(); Serial.print ("Light : Pre-Off ");}
        else {blinkred(); Serial.print ("Light : Off ");}
      }
      if(totalstart > totalend);
      {  
        if(prestart <= 1430 && totaltime <= totalstart && totaltime > prestart) {blinkwarnred(); Serial.print ("Light : Pre-On ");}
        else if (prestart > 1430 && totaltime <= totalstart && (totaltime > prestart || totaltime < (prestart-1431))) {blinkwarnred(); Serial.print ("Light : Pre-On ");}
        else if (totaltime >= totalstart && totaltime < 1440 && totaltime > 0 && totaltime <= preend) {blinkblue(); Serial.print ("Light : On ");}
        else if (preend <= 1430 && totaltime <= totalend && totaltime > preend) {blinkwarnblue(); Serial.print ("Light : Pre-Off ");}
        else if (prestart > 1430 && totaltime <= totalend && (totaltime > preend || totaltime < (preend-1431))) {blinkwarnblue(); Serial.print ("Light : Pre-Off ");}
        else {blinkred(); Serial.print ("Light : Off ");}
      }
      Serial.print ("START : ");
      Serial.print (totalstart);
      Serial.print (" NOW : ");
      Serial.print (totaltime);
      Serial.print (" END : ");
      Serial.println (totalend);
    }
  }
}
