String cmd = "";
String trigger = "X";
String temp = trigger;
void setup() {
 Serial.begin(9600);
 Serial.println(trigger);
}

void loop() {
  if (Serial.available()){
    char SerialInByte;
    SerialInByte = Serial.read();
    if (temp==trigger){
          if(SerialInByte==13) //carriage return \n
         { 
                //Serial.print("Received message : ");
                Serial.println(cmd);
                cmd = "";
//                delay(250);
//                LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
                if(trigger=="X"){
                  trigger = "Y";
                  delay(1000);
                  Serial.println(trigger);
                  }
                else if(trigger=="Y"){
                  trigger = "Z";
                  delay(1000);
                  Serial.println(trigger);
                  }
                else if(trigger=="Z"){
                  trigger = "X";
                  delay(1000);
                  Serial.println(trigger);
                  }
                
                 
         }
         else
         {
          cmd += String(SerialInByte);
         }
      }
    temp = trigger;
  }
}