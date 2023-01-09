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
int ID = 1; //2
int counter = 0; // tambahan
unsigned long duration;
unsigned long durasi;
unsigned long starttime;
unsigned long sampletime_ms = 500;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
unsigned long lowpulseoccupansi = 0;
float ratioPM25 = 0;
float ratioPM10 = 0;
float concentrationPM25 = 0;
float concentrationPM10 = 0;
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
    char a;
    int b = Serial.read();
    //Serial.print(b);
 // b==88 means NUMBER 1 in transmitter 
    if(b==88){
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
        ratioPM10 = lowpulseoccupancy/(sampletime_ms*10.0); 
        PM10 = 1.1*pow(ratioPM10,3)-3.8*pow(ratioPM10,2)+520*ratioPM10+0.62;
        ratioPM25 = lowpulseoccupancy/(sampletime_ms*10.0); // Integer percentage 0=>100rasioPM10 = lowpulseoccupansi/(sampletime_ms*10.0); 
        PM25 = 1.1*pow(ratioPM25,3)-3.8*pow(ratioPM25,2)+520*ratioPM25+0.62; // using spec sheet curve+String(dust10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+"}'";

        //Begin mass concentration
        float concentrationPM25 = 0.0;
        float concentrationPM10 = 0.0;
        float pi = 3.14159;
        float density = 1.65*pow(10,12);
        float K = 3531.5;
        // PM10
        float r10 = 2.6*pow(10,-6);
        float vol10 = (4/3)*pi*pow(r10,3);
        float mass10 = density*vol10;
        concentrationPM10 = (PM10)*K*mass10;
        // PM2.5
        float r25 = 0.44*pow(10,-6);
        float vol25 = (4/3)*pi*pow(r25,3);
        float mass25 = density*vol25;
        concentrationPM25 = (PM25)*K*mass25;
        // End of mass concentration calculation
            
      } 
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      float hif = dht.computeHeatIndex(f, h);
      float hic = dht.computeHeatIndex(t, h, false);
      //Loramessage = "'{'ID':" + String(ID) + ", 'Hum':"+String(h)+",'TempC':"+String(t)+",'Hic':"+String(hic)+",'PM25':"+String(concentrationPM25)+'ug/m3'+",'PM25 count':"+String(PM25)+'pt/cf'+",'PM10':"+String(concentrationPM10)+'ug/m3'+",'PM10 count':"+String(PM10)+'pt/cf'+",'Avib':"+String(Ana_out)+",'Dvib':"+String(Dig_out)+",'dsound':"+String(Soundvalue, DEC)+"}'";
      //Loramessage = "'{'ID':" + String(ID) + ", 'Hum':"+String(h)+",'TempC':"+String(t)+",'Hic':"+String(hic)+",'PM25':"+String(dust10)+",'PM10':"+String(dust10)+",'Avib':"+String(Ana_out)+",'Dvib':"+String(Dig_out)+",'dsound':"+String(Soundvalue, DEC)+"}'";     
      c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(concentrationPM25)+",'E':"+String(PM25)+",'F':"+String(concentrationPM10)+",'G':"+String(PM10)+",'H':"+String(Ana_out)+",'I':"+String(Dig_out)+",'J':"+String(Soundvalue, DEC)+",'Con':"+String(PM25)+"}'";
      //c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(PM25)+",'E':"+String(PM10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
      
      Serial.println(c);  
      counter ++;
      delay(5000);
      }
  }
}



// link for dust
//https://www.shadowandy.net/2015/05/arduino-dust-sensor.htm
