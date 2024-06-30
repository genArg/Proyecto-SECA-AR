#define INTERRUPCION_PULSE (19)
#define INTERRUPCION_C1 (20)
#define INTERRUPCION_C2 (21)
#define TIME_INT_PULSE (1000)  //TIEMP ENTRE DOS POSBLES INTERRUPCIONES DEL MISMO PIN
#define TIME_INT_C1 (1000)
#define TIME_INT_C2 (1000)

#define PIN_READY (31)
#define PIN_HAPPY_LED (37)
#define TIME_HAPPY_LED (10000)


#define PIN_PRESION (A13)
#define PIN_NIVEL (A15)

int led_estado = 1;
int happy_led_estado = 1;
int a_pulse, a_c1, a_c2;
int b_pulse, b_c1, b_c2;
int contador_int_pulse = 0;
int contador_int_c1 = 0;
int contador_int_c2 = 0;
int contador_happy_led = 0;
int contador = 100;

uint8_t pin_estado_pulse, pin_estado_c1, pin_estado_c2, pin_auxiliar;

void setup() {
  Serial.begin(9600);

  // Configurar el pin 13 como salida
  pinMode(LED_BUILTIN, OUTPUT);
  // Configurar el pin PIN_READY como entrada
  pinMode(PIN_READY, INPUT_PULLUP);
  // Configurar el pin PIN_HAPPY_LED como salida
  pinMode(PIN_HAPPY_LED, OUTPUT);
  // Configurar el pines de interrupcion como entreadas
  pinMode(INTERRUPCION_PULSE, INPUT);  // pulsador -> forzar medicion
  pinMode(INTERRUPCION_C1, INPUT);     // caudalimetro 1
  pinMode(INTERRUPCION_C2, INPUT);     // caudalimetro 2

  // inicia los estados de los pines de interrupcion
  pin_estado_pulse = digitalRead(INTERRUPCION_PULSE);
  pin_estado_c1 = digitalRead(INTERRUPCION_C1);
  pin_estado_c2 = digitalRead(INTERRUPCION_C2);

  // Habilitar la interrupción en el pin 21
  // attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
  // mode puede ser: LOW, CHANGE, RISING, FALLING, HIGH
  attachInterrupt(digitalPinToInterrupt(INTERRUPCION_PULSE), ISR_Pin, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPCION_C1), ISR_Pin, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPCION_C2), ISR_Pin, RISING);
  a_pulse = 1;
  a_c1 = 1;
  a_c2 = 1;

  b_pulse = 1;
  b_c1 = 1;
  b_c2 = 1;
}

void loop() {
  // Tu código aquí
  delay(1);

  contador_int_pulse = (contador_int_pulse + 1) % TIME_INT_PULSE;
  contador_int_c1 = (contador_int_c1 + 1) % TIME_INT_C1;
  contador_int_c2 = (contador_int_c2 + 1) % TIME_INT_C2;

  if (contador_int_pulse == 0) {
    a_pulse = 1;
  }
  if (contador_int_c1 == 0) {
    a_c1 = 1;
  }
  if (contador_int_c2 == 0) {
    a_c2 = 1;
  }

  if ((a_pulse == 0) && (b_pulse == 1)) {
    led_estado = (led_estado) ? 0 : 1;
    digitalWrite(LED_BUILTIN, led_estado);
    b_pulse = 0;
    Serial.println("interrupcion 19");
  }

  if ((a_c1 == 0) && (b_c1 == 1)) {
    //digitalWrite(LED_BUILTIN, 1);
    b_c1 = 0;
    contador++;
    Serial.println("interrupcion 20");
    Serial.println(contador);
  }

  if ((a_c2 == 0) && (b_c2 == 1)) {
    //digitalWrite(LED_BUILTIN, 0);
    b_c2 = 0;
    contador--;
    Serial.println("interrupcion 21");
    Serial.println(contador);
  }

  if (digitalRead(PIN_READY)) {

    if(contador_int_pulse == 0){ //------------------> condicion para que no se repita demasido rapido
// codigo para leer las entradas analogicas
    Serial.print("PRESION: ");
    Serial.println(analogRead(PIN_PRESION));
    Serial.print("NIVEL: ");
    Serial.println(analogRead(PIN_NIVEL));
    }
    
    // codigo para el happy led
    contador_happy_led = (contador_happy_led + 1) % TIME_INT_PULSE;
    if (contador_happy_led == 0) {
      happy_led_estado = (happy_led_estado) ? 0 : 1;
    }
    digitalWrite(PIN_HAPPY_LED, happy_led_estado);
  }
}



// Definir la función de interrupción
void ISR_Pin() {
  if (pin_estado_pulse != (pin_auxiliar = digitalRead(INTERRUPCION_PULSE))) {
    pin_estado_pulse = pin_auxiliar;
    if (a_pulse && pin_estado_pulse) {
      a_pulse = 0;
      b_pulse = 1;
      contador_int_pulse = 1;
    }
  }

  if (pin_estado_c1 != (pin_auxiliar = digitalRead(INTERRUPCION_C1))) {
    pin_estado_c1 = pin_auxiliar;
    if (a_c1 && pin_estado_c1) {
      a_c1 = 0;
      b_c1 = 1;
      contador_int_c1 = 1;
    }
  }

  if (pin_estado_c2 != (pin_auxiliar = digitalRead(INTERRUPCION_C2))) {
    pin_estado_c2 = pin_auxiliar;
    if (a_c2 && pin_estado_c2) {
      a_c2 = 0;
      b_c2 = 1;
      contador_int_c2 = 1;
    }
  }
}
