/*
This Simblee sketch demonstrates a full bi-directional Bluetooth Low
Energy 4 connection between an iPhone application and an Simblee.

This sketch works with SimbleeForMobile.

The button on the iPhone can be used to turn the green led on or off.
The button state of button 1 is transmitted to the iPhone and shown in
the application.
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

// pin 3 on the RGB shield is the red led
// (can be turned on/off from the iPhone app)
int led = 3;

// pin 5 on the RGB shield is button 1
// (button press will be shown on the iPhone app)
int button = 5;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

void setup() {
  // led turned on/off from the iPhone app
  pinMode(led, OUTPUT);

  // button press will be shown on the iPhone app)
  pinMode(button, INPUT);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  SimbleeForMobile.advertisementData = "ledbtn";

  // use a shared cache
  SimbleeForMobile.domain = "Simblee.com";

  SimbleeForMobile.begin();
}

int debounce(int state)
{
  int start = millis();
  int debounce_start = start;

  while (millis() - start < debounce_timeout)
    if (digitalRead(button) == state)
    {
      if (millis() - debounce_start >= debounce_time)
        return 1;
    }
    else
      debounce_start = millis();

  return 0;
}

int delay_until_button(int state)
{
  // set button edge to wake up on
  if (state)
    Simblee_pinWake(button, HIGH);
  else
    Simblee_pinWake(button, LOW);

  do
    // switch to lower power mode until a button edge wakes us up
    Simblee_ULPDelay(INFINITE);
  while (! debounce(state));

  // if multiple buttons were configured, this is how you would determine what woke you up
  if (Simblee_pinWoke(button))
  {
    // execute code here
    Simblee_resetPinWake(button);
  }
}

//uint8_t ui_button;
uint8_t ui_rect;
uint8_t rect1;
uint8_t rect2;
uint8_t rect3;

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

void loop() {
  // todo: delay_until_button(HIGH);
  // todo: SimbleeBLE.send(1);

  // todo: delay_until_button(LOW);
  // todo: SimbleeBLE.send(0);

  if (SimbleeForMobile.updatable)
    SimbleeForMobile.updateColor(ui_rect, digitalRead(button) ? GREEN : WHITE);

  // process must be called in the loop for SimbleeForMobile
  SimbleeForMobile.process();
}

void SimbleeForMobile_onDisconnect()
{
  // don't leave the led on if they disconnect
  digitalWrite(led, LOW);
}





void ui()
{
  SimbleeForMobile.beginScreen(WHITE);

    SimbleeForMobile.drawText(60, 90, ".");

  // I prefer the button style below, uncomment the line following for the original button style
  //  ui_button = SimbleeForMobile.drawButton(120, 180, 80, "Button");
  //SimbleeForMobile.drawButton(120, 180, 80, "Button", BLUE, TEXT_TYPE);

  off_but =          SimbleeForMobile.drawButton(10, 60, 93, "OFF", BLUE);
  solid_but =        SimbleeForMobile.drawButton(113, 60, 94, "Solid", BLACK);
  pulse_but =        SimbleeForMobile.drawButton(217, 60, 93, "Pulse", BLACK);
  rainbowTime_but =  SimbleeForMobile.drawButton(10, 105, 145, "Time Rainbow", BLACK);
  rainbowSpace_but = SimbleeForMobile.drawButton(165, 105, 145, "Space Rainbow", BLACK);
  fColor_but =       SimbleeForMobile.drawButton(10, 150, 145, "Flash Color", BLACK);
  fRainbow_but =     SimbleeForMobile.drawButton(165, 150, 145, "Flash Rainbow", BLACK);
  fRandom_but =      SimbleeForMobile.drawButton(10, 195, 145, "Flash Random", BLACK);
  candle_but =       SimbleeForMobile.drawButton(165, 195, 145, "Candle", BLACK);
  theater_but =      SimbleeForMobile.drawButton(10, 240, 145, "Theater Chase", BLACK);
  christmas_but =    SimbleeForMobile.drawButton(165, 240, 145, "Christmas", BLACK);
  
  SimbleeForMobile.setEvents(off_but,          EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(solid_but,        EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(pulse_but,        EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(rainbowTime_but,  EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(rainbowSpace_but, EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(fColor_but,       EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(fRainbow_but,     EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(candle_but,       EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(theater_but,      EVENT_PRESS | EVENT_RELEASE);
  SimbleeForMobile.setEvents(christmas_but,    EVENT_PRESS | EVENT_RELEASE);

  SimbleeForMobile.endScreen();
}

void ui_event(event_t &event)
{
  if (event.id == off_but)
    if (event.type == EVENT_PRESS)
      digitalWrite(led, HIGH);
    else if (event.type == EVENT_RELEASE)
      digitalWrite(led, LOW);
}

