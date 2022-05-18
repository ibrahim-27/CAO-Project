#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Mani"
#define WIFI_PASSWORD "qwerty123"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCRN03BW480BXmOxb9Jv9Q4hcyhnmCLMJg"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://crowd-control-a5cf8-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int count = 0;
bool signupOK = false;
/* Variables Used */

// pins used
int IR1 = 14;
int IR2 = 4;
int red = 15;
int green = 13;

// variables to store count and triggered IR
int enter = 0;
int ext = 0;
bool check1=false;
bool check2=false;
int buzzled=12;
int max_ppl = 5;

// to store time
long t = 0;

// to store last occurrence (enter / exit)
String last="";   

void setup() {
  Serial.begin(115200);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  /* Setting pins mode */
  pinMode(IR1,INPUT);
  pinMode(IR2, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzled,OUTPUT);
    if(Firebase.ready() && signupOK){
    Firebase.RTDB.setInt(&fbdo, "limit",max_ppl);
  }
}

void loop() {
  //Setting maxppl
  if (Firebase.ready() && signupOK ) {
    if (Firebase.RTDB.getInt(&fbdo, "limit")) {
      if (fbdo.dataType() == "int") {
        max_ppl = fbdo.intData();
      }
    }
  // setup
  if(digitalRead(IR1)==HIGH && digitalRead(IR2)==HIGH){
    last="";
  }
  
  /* If the first IR is triggered */
  if(digitalRead(IR1) == LOW)
  {
    if(last!="enter")
      check1=true;
      
    t = millis();
    if(check2){
      enter++;
      last="enter";
      check1=false;
      check2=false;
    }
    digitalWrite(green, HIGH);
  }
  else
  {
    digitalWrite(green, LOW);
  }

  /* If the second IR is triggered */
  if(digitalRead(IR2) == LOW)
  {
    if(last!="ext")
      check2=true;
      
    t = millis();
    if(check1){
      ext++;
      last="ext";
      check1=false;
      check2=false;
    }
    digitalWrite(red, HIGH);
  }
  else
  {
    digitalWrite(red, LOW);
  }

  /* Printing details */
  if(enter - ext < max_ppl)
  {
    Serial.print("People in the room are: ");
    Serial.println(enter - ext);
     if(Firebase.ready() && signupOK){
    Firebase.RTDB.setInt(&fbdo, "Total",enter-ext);
    digitalWrite(buzzled,LOW);
  }
  }
  else
  {
   
  digitalWrite(buzzled,HIGH);
  delay(100);
  digitalWrite(buzzled,LOW);
  }

  /* For debugging */
  if(check1){
    Serial.println("1 RECOGNIZED");
    }
   if(check2){
    Serial.println("2 RECOGNIZED");
   }

   /* Checks if an IR is left triggered -- reset after 2 sec */
   if(!(check1 && check2))
   {
      if(millis() - t > 2000)
      {
        check1 = false;
        check2 = false;
      }
   }
}
}
