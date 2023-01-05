//https://telinks.wordpress.com/2021/09/06/arduino-mengirim-data-suhu-dan-kelembaban-dht11-melalui-serial-dengan-format-json-menggunakan-library-arduinojson/
#include "DHT.h"
#include <ArduinoJson.h>

int pin = 8; // kuning
int pon = 9; // abu abu
int soundPin = A0; // sound
int Soundvalue = 0; // sound

#define Dig_pin 6 // vibr
#define DHTPIN 4  // dht = dat
#define DHTTYPE DHT22   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int Dig_out = LOW;
int Ana_out = 0;

unsigned long duration;
unsigned long durasi;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
unsigned long lowpulseoccupansi = 0;
float ratioPM25 = 0;
float rasioPM10 = 0;
float PM25 = 0;
float PM10 = 0;

DynamicJsonDocument jsonOUT(100);

void setup() {
 Serial.begin(9600);
 pinMode(8,INPUT);
 starttime = millis();//get the current time;
 Serial.println(F("Sensor test show values!"));
 dht.begin();
 
 jsonOUT["All Value"] = "All SENSOR";
}

void loop() {
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
 if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
 }
 float hif = dht.computeHeatIndex(f, h);
 float hic = dht.computeHeatIndex(t, h, false);
 ratioPM25 = lowpulseoccupancy/(sampletime_ms*10.0); // Integer percentage 0=>100
 rasioPM10 = lowpulseoccupansi/(sampletime_ms*10.0); 
 PM25 = 1.1*pow(ratioPM25,3)-3.8*pow(ratioPM25,2)+520*ratioPM25+0.62; // using spec sheet curve
 PM10 = (1.1*pow(rasioPM10,3)-3.8*pow(rasioPM10,2)+520*rasioPM10+0.62)*4;

 jsonOUT["Temperature"] = h;
 jsonOUT["Humidity"] = t;
 jsonOUT["Heat Index"] = f;
 jsonOUT["Heat in Celcius"] = hic;
 jsonOUT["Heat in Farenheit"] = hif;
 
 jsonOUT["PM 2.5"] = PM25;
 jsonOUT["PM 10"] = PM10;
 jsonOUT["Analog vibration"] = Ana_out;
 jsonOUT["Digital vibration"] = Dig_out;
 jsonOUT["Digital vibration"] = Soundvalue;

String s;
serializeJson(jsonOUT, s);
Serial.println(s);
serializeJsonPretty(jsonOUT, Serial);
Serial.println("");

/*
//temperature
 //Serial.print(F(" Hum: "));
 Serial.print(h);

 //Serial.print(F("%  Temp: "));
 Serial.print(F(" "));
 Serial.print(t);
 //Serial.print(F("C "));
 
 Serial.print(F(" "));
 Serial.print(f);
 Serial.print(F(" "));

 //Serial.print(F("F  Heat index: "));
 Serial.print(hic);
 Serial.print(F(" "));
 //Serial.print(F("C "));
 
 Serial.print(hif);
 Serial.print(F(" "));
 //Serial.print(F("F"));

// dust
 //Serial.print(" PM2.5 = ");
 Serial.print(PM25);
 Serial.print(F(" "));

 //Serial.print(" PM10 = ");
 Serial.print(PM10);
 Serial.print(F(" "));

//vibration
 //Serial.print(" pcs/0.01cf");
 //Serial.print("  Anaolog : ");
 Serial.print(Ana_out);
 Serial.print(F(" "));

 //Serial.print("  Digital :");
 Serial.print(Dig_out);
 Serial.print(F(" "));

 //Serial.print("  Digital :");
 Serial.println (Soundvalue, DEC);
*/
 }
