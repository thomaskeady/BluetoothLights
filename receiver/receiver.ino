// Receiver code from http://robotic-controls.com/learn/arduino/arduino-arduino-serial-communication

//char str[4];
//int received = 0;
uint8_t received = 0;

void setup() {
  Serial.begin(9600);
  //Serial.begin(9600);//
}

void loop() {
  int i=0;

  if (Serial.available()) {
    delay(100); //allows all serial sent to be received together
    while(Serial.available()) {
      received = Serial.read();
      Serial.println(received);
    }
    //Serial.println(received);
  }

}
