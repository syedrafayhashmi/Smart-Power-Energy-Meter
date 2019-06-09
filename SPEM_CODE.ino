#include <FirebaseArduino.h>
#include <FirebaseObject.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "ACS712.h"
#include <Smoothed.h> 
#include <math.h>

#define FIREBASE_HOST "yourhost.firebaseio.com"  // change it with your host
#define FIREBASE_AUTH "your Auth key"  // change it with your Auth key
#define WIFI_SSID "your_ssid" // change it with your ssid
#define WIFI_PASSWORD "your pass word"  // change it with your password
#define sensorInput A0
#define lastVal
#define currentReading
#define Relay_pin D3

ACS712 sensor(ACS712_20A, A0);
float I_reading = 0;
int m=0;
float power = 0;                 
float Wh =0 ;
float Cost=0;
void setup() {

pinMode(Relay_pin,OUTPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
    
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  String path="/";  
  FirebaseObject object = Firebase.get(path);
  Serial.println("Firebase begin");
  sensor.calibrate();
  Serial.println("Done!"); 
  digitalWrite(Relay_pin, HIGH);
 
}

void loop() {
    int menu;
    menu=Firebase.getString("MeasuredData/Menu").toInt();
       
    Serial.println(menu);
   switch (menu)
    {sensor.calibrate();
   case 1: {
      Serial.println("true");
      digitalWrite(Relay_pin, LOW);
      m=m+1;
      sensor.calibrate();
      float I_reading = sensor.getCurrentAC();
      Serial.println(I_reading);
      float power= I_reading*220;
      Wh = Wh + power *(((m))/3600000.0) ;
      Cost=Cost + (Wh*8);
      Serial.println(m);   
      Firebase.setFloat("MeasuredData/Current",I_reading);
      //Firebase.setFloat("power",power);  directly in app
      Firebase.setFloat("MeasuredData/Cost",Cost);
      Firebase.setFloat("MeasuredData/Wh",Wh);
      Firebase.setFloat("MeasuredData/m", m);
      delay(1000);
      
     break;
     }
    case 0:
    {//sensor.calibrate();
    Serial.println("false");
    digitalWrite(Relay_pin, HIGH);
    sensor.calibrate();
    I_reading = 0;
    power=0;
    Wh=0;
    m=0;
    Cost=0;
    Firebase.setFloat("MeasuredData/Current", I_reading);
   // Firebase.setFloat("MeasuredData/power", power);
    Firebase.setFloat("MeasuredData/Wh", Wh);
    Firebase.setFloat("MeasuredData/m", m);
    Firebase.setFloat("MeasuredData/Cost",Cost);
    
    break;
    
    }
    default:
      break;    }
  
   }
