#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);


uint8_t indicator = 0x00;
uint8_t value = 0x00;

int red = 2;
int green = 3;
int blue = 4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  /*pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);*/

  strip.setBrightness(64);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

byte btype = 0;
char ctype = 0;

byte r = 0;
byte g = 0;
byte b = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 3) {
    btype = Serial.read();
    Serial.print(btype, BIN);
    Serial.print(" ");
    if (btype != 0xAA) {  // Not the sync byte
      // Serial.flush??
      while (Serial.available()) {
        Serial.read();
      }
    } else {  // Is the sync byte
      indicator = Serial.read();  // 2nd byte
      value = Serial.read();  // 3rd byte

      Serial.print(indicator, BIN);
      Serial.print(" ");
      Serial.println(value, BIN);

      switch (indicator) {
        case 0x01:  // Red
          //analogWrite(red, value);
          //colorFlash(strip.Color(255, 0, 0), 1500); // Red
          r = value;
          break;
        case 0x02:
          //analogWrite(green, value);
          //colorFlash(strip.Color(0, 255, 0), 1500); // Green
          g = value;
          break;
        case 0x03:
          //analogWrite(blue, value);
          //colorFlash(strip.Color(0, 0, 255), 1500); // Blue
          b = value;
          break;
      }

      colorFlash(strip.Color(r, g, b), 10);
      
    }
  }
}

void colorFlash(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  
  strip.show();
  delay(wait);
}



