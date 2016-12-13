// Receiver code from http://robotic-controls.com/learn/arduino/arduino-arduino-serial-communication

char str[4];

void setup() {
  Serial.begin(9600);
  //Serial.begin(9600);//
}

void loop() {
  int i=0;

  if (Serial.available()) {
    delay(100); //allows all serial sent to be received together
    while(Serial.available() && i<3) {
      str[i++] = Serial.read();
    }
    str[i++]='\0';
  }

  if(i>0) {
    Serial.println(str);
  }
}
