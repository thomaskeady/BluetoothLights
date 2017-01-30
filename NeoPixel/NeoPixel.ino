// Neopixel class produced by Adafruit https://learn.adafruit.com/multi-tasking-the-arduino-part-3/overview

#include <Adafruit_NeoPixel.h>

#define PIN 5

///////////////////////////////////////////// // Communication variables

#define PATTERN 0x00      // What pattern:
#define BRIGHTNESS 0x01 //
#define RATE 0x02       // How fast
#define SIZE 0x03        // How large
#define COLOR_R 0x04      // What color
#define COLOR_G 0x05      // What color
#define COLOR_B 0x06      // What color

#define NO_PATTERN 0x00
#define RAINBOW_TIME 0x01
#define RAINBOW_SPACE 0x02
#define FLASH_COLOR 0x03
//#define FLASH_RAINBOW 0x04
//#define FLASH_RANDOM 0x05
#define PULSE 0x06
#define THEATER 0x07
#define CHRISTMAS 0x08
#define CANDLE 0x09
#define SOLID_COLOR 0x0A
#define SCAN 0x0B

// IF EDIT THE ABOVE MAKE SURE TO CHANGE CORRESPONDING FILE AS WELL!!!

uint8_t SYNC_BYTE = 0xAA;

///////////////////////////////////////////// // End communication variables


// Pattern types supported:
enum  pattern {  NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, SOLID };


// Patern directions supported:
enum  direction { FORWARD, REVERSE };


// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    void (*OnComplete)();  // Callback on completion of pattern

    //////// Added by me
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    //////// End added by me

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      OnComplete = callback;
    }

    // Update the pattern
    void Update()
    {
      if ((millis() - lastUpdate) > Interval) // time to update
      {
        lastUpdate = millis();
        switch (ActivePattern)
        {
          case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
          case THEATER_CHASE:
            TheaterChaseUpdate();
            break;
          case COLOR_WIPE:
            ColorWipeUpdate();
            break;
          case SCANNER:
            ScannerUpdate();
            break;
          case FADE:
            FadeUpdate();
            break;
          case SOLID:
            SolidUpdate();
            break;
          default:
            break;
        }
      }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
      if (Direction == FORWARD)
      {
        Index++;
        if (Index >= TotalSteps)
        {
          Index = 0;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
      else // Direction == REVERSE
      {
        --Index;
        if (Index <= 0)
        {
          Index = TotalSteps - 1;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
    }

    // Reverse pattern direction
    void Reverse()
    {
      if (Direction == FORWARD)
      {
        Direction = REVERSE;
        Index = TotalSteps - 1;
      }
      else
      {
        Direction = FORWARD;
        Index = 0;
      }
    }

    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = RAINBOW_CYCLE;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
    }

    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
      }
      show();
      Increment();
    }

//////////////////////////////////// Start of my patterns

    // Sets the strip's Color1 to the combination of 8 bit red, green and blue
    void setColor1() {
      Color1 = Color(red, green, blue);
    }

    void setColor2() {
      Color2 = Color(red, green, blue);
    }

    void resetStrip() {
/*      myStrip.ActivePattern = NONE;
      myStrip.Direction = FORWARD;
      myStrip.Interval = 100;
      myStrip.Color1 = myStrip.Color(0, 0, 0);
      myStrip.TotalSteps = 255;
      myStrip.Index = 0;*/
      ActivePattern = NONE;
      Direction = FORWARD;
      Interval = 100;
      Color1 = Color(0, 0, 0);
      TotalSteps = 255;
      Index = 0;
    }


    void Solid() {
      ActivePattern = SOLID;
      Interval = 5;
      TotalSteps = 2; // Doesnt matter right?
      Index = 0;
      Direction = FORWARD;
    }

    void SolidUpdate() {
      for (int i = 0; i < numPixels(); i++) {
        setPixelColor(i, red, green, blue);
      }
      show();
      Increment();
    }





//////////////////////////////////// End of my patterns


    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = THEATER_CHASE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if ((i + Index) % 3 == 0)
        {
          setPixelColor(i, Color1);
        }
        else
        {
          setPixelColor(i, Color2);
        }
      }
      show();
      Increment();
    }

    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = COLOR_WIPE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color;
      Index = 0;
      Direction = dir;
    }

    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
      setPixelColor(Index, Color1);
      show();
      Increment();
    }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval)
    {
      ActivePattern = SCANNER;
      Interval = interval;
      TotalSteps = (numPixels() - 1) * 2;
      Color1 = color1;
      Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if (i == Index)  // Scan Pixel to the right
        {
          setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) // Scan Pixel to the left
        {
          setPixelColor(i, Color1);
        }
        else // Fading tail
        {
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }
      show();
      Increment();
    }

    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = FADE;
      Interval = interval;
      TotalSteps = steps;
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Fade Pattern
    void FadeUpdate()
    {
      // Calculate linear interpolation between Color1 and Color2
      // Optimise order of operations to minimize truncation error
      uint8_t red1 = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
      uint8_t green1 = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
      uint8_t blue1 = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

      ColorSet(Color(red1, green1, blue1));
      show();
      Increment();
    }

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, color);
      }
      show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;
      if (WheelPos < 85)
      {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }
};

void myStripComplete();
//void Ring2Complete();
//void StickComplete();



// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
//NeoPatterns myStrip(150, PIN, NEO_GRB + NEO_KHZ800, &Ring1Complete);
NeoPatterns myStrip(150, PIN, NEO_GRB + NEO_KHZ800, &myStripComplete);

// Initialize everything and prepare to start
void setup()
{
  Serial.begin(57600);

  // Initialize all the pixelStrips
  myStrip.begin();

  myStrip.resetStrip(); // All off

  // Kick off a pattern
  //myStrip.TheaterChase(myStrip.Color(255, 255, 0), myStrip.Color(0, 0, 50), 100);
  //myStrip.ColorWipe(myStrip.Color(255, 255, 255), 100);
  //myStrip.Scanner(myStrip.Color(255, 255, 255), 50);
  //myStrip.Fade(myStrip.Color(0, 0, 0), myStrip.Color(255, 255, 255), 50, 50);

}

byte btype;
byte indicator;
byte value;

// Main loop
void loop()
{
  // Update the rings.
  myStrip.Update();

  if (Serial.available() >= 3) {
    btype = Serial.read();
    if (btype != 0xAA) {  // Not the sync byte
      // Serial.flush??
      while (Serial.available()) {
        Serial.read();
      }
    } else {  // Is the sync byte
      indicator = Serial.read();  // 2nd byte
      value = Serial.read();  // 3rd byte

      switch (indicator) {
        case PATTERN:
          changePattern(value);

          break;
        case BRIGHTNESS:
          // For now do nothing

          break;
        case RATE:
          changeRate(value);

          break;
        case SIZE:
          changeSize(value);

          break;
        case COLOR_R:
          myStrip.red = value;
          myStrip.setColor1();

          break;
        case COLOR_G:
          myStrip.green = value;
          myStrip.setColor1();

          break;
        case COLOR_B:
          myStrip.blue = value;
          myStrip.setColor1();

          break;

      }



    }
  }

}

void changePattern(uint8_t value) {
  switch (value) {
    case NO_PATTERN:
      myStrip.ActivePattern = NONE;

      break;
    //    case RAINBOW_TIME:
    //      myStrip.ActivePattern =
    //
    //      break;
    case RAINBOW_SPACE:
      myStrip.ActivePattern = RAINBOW_CYCLE;

      break;
    //    case FLASH_COLOR:
    //      myStrip.ActivePattern =
    //
    //      break;
    case PULSE:
      myStrip.ActivePattern = FADE;

      break;
    case THEATER:
      myStrip.red = 0;
      myStrip.green = 0;
      myStrip.blue = 0;
      myStrip.setColor2();
    
      myStrip.ActivePattern = THEATER_CHASE;

      break;
    //    case CHRISTMAS:
    //      myStrip.ActivePattern =
    //
    //      break;
    //    case CANDLE:
    //      myStrip.ActivePattern =
    //
    //      break;
    case SOLID_COLOR:
      myStrip.ActivePattern = SOLID;

      break;
    case SCAN:
      myStrip.ActivePattern = SCANNER;

      break;
    case CHRISTMAS:
      myStrip.red = 255;
      myStrip.green = 0;
      myStrip.blue = 0;
      myStrip.setColor1();

      myStrip.red = 0;
      myStrip.green = 255;
      myStrip.blue = 0;
      myStrip.setColor2();
      
      myStrip.TheaterChase(myStrip.Color1, myStrip.Color2, myStrip.Interval);


      break;

  }

}


void changeRate(uint8_t value) {
  myStrip.Interval = value;
}

void changeSize(uint8_t value) {
  // Right now dont do anything
}

/*
     // Switch patterns on a button press:
     if (digitalRead(8) == LOW) // Button #1 pressed
     {
         // Switch Ring1 to FASE pattern
         Ring1.ActivePattern = FADE;
         Ring1.Interval = 20;
         // Speed up the rainbow on Ring2
         Ring2.Interval = 0;
         // Set stick to all red
         Stick.ColorSet(Stick.Color(255, 0, 0));
     }
     else if (digitalRead(9) == LOW) // Button #2 pressed
     {
         // Switch to alternating color wipes on Rings1 and 2
         Ring1.ActivePattern = COLOR_WIPE;
         Ring2.ActivePattern = COLOR_WIPE;
         Ring2.TotalSteps = Ring2.numPixels();
         // And update tbe stick
         Stick.Update();
     }
     else // Back to normal operation
     {
         // Restore all pattern parameters to normal values
         Ring1.ActivePattern = THEATER_CHASE;
         Ring1.Interval = 100;
         Ring2.ActivePattern = RAINBOW_CYCLE;
         Ring2.TotalSteps = 255;
         Ring2.Interval = min(10, Ring2.Interval);
         // And update tbe stick
         Stick.Update();
     }*/


//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------


// Ring1 Completion Callback
void myStripComplete()
{
  /*if (digitalRead(9) == LOW)  // Button #2 pressed
    {
    // Alternate color-wipe patterns with Ring2
    Ring2.Interval = 40;
    Ring1.Color1 = Ring1.Wheel(random(255));
    Ring1.Interval = 20000;
    }
    else  // Retrn to normal
    {*/
  myStrip.Reverse();
  //}
}

/*
  // Ring 2 Completion Callback
  void Ring2Complete()
  {
  if (digitalRead(9) == LOW)  // Button #2 pressed
  {
    // Alternate color-wipe patterns with Ring1
    Ring1.Interval = 20;
    Ring2.Color1 = Ring2.Wheel(random(255));
    Ring2.Interval = 20000;
  }
  else  // Retrn to normal
  {
    Ring2.RainbowCycle(random(0, 10));
  }
  }

  // Stick Completion Callback
  void StickComplete()
  {
  // Random color change for next scan
  Stick.Color1 = Stick.Wheel(random(255));
  }

*/
