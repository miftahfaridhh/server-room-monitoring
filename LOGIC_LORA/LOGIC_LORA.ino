#include <LowPower.h>
String c = "";
String d = "";
String e = "";
char a;
String cmd;
int i = 2;

void setup() {
  Serial.begin(9600);
  c = "'Fajar Ganteng'";
//  Serial.println(c);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if(Serial.available()){
    char a;
    int b = Serial.read();
//    delay(100);
//    Serial.print(b);
//    if(b==49){
//      c = "'Fajar Ganteng'";
//      Serial.println(c);
//      
//      delay(500);
////      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
////      delay(1000);
////      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
//      }
//    if (b==50){
//      d = "'Fajar Ganteng Dong'";
//      
//      //digitalWrite(LED_BUILTIN, HIGH);
//      Serial.println(d);
//      
//      delay(500);
////      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
////      delay(1000);
////      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
//      }
      if (b==51){
      e = "'Fajar Ganteng Banget'";
      
      //digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(e);
      
      delay(500);
//      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
//      delay(1000);
//      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
      }
    }
//    delay(1500);
}
