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
#define BLUE_LED 4

#define NONE 0x00
#define RAINBOW_TIME 0x01
#define RAINBOW_SPACE 0x02
#define FLASH_COLOR 0x03
#define FLASH_RAINBOW 0x04
#define FLASH_RANDOM 0x05
#define PULSE 0x06
#define THEATER_CHASE 0x07
#define CHRISTMAS 0x08
#define FLICKER 0x09


uint8_t SYNC_BYTE = 0xAA;

///////////////////////////////////////////// // Communication variables

byte PATTERN = 0x00;      // What pattern:
byte BRIGHTNESS = 0x01; //
byte RATE = 0x02;       // How fast
byte SIZE = 0x03;       // How large
byte COLOR = 0x04;      // What color


///////////////////////////////////////////// // End communication variables


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
  pinMode(BLUE_LED, OUTPUT);

  redLedOn();

  Serial.begin(57600);  // Pretty sure this is good

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

uint8_t off_but;
uint8_t solid_but;
uint8_t pulse_but;
uint8_t rainbowTime_but;
uint8_t rainbowSpace_but;
uint8_t fColor_but;
uint8_t fRainbow_but;
uint8_t fRandom_but;
uint8_t candle_but;
uint8_t theater_but;
uint8_t christmas_but;
uint8_t scanner_but;

void SimbleeForMobile_onConnect() {
  greenLedOn();
}

void SimbleeForMobile_onDisconnect() {
  redLedOn();
}

void update();

void ui()
{
  SimbleeForMobile.beginScreen(GRAY);

  // put the controls at the top, so the keyboard doesn't cover them up
  /*
     * Two buttons per row: left x coord: 10, right x coord: 165
     *  Width: 145, 145
     * Three buttons per row: left x: 10, center x: 113, right: 217
     *  Width: 93, 94, 93
     * Four buttons per row: left x: 10, center-left x: 87, center-right: 164, right: 242
     *  Width: 67, 67, 68, 68
     *
     * 60 px buffer from top of screen, 45px between button layers
     */

  off_but =          SimbleeForMobile.drawButton(10, 60, 93, "OFF", BLACK);
  solid_but =        SimbleeForMobile.drawButton(113, 60, 94, "Solid", BLACK);
  pulse_but =        SimbleeForMobile.drawButton(217, 60, 93, "Pulse", BLACK);

  candle_but =       SimbleeForMobile.drawButton(10, 105, 93, "Candle", BLACK);
  fColor_but =       SimbleeForMobile.drawButton(113, 105, 94, "Flash", BLACK);
  scanner_but =      SimbleeForMobile.drawButton(217, 105, 93, "Scanner", BLACK);

  rainbowTime_but =  SimbleeForMobile.drawButton(10, 150, 145, "Time Rainbow", BLACK);
  rainbowSpace_but = SimbleeForMobile.drawButton(165, 150, 145, "Space Rainbow", BLACK);

  //  fRainbow_but =     SimbleeForMobile.drawButton(165, 150, 145, "Flash Rainbow", BLACK);
  //  fRandom_but =      SimbleeForMobile.drawButton(10, 195, 145, "Flash Random", BLACK);

  christmas_but =    SimbleeForMobile.drawButton(10, 195, 145, "Christmas", BLACK);
  theater_but =      SimbleeForMobile.drawButton(165, 195, 145, "Theater Chase", BLACK);


  SimbleeForMobile.setEvents(off_but,          EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(solid_but,        EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(pulse_but,        EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(rainbowTime_but,  EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(rainbowSpace_but, EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(fColor_but,       EVENT_PRESS | EVENT_RELEASE);
  //  SimbleeForMobile.setEvents(fRainbow_but,     EVENT_PRESS | EVENT_RELEASE);
  //  SimbleeForMobile.setEvents(fRandom_but,      EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(candle_but,       EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(theater_but,      EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(christmas_but,    EVENT_PRESS | EVENT_RELEASE);

  SimbleeForMobile.endScreen();

}


void ui_event(event_t &event)
{

  if (event.type == EVENT_PRESS) {
    if (event.id == off_but) {
      digitalWrite(BLUE_LED, LOW);

    } else if (event.id == solid_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == pulse_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == rainbowTime_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == rainbowSpace_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == fColor_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == fRainbow_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == fRandom_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == candle_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == theater_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == christmas_but) {
      digitalWrite(BLUE_LED, HIGH);

    } else if (event.id == scanner_but) {
      digitalWrite(BLUE_LED, HIGH);

    }
  }

}

void send_change(uint8_t indicator, uint8_t value) {
  Serial.write(SYNC_BYTE);
  Serial.write(indicator);
  Serial.write(value);
}

