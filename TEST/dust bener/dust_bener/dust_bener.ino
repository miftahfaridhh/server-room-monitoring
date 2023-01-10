/**********************************************/
/* Building Arduino Dust Sensor using:        */
/*      - Arduino Mega 2560                   */
/*      - Shinyei PPD42NS                     */
/* http://www.sca-shinyei.com/pdf/PPD42NS.pdf */
/*                                            */
/* Author: shadowandy[dot]sg[at]gmail[dot]com */
/* Web: www.shadowandy.net                    */
/*                                            */         
/* Wiring Instruction:                        */
/*      - PPD42NS Pin 1 => GND                */
/*      - PPD42NS Pin 2 => D51                */
/*      - PPD42NS Pin 3 => 5V                 */
/*      - PPD42NS Pin 4 => D50                */
/**********************************************/
 
#include <avr/wdt.h>
 
#define PM25 0
#define PM10 1
int pin[] = {8,9}; // 56 = 8, 57=9
unsigned long starttime;
unsigned long sampletime_ms = 2000;
unsigned long triggerOn[2];
unsigned long triggerOff[2];
unsigned long lowpulseoccupancy[] = {0,0};
float ratio[] = {0,0};
float count[] = {0,0};
boolean value[] = {HIGH,HIGH};
boolean trigger[] = {false, false};
 
void setup()
{
  
  Serial.begin(9600); //Output to Serial at 9600 baud
  pinMode(pin[PM25],INPUT); //Listen at the designated PIN
  starttime = millis(); //Fetching the current time
}
 
void loop()
{
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
    // End of mass concentration calculation
    
    // Begin printing to Serial
    Serial.print("PM10       : ");
    Serial.print(concentration[PM10]);
    Serial.println(" ug/m3");
    Serial.print("PM10 Count : ");
    Serial.print(count[PM10]);
    Serial.println(" pt/cf");
    Serial.print("PM2.5      : ");
    Serial.print(concentration[PM25]);
    Serial.println(" ug/m3");
    Serial.print("PM2.5 Count: ");
    Serial.print(count[PM25]);
    Serial.println(" pt/cf");
    Serial.println("");
   
    // Resetting for next sampling
    lowpulseoccupancy[PM25] = 0;
    lowpulseoccupancy[PM10] = 0;
    starttime = millis();
  }
}
