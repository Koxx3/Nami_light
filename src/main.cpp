#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN PD3
#define NB_LEDS 22

#define TURN_CYCLE_IN_MILLIS 1500

// Declare our NeoPixel strip object:
Adafruit_NeoPixel *strip;

bool turnLeftPressed = false;
bool turnLeftLedsOn = false;
bool oldTurnLeftPressed = false;
bool lightPressed = false;
bool oldLightPressed = false;

uint32_t startTurnLeftTime = 0;
uint32_t startLightTime = 0;

uint32_t lightColor;
uint32_t brakeColor;

void setup()
{
  Serial.begin(115200);

  Serial.println("setup begin");

  strip = new Adafruit_NeoPixel(NB_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);
  strip->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip->show();  // Turn OFF all pixels ASAP
  strip->setBrightness(20);

  Serial.println("setup end");
}

void loop()
{

  // detect button press change to "pressed"
  if ((turnLeftPressed == true) && (oldTurnLeftPressed == false))
    startTurnLeftTime = millis();
  oldTurnLeftPressed = turnLeftPressed;

  if ((lightPressed == true) && (oldLightPressed == false))
    startLightTime = millis();
  oldLightPressed = lightPressed;

  // set color for light
  lightColor = (millis() - startLightTime) / 10;
  if (lightColor > 255)
    lightColor = 255;
  brakeColor = (millis() - startLightTime) / 10;
  if (brakeColor > 255)
    brakeColor = 255;

  // process left turn state
  if (turnLeftPressed)
  {
    if ((millis() - startTurnLeftTime) % TURN_CYCLE_IN_MILLIS > TURN_CYCLE_IN_MILLIS / 2)
    {
      turnLeftLedsOn = false;
    }
    else
    {
      turnLeftLedsOn = true;
    }
  }
  Serial.println("millis = " + (String)millis() + " / nextTurnLeftOn = " + (String)startTurnLeftTime + " / turnLeftLedsOn = " + (String)turnLeftLedsOn + " / startLightTime = " + (String)startLightTime + " / lightColor = " + (String)lightColor);

  // Push led states
  // sector A
  for (int i = 0; i < 10; i++)
  {
    if (turnLeftPressed)
    {
      if (turnLeftLedsOn)
      {
        strip->setPixelColor(i, strip->Color(0, 255, 0, 10));
      }
      else
      {
        strip->setPixelColor(i, strip->Color(0, 0, 0, 10));
      }
    }
    else if (lightPressed)
    {
      strip->setPixelColor(i, strip->Color(lightColor, lightColor, lightColor, 10));
    }
    else
    {
      strip->setPixelColor(i, strip->Color(0, 0, 0, 10));
    }
  }

  // sector B
  for (int i = 10; i < 20; i++)
  {

    if (turnLeftPressed)
    {
      if (turnLeftLedsOn)
      {
        strip->setPixelColor(i, strip->Color(0, 255, 0, 10));
      }
      else
      {
        strip->setPixelColor(i, strip->Color(0, 0, 0, 10));
      }
    }
    else if (lightPressed)
    {
      strip->setPixelColor(i, strip->Color(brakeColor, 0, 0, 10));
    }
    else
    {
      strip->setPixelColor(i, strip->Color(0, 0, 0, 10));
    }
  }

  strip->show();

  // Serial.print(".");

  delay(5);

  // button press simulation
  if (millis() > 1000)
  {
    lightPressed = true;
    turnLeftPressed = false;
  }

  if (millis() > 10000)
  {
    lightPressed = true;
    turnLeftPressed = true;
  }
}