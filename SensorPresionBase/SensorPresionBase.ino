#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

//leds
#define LED_PIN 6
#define NUM_LEDS 64
#define LED_TYPE WS2815
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

//sensor
Adafruit_BMP280 bmp;
#define SENSOR_PIN A0
float presionBase = 0;
unsigned int index = 0;

//electroimanes
#define ELECTROIMAN_1 9
#define ELECTROIMAN_2 8

void setup() {
  // put your setup code here, to run once:
  //electroimanes
  pinMode(ELECTROIMAN_1, OUTPUT);
  pinMode(ELECTROIMAN_2, OUTPUT);
  digitalWrite(ELECTROIMAN_1, LOW); //iniciarlos en bajo
  digitalWrite(ELECTROIMAN_2, LOW);
  //comunicación serial
  Serial.begin(9600);
  //FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(120);+
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  //Sensor
  delay(500);
  presionBase = bmp.readPressure() / 100; // en hPa
  bool sensorDisponible = bmp.begin(0x76); //dirección I2C de ejemplo (sustituir por la real)
}

void loop() {
  // put your main code here, to run repeatedly:

  if(sensorDisponible){
    float presion = bmp.readPressure() / 100;
    //parpadeo primer led mientras no se sopla
    if (index == 0 && presion < 1.0){
      indicarJuegoActivo();
      delay(500);
      return; //salir del loop aquí y no ejecutar el resto del código
    }
    //calcular diferéncia respecto a la presión base
    float dif = presionBase - presion;

    //mapear el cambio de leds según la presión
    din = constrain(dif, 0, 5); //limitar el valor para evitar errores
    unsigned int maxEncender = map(dif * 100, 0, 500, 0, NUM_LEDS); //multiplicar por 100 porque map() requiere enteros

    if(index < maxEncender && index < NUM_LEDS){
      uint8_t tono = map(index, 0, NUM_LEDS -1, 0, 96);
      leds[index] = CHSV(tono, 255, 255);
      index++;
    } else if (index > maxEncender && index > 0){
      leds[index] = CRGB::Black;
      index--;
    }

    if(index => NUM_LEDS){
      digitalWrite(ELECTROIMAN_1, HIGH);
      digitalWrite(ELECTROIMAN_2, HIGH);
    }

    FastLED.show();
    delay(800);
  }
}

void indicarJuegoActivo(){
  static bool estado = false

  leds[0] = estado ? CRGB::Blue : CRGB::Black; //alterar entre azul y apagado
  FastLED.show();
  estado = !estado; //cambiar estado para la proxima llamada
}
