#include "DHT.h"
#include <LowPower.h>
#include <avr/wdt.h>
#define Dig_pin 6 // vibr
#define DHTPIN 4  // dht = dat
#define DHTTYPE DHT22   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
/////------------------------------
//int pin = 8; // kuning
//int pon = 9; // abu abu
#define PM25 0
#define PM10 1
int pin[] = {8,9}; // 56 = 8, 57=9
unsigned long starttime;
unsigned long sampletime_ms = 1000;
unsigned long triggerOn[2];
unsigned long triggerOff[2];
unsigned long lowpulseoccupancy[] = {0,0};
float ratio[] = {0,0};
float count[] = {0,0};
boolean value[] = {HIGH,HIGH};
boolean trigger[] = {false, false};
float concentration[] = {0,0};

/////------------------------------
int soundPin = A0; // sound
int Soundvalue = 0; // sound

int Dig_out = LOW;
int Ana_out = 0;\
int ID = 1; //2
int counter = 0; // tambahan

String c = "";
String cmd;
float con25 = 0;
float con10 = 0;
float ct25 = 0;
float ct10 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pin[PM25],INPUT); //Listen at the designated PIN
  starttime = millis();//get the current time;
  dht.begin();
}

void loop() {
  if(Serial.available()){
    
    int b = Serial.read();
    value[PM25] = digitalRead(pin[PM25]);
    value[PM10] = digitalRead(pin[PM10]);
    
    if(value[PM25] == LOW && trigger[PM25] == false) {
      trigger[PM25] = true;
      triggerOn[PM25] = micros();
    }
    if(value[PM25] == HIGH && trigger[PM25] == true) {
      triggerOff[PM25] = micros();
      lowpulseoccupancy[PM25] += (triggerOff[PM25] - triggerOn[PM25]);
      trigger[PM25] = false;
    }
    if(value[PM10] == LOW && trigger[PM10] == false) {
      trigger[PM10] = true;
      triggerOn[PM10] = micros();
    }
    if(value[PM10] == HIGH && trigger[PM10] == true) {
      triggerOff[PM10] = micros();
      lowpulseoccupancy[PM10] += (triggerOff[PM10] - triggerOn[PM10]);
      trigger[PM10] = false;
    }
    wdt_reset();
   
    //Serial.print(b);
 // b==88 means NUMBER 1 in transmitter 
    if(b==88){
      if ((millis()-starttime) > sampletime_ms)//Checking if it is time to sample
      {
        ratio[PM25] = lowpulseoccupancy[PM25]/(sampletime_ms*10.0);
        count[PM25] = 1.1*pow(ratio[PM25],3)-3.8*pow(ratio[PM25],2)+520*ratio[PM25]+0.62;
        ratio[PM10] = lowpulseoccupancy[PM10]/(sampletime_ms*10.0);
        count[PM10] = 1.1*pow(ratio[PM10],3)-3.8*pow(ratio[PM10],2)+520*ratio[PM10]+0.62;
        count[PM25] -= count[PM10];
    // Begin mass concentration calculation
        float concentration[] = {0,0};
        double pi = 3.14159;
        double density = 1.65*pow(10,12);
        double K = 3531.5;
        // PM10
        double r10 = 2.6*pow(10,-6);
        double vol10 = (4/3)*pi*pow(r10,3);
        double mass10 = density*vol10;
        concentration[PM10] = (count[PM10])*K*mass10;
        // PM2.5
        double r25 = 0.44*pow(10,-6);
        double vol25 = (4/3)*pi*pow(r25,3);
        double mass25 = density*vol25;
        concentration[PM25] = (count[PM25])*K*mass25;
        
        Soundvalue = analogRead (soundPin);
        Dig_out = digitalRead(Dig_pin);
        Ana_out = analogRead(A0);
       // duration = pulseIn(pin, LOW);
       // durasi = pulseIn(pon, LOW);
       // lowpulseoccupancy = lowpulseoccupancy+duration;
       // lowpulseoccupansi = lowpulseoccupansi+durasi;
        float h = dht.readHumidity(); //Celcius
        float t = dht.readTemperature(); // Farenheit
        float f = dht.readTemperature(true);
   
        if (isnan(h) || isnan(t) || isnan(f)) {
          Serial.println(F("Failed to read from DHT sensor!"));
          return;
        }
        float hif = dht.computeHeatIndex(f, h);
        float hic = dht.computeHeatIndex(t, h, false);
        
        con25 = concentration[PM25];
        con10 = concentration[PM10];
        ct25 = count[PM25];
        ct10 = count[PM10];

        c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(con25)+",'E':"+String(ct25)+",'F':"+String(con10)+",'G':"+String(ct10)+",'H':"+String(Ana_out)+",'I':"+String(Dig_out)+",'J':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
      //c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+concentration[PM25]+",'E':"+count[PM25]+",'F':"+concentration[PM10]+",'G':"+count[PM10]+",'H':"+String(Ana_out)+",'I':"+String(Dig_out)+",'J':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
      //c = "'{'ID':" + String(ID) + ", 'A':"+String(h)+",'B':"+String(t)+",'C':"+String(hic)+",'D':"+String(PM25)+",'E':"+String(PM10)+",'F':"+String(Ana_out)+",'G':"+String(Dig_out)+",'H':"+String(Soundvalue, DEC)+",'Con':"+String(counter)+"}'";
        Serial.println(c);  
        counter ++;
        delay(3000);
        
        // End of mass concentration calculation
      // Resetting for next sampling
        lowpulseoccupancy[PM25] = 0;
        lowpulseoccupancy[PM10] = 0;
        starttime = millis();
        wdt_reset();

       }
     }
  }
}
