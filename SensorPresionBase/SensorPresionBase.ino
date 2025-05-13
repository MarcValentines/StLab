#include <FastLED.h>

//leds
#define LED_PIN 6
#define NUM_LEDS 64
#define LED_TYPE SK6812
#define COLOR_ORDER GRB

CGRB leds[NUM_LEDS];

//sensor
#define SENSOR_PIN A0
float presionBase = 0;
unsigned int index = 0;
unsigned int maxEncender = 0;
bool juegoActivo = false;

//electroimanes
#define ELECTROIMAN_1 8
#define ELECTROIMAN_2 9

void setup() {
  pinMode(ELECTROIMAN_1, OUTPUT);
  pinMode(ELECTROIMAN_2, OUTPUT);
  digitalWrite(ELECTROIMAN_1, LOW);
  digitalWrite(ELECTROIMAN_2, LOW);
  Serial.begin(9600);

  //config leds
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(120);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  // config sensor
  delay(500);
  presionBase = leerPresion();  //presión base
}

void loop() {
  //Leer la presión en cada ciclo
  float presion = leerPresion();

  / /alcular la diferencia con la presión base
  float dif = presionBase - presion;
  
  //Mapear el cambio de presión a los LEDs
  maxEncender = map(dif * 100, 0, 500, 0, NUM_LEDS); //Multiplicar por 100 para obtener un rango más amplio

  //Si el juego no ha comenzado, hacer parpadear el primer LED
  if (index == 0 && presion < 1.0) {
    indicarJuegoActivo();
    delay(500);
    return;  //Salir del loop hasta que la presión cambie
  }

  //Encender LEDs dependiendo de la presión
  if (index < maxEncender && index < NUM_LEDS) {
    uint8_t tono = map(index, 0, NUM_LEDS - 1, 0, 96);
    leds[index] = CHSV(tono, 255, 255);
    index++;
  } else if (index > maxEncender && index > 0) {
    leds[index] = CRGB::Black;
    index--;
  }

  //Mostrar los LEDs
  FastLED.show();
  delay(800);

  //Abrir los electroimanes si todos los LEDs están encendidos
  if (index >= NUM_LEDS) {
    abrirElectroimanes();
  }
}

// Función para leer la presión desde el sensor
float leerPresion() {
  int lectura = analogRead(SENSOR_PIN);  //Leer el valor analógico del sensor
  float voltaje = lectura * (5.0 / 1023.0);  //Convertir a voltaje (0-5V)
  float presionPSI = (voltaje - 0.5) * (5.0 / 4.0);  //Convertir voltaje a presión (0-5 psi)
  if (presionPSI < 0) presionPSI = 0;  //Evitar valores negativos por ruido
  return presionPSI;  //Retornar la presión en PSI
}

//Función que hace parpadear el primer LED para indicar que el juego está activo
void indicarJuegoActivo() {
  static bool estado = false;
  leds[0] = estado ? CRGB::Blue : CRGB::Black;  //Alternar entre azul y apagado
  FastLED.show();
  estado = !estado;  //Cambiar el estado para la próxima vez
}

// Función para abrir los electroimanes (por ejemplo, con un relé)
void abrirElectroimanes() {
  digitalWrite(ELECTROIMAN_1, HIGH);
  digitalWrite(ELECTROIMAN_2, HIGH);
}
