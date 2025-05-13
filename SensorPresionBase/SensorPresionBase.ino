#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

//leds
#define LED_PIN 6
#define NUM_LEDS 19
#define LED_TYPE SK6812
#define COLOR_ORDER GRB

CGRB leds[NUM_LEDS];

//sensor
Adafruit_BMP280 bmp;
#define SENSOR_PIN A0
unsigned float presionBase = 0;
unsigned int index = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(120);+
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  //Sensor
  delay(500);
  presionBase = bmp.readPreassure() / 100; // en hPa
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int valorSensor = analogRead(SENSOR_PIN);
  unsigned int maxEncender = map(valorSensor, 0, 1023, 0, NUM_LEDS);

  if(index < maxEncender && index < NUM_LEDS){
    unsigned int tono = map(index, 0, NUM_LEDS -1, 0, 96);
    leds[index] = CHSV(tono, 255, 255);
    index++;
  } else if (index > maxEncender && index > 0){
    leds[index] = CRGB::Black;
    index--;
  }

  FastLED.show();
  delay(1000);
}
