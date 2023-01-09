#include "DHT.h"
#include <LowPower.h>
int pin = 8; // kuning
int pon = 9; // abu abu
int soundPin = A0; // sound
int Soundvalue = 0; // sound

#define Dig_pin 6 // vibr
#define DHTPIN 4  // dht = dat
#define DHTTYPE DHT22   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int Dig_out = LOW;
int Ana_out = 0;\
//this code for JSON format
int ID = 3;
int dust10 = 0;
int counter = 0; /// tambahan

float ratioPM25 = 0;
float rasioPM10 = 0;
float PM25 = 0;
float PM10 = 0;

String c = "";
char a;
String cmd;

void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);
  c = "'Fajar Ganteng'";
//  Serial.println(c);
  dht.begin();

}

void loop() {
  if(Serial.available()){
    int b = Serial.read();
    //Serial.print(b);
 // b==88 means NUMBER 1 in transmitter 
    if(b==90){
      Soundvalue = analogRead (soundPin);
      Dig_out = digitalRead(Dig_pin);
      Ana_out = analogRead(A0);
      float h = dht.readHumidity(); //Celcius
      float t = dht.readTemperature(); // Farenheit
      float f = dht.readTemperature(true);
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
        }
      float hif = dht.computeHeatIndex(f, h);
      float hic = dht.computeHeatIndex(t, h, false);
  
      c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(dust10)+",'E':"+String(dust10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
      Serial.println(c);
      counter ++;
      delay(5000);
      }
    }
//    delay(1500);
}
