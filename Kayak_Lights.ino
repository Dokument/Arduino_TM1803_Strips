// Use this code however you like. Lots of it was taken from FastLED.h examples.
// 2016 Adam Melton

#include <FastLED.h>

const int  BUTTON_PIN = 2;    // the pin that the pushbutton is attached to

#define LED_PIN     3 // the pin that the LED strip is attached to
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    TM1803
#define COLOR_ORDER RBG
CRGB leds[NUM_LEDS];

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
    pinMode(BUTTON_PIN, INPUT);
    Serial.begin(9600);      // open the serial port at 9600 bps: 
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( CoolWhiteFluorescent  );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop() {
    // read the pushbutton input pin:
    buttonState = digitalRead(BUTTON_PIN);
  
    if (buttonState == HIGH) {
        // if the current state is HIGH then the button
        // went from off to on:
   
        // Delay a little bit to avoid bouncing
        delay(200);
        if (digitalRead(BUTTON_PIN) == HIGH) {
            // If still high then start adjusting brightness until buttonState low
            int temp_brightness = 0;
            fill_solid( currentPalette, 16, CRGB::Red);
 
            while(digitalRead(BUTTON_PIN) == HIGH){
                temp_brightness = temp_brightness + 5;
                if( temp_brightness == 250) {
                  // Blink just before max value reached
                  fill_solid( currentPalette, 16, CRGB::Black);
                  pushChanges();
                  delay(1000);
                }
                else if( temp_brightness >= 255) {
                  //Reset to 5
                  temp_brightness = 5;
                }
                Serial.println(temp_brightness);
                FastLED.setBrightness(  temp_brightness );
                fill_solid( currentPalette, 16, CRGB::Red);
                pushChanges();
                delay(50);
            }
            fill_solid( currentPalette, 16, CRGB::Black);
            delay(150);
        }
        else {
            // Only change to next sequence on short button presses
            buttonPushCounter++;
            Serial.println(buttonPushCounter);
        }
    }
    ChangePaletteOnButtonPress();
    pushChanges();
}

void pushChanges(){
    // pushes changes to the led strip
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; // motion speed
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = NUM_LEDS -1; i >= 0; i--) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePaletteOnButtonPress()
{ 
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;

      // Removed, can be used later if desired
      // else if( buttonPushCounter == 1)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
      // else if( buttonPushCounter == 5)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
      // else if( buttonPushCounter == 9)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
      // else if( buttonPushCounter == 11)  { SetupBlink();                            currentBlending = NOBLEND; }
      // else if( buttonPushCounter == 2)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
    
      if( buttonPushCounter == 0)  { fill_solid( currentPalette, 16, CRGB::Red); currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 1)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 2)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 3)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 4)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 5)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 6)  { America();                                currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 7)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 8)  { SetupBlackAndWhiteStripedPalette();             currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 9)  { fill_solid( currentPalette, 16, CRGB::White); currentBlending = LINEARBLEND; }
      else if( buttonPushCounter == 10)  { paletteTimeLoop(secondHand);  /*transitions between paletts in function*/} 
      
      else { buttonPushCounter = 0; }
    }
}

void paletteTimeLoop(int secondHand){
    // Transition between palettes at every X second
    Serial.println(secondHand);
    if( secondHand <=  14)      { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    else if( secondHand <= 29)  { America();                                currentBlending = LINEARBLEND; }
    else if( secondHand <= 41)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND; }
    else if( secondHand <= 53)  { America();                                currentBlending = NOBLEND; }
    else if( secondHand <= 59)  { fill_solid( currentPalette, 16, CRGB::Red); currentBlending = LINEARBLEND; }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

void America()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to xxx.
    currentPalette[0] = CRGB::Red;
    currentPalette[1] = CRGB::Red;
    currentPalette[2] = CRGB::Red;
    currentPalette[3] = CRGB::Red;
    
    currentPalette[5] = CRGB::White;
    currentPalette[6] = CRGB::White;
    currentPalette[7] = CRGB::White;
    currentPalette[8] = CRGB::White;
    
    currentPalette[10] = CRGB::Blue;
    currentPalette[11] = CRGB::Blue;
    currentPalette[12] = CRGB::Blue;
    currentPalette[13] = CRGB::Blue;
    
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::White,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::White,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::White,
    CRGB::White,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
