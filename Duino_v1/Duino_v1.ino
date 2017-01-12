
uint8_t indicator = 0x00;
uint8_t value = 0x00;

int red = 2;
int green = 3;
int blue = 4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 3) {
    if (Serial.read() != 0xAA) {  // Not the sync byte
      // Serial.flush??
      while (Serial.available()) {
        Serial.read();
      }
    } else {  // Is the sync byte
      indicator = Serial.read();  // 2nd byte
      value = Serial.read();  // 3rd byte

      switch (indicator) {
        case 0x01:  // Red
          analogWrite(red, value);
          break;
        case 0x02:
          analogWrite(green, value);
          break;
        case 0x02:
          analogWrite(blue, value);
          break;
      }
      
    }
  }
}
