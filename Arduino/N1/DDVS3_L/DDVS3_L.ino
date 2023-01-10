#include "DHT.h"
#include <LowPower.h>
#define Dig_pin 6 // vibr
#define DHTPIN 4  // dht = dat
#define DHTTYPE DHT22   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int pin = 8; // kuning
int pon = 9; // abu abu
int soundPin = A0; // sound
int Soundvalue = 0; // sound

int Dig_out = LOW;
int Ana_out = 0;\
//this code for JSON format
int ID = 2; //2
int counter = 0; // tambahan
unsigned long duration;
unsigned long durasi;
unsigned long starttime;
unsigned long sampletime_ms = 500;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
unsigned long lowpulseoccupansi = 0;
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
  starttime = millis();//get the current time;
  c = "'Fajar Ganteng'";
  dht.begin();
}

void loop() {
  if(Serial.available()){
    int b = Serial.read();
    //Serial.print(b);

 // b==89 means NUMBER 1 in transmitter 
    if(b==89){
      Soundvalue = analogRead (soundPin);
      Dig_out = digitalRead(Dig_pin);
      Ana_out = analogRead(A0);
      duration = pulseIn(pin, LOW);
      durasi = pulseIn(pon, LOW);
      lowpulseoccupancy = lowpulseoccupancy+duration;
      lowpulseoccupansi = lowpulseoccupansi+durasi;
      float h = dht.readHumidity(); //Celcius
      float t = dht.readTemperature(); // Farenheit
      float f = dht.readTemperature(true);
      if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
      {
        ratioPM25 = lowpulseoccupancy/(sampletime_ms*10.0); // Integer percentage 0=>100rasioPM10 = lowpulseoccupansi/(sampletime_ms*10.0); 
        PM25 = 1.1*pow(ratioPM25,3)-3.8*pow(ratioPM25,2)+520*ratioPM25+0.62; // using spec sheet curve+String(dust10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+"}'";
        PM10 = 1.1*pow(rasioPM10,3)-3.8*pow(rasioPM10,2)+520*rasioPM10+0.62;
        PM25 -= PM10;
      } 
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      float hif = dht.computeHeatIndex(f, h);
      float hic = dht.computeHeatIndex(t, h, false);
    
      c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(PM25)+",'E':"+String(PM10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
      Serial.println(c);  
      counter ++;
      delay(5000);
      }
  }
}
