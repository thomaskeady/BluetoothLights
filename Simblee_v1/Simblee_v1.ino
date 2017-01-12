/*
Based off of ColorWheel 

This sketch is used with SimbleeForMobile.  The sketch will maintain
the color selected when disconnected, and can be easily
modified when reconnected.
*/

/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

#include <SimbleeForMobile.h>

#define RED_LED 2
#define GREEN_LED 3
uint8_t SYNC_BYTE = 0xAA;

// Color variables (start with all blank)
uint8_t red = 0x00;
uint8_t green = 0x00;
uint8_t blue = 0x00;

void greenLedOn() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}

void redLedOn() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

void setup() {

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  greenLedOn();

  Serial.begin(56700);  // Pretty sure this is good

  SimbleeForMobile.advertisementData = "Lights";

  // use a shared cache
  SimbleeForMobile.domain = "Simblee.com";

  SimbleeForMobile.begin();
}

void loop() {
  // switch to lower power mode
  // todo: Simblee_ULPDelay(INFINITE);

  // process must be called in the loop for SimbleeForMobile
  SimbleeForMobile.process();  
}

uint8_t rtextfield;
uint8_t rslider;
uint8_t gtextfield;
uint8_t gslider;
uint8_t btextfield;
uint8_t bslider;
uint8_t swatch;
uint8_t color_wheel;

void SimbleeForMobile_onConnect() {
  greenLedOn();
}

void SimbleeForMobile_onDisconnect() {
  redLedOn();
}

void update();

void ui()
{
  color_t darkgray = rgb(85,85,85);
  
  SimbleeForMobile.beginScreen(darkgray);

  // put the controls at the top, so the keyboard doesn't cover them up
  
  SimbleeForMobile.drawText(25, 71, "R:", WHITE);
  rslider = SimbleeForMobile.drawSlider(55, 65, 175, 0, 255);
  rtextfield = SimbleeForMobile.drawTextField(245, 65, 50, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(25, 116, "G:", WHITE);
  gslider = SimbleeForMobile.drawSlider(55, 110, 175, 0, 255);
  gtextfield = SimbleeForMobile.drawTextField(245, 110, 50, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(25, 161, "B:", WHITE);
  bslider = SimbleeForMobile.drawSlider(55, 155, 175, 0, 255);
  btextfield = SimbleeForMobile.drawTextField(245, 155, 50, 255, "", WHITE, darkgray);
  
  // border
  SimbleeForMobile.drawRect(25, 200, 270, 40, WHITE);
  swatch = SimbleeForMobile.drawRect(26, 201, 268, 38, WHITE);

  color_wheel = SimbleeForMobile.drawImage(COLOR_WHEEL, 10, 250);

  SimbleeForMobile.setEvents(color_wheel, EVENT_COLOR);
  
  // todo; color swatch
  
  SimbleeForMobile.endScreen();

  // populate with the current red/green/blue values
  // (this must be done after endScreen() to force it to be done each time,
  // otherwise the initial values will be populated from the cache)

  // Put different buttons for different modes

  update();
}

uint8_t RED_INDICATOR = 0x01;
uint8_t GREEN_INDICATOR = 0x02;
uint8_t BLE_INDICATOR = 0x03;

void update()
{
  // Here was where they do the updating in the orig, do it again here?
  // Use Serial.write or Serial.write??
  Serial.write(SYNC_BYTE);
  Serial.write(RED_INDICATOR);
  Serial.write(red);
  Serial.write(SYNC_BYTE);  // Sending immediately will simply fill buffer, and be read off properly in order
  Serial.write(GREEN_INDICATOR);
  Serial.write(green);
  Serial.write(SYNC_BYTE);
  Serial.write(BLUE_INDICATOR);
  Serial.write(blue);
    
  SimbleeForMobile.updateValue(rslider, red);
  SimbleeForMobile.updateValue(rtextfield, red);

  SimbleeForMobile.updateValue(gslider, green);
  SimbleeForMobile.updateValue(gtextfield, green);

  SimbleeForMobile.updateValue(bslider, blue);
  SimbleeForMobile.updateValue(btextfield, blue);
  
  SimbleeForMobile.updateColor(swatch, rgb(red,green,blue));
}

void ui_event(event_t &event)
{
  if (event.id == color_wheel)
  {
    red = event.red;
    green = event.green;
    blue = event.blue;
  }
  else if (event.id == rslider || event.id == rtextfield)
    red = event.value;
  else if (event.id == gslider || event.id == gtextfield)
    green = event.value;
  else if (event.id == bslider || event.id == btextfield)
    blue = event.value;
    
  update();
}
