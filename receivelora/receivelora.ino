String cmd = "";

void setup() {
 Serial.begin(9600);
 
}

void loop() {
  if (Serial.available()){
    char SerialInByte;
    SerialInByte = Serial.read();
    if(SerialInByte==13) //carriage return \n
     { 
            //Serial.print("Received message : ");
            Serial.print(cmd);
            cmd = "";
     }
     else
     {
      cmd += String(SerialInByte);
     }
  }
}
