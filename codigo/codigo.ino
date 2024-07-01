#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include <TimerOne.h>
#include "define.h"
#include "caudal.h"
#include "tiempo.h"
#include "presion.h"


#define PULSE_INT ((a_pulse == 0) && (b_pulse == 1))

//--------------------------------------------------------------------------------------------------------------------
// Pantalla
// Calibración de la pantalla táctil para landscape
const int XP = 8, XM = A2, YP = A3, YM = 9;  // Pines del touch panel
//portrait
//const int TS_LEFT = 67, TS_RT = 897, TS_TOP = 924, TS_BOT = 103;
//landscape
const int TS_LEFT = 70, TS_RT = 893, TS_TOP = 918, TS_BOT = 10;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Botones para la matriz de 5x2 de la pantalla_1
Adafruit_GFX_Button btn_matrix[12];  // Botones 5 filas x 2 columnas
const char* matrix_labels[12] = { "Hora", "Config", "Presion", "x", "Caudal", "x", "Nivel Estatico", "x", "Nivel Dinamico", "x", "Q Especifico", "x" };


// Botones para la pantalla_2
// Botones para el teclado numérico
Adafruit_GFX_Button btn_digits[12];  // Botones 0-9, OK, y BACK
const char* labels[12] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "BK", "0", "OK" };

// Botones para la mitad derecha
Adafruit_GFX_Button right_btns[4];
const char* right_labels[4] = { "Q meter", "Parametro", "Valor", "N_valor" };

int pixel_x, pixel_y;  // Variables globales para almacenar las coordenadas del toque
int pant;              // para intercambiar entre pantalla
int num_aux;           //usado para el teclado numerico
char buffer[20];       // Buffer para almacenar la cadena

//
//--------------------------------------------------------------------------------------------------------------------
caudalimetro_t caudalimetro;  // crea un caudalimetro
char mensaje[] = "mensaje";
char letra;
static manometro_t nivel_alto;
static manometro_t nivel_bajo;
static manometro_t manometro_1;
static caudalimetro_t caudalimetro_1;
static caudalimetro_t caudalimetro_2;
static reloj_t reloj_1;
// para tener un frame por segundo
uint8_t aux_refresh = 0;
// contador delta de tiempo entre mediciones
uint16_t contador_compresor = 1;
uint16_t contador_200ms = 1;
uint16_t contador_1s = 1;
uint16_t contador_10s = 1;
// bit para iniciar el tiempo de espera hasta tomar la medicion
uint8_t bit_compresor = 0;
// cantidad de niveles dinamicos sobre cantidad de valores estaticos
uint8_t i_dif = 0;
//contador para esperar 10 segundos  antes de tomar medicion
uint16_t timer_compresor = 1;
// bit para iniciar la toma la mediciones
uint8_t bit_segundos_compresor = 0;
uint16_t contador_1m = 1;
uint8_t bit_1m = 0;
// para actualizar los datos de la pantalla
uint8_t bit_hora = 1;
// bit para actualizar el valor en pantalla
uint8_t bit_caudal = 0;
//!> bit para actualizar el dato en pantalla
uint8_t bit_presion = 0;
uint8_t bit_nivel_dinamico = 0;
uint8_t bit_nivel_estatico = 0;
uint8_t bit_caudal_especifico = 0;
uint8_t bit_cambio = 0;
float caudal_especifico = 0;
float caudal_media = 0;
float delta_altura = 0;

//--------------------------------------------------------------------------------------------------------------------
// para las entradas de interrupciones y demas
//usado en para le estado del led
int happy_led_estado = 1;
// bandera para tomar un tiempo entre interrupciones
int a_pulse, a_c1, a_c2;
// bandera para ejecutar la funciones de la interrupcion
int b_pulse, b_c1, b_c2;
int contador_int_pulse = 0;
int contador_int_c1 = 0;
int contador_int_c2 = 0;
int contador_happy_led = 0;
// estado del pulsador previo
uint8_t pin_estado_pulse, pin_estado_c1, pin_estado_c2, pin_auxiliar;


//--------------------------------------------------------------------------------------------------------------------
uint16_t valor_aux;  // varibale de pruebas

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(9600);
  Serial.println("OK");

  //--------------------------------------------------------------------------------------------------------------------
  Timer1.initialize(TIME_BASE);        // Inicializa el temporizador a 1 segundo (100000 µs)
  Timer1.attachInterrupt(FnCallback);  // Adjunta la función de callback

  //--------------------------------------------------------------------------------------------------------------------
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
  //--------------------------------------------------------------------------------------------------------------------
  // pantalla
  uint16_t ID = tft.readID();     // Lee el ID de la pantalla TFT
  if (ID == 0xD3D3) ID = 0x9486;  // Ajuste para pantallas de solo escritura
  tft.begin(ID);                  // Inicializa la pantalla TFT
  tft.setRotation(1);             // Configura la orientación de la pantalla (1 = landscape)
  tft.fillScreen(WHITE);          // Rellena la pantalla de blanco
  pant = 0;
  Pantalla_1();

  //--------------------------------------------------------------------------------------------------------------------

  nivel_alto = InicializarManometro();
  nivel_bajo = InicializarManometro();
  manometro_1 = InicializarManometro();
  caudalimetro_1 = InicializarCaudal();
  caudalimetro_2 = InicializarCaudal();
  reloj_1 = InicializarReloj();

  //apaga el compresor
  digitalWrite(PIN_OUT_COMPRESOR, LOW);  //apaga el compresor
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  //--------------------------------------------------------------------------------------------------------------------
  // medir nivel
  if (digitalRead(PIN_READY)) {
    if (bit_compresor) {
      digitalWrite(PIN_OUT_COMPRESOR, HIGH);  // enciende compresor
      if (bit_segundos_compresor) {           //esperar que el 10 segundos para realizar la lectura
        TomarValor(nivel_bajo, PIN_NIVEL);    //almacenar valor de nivel bajo
        i_dif++;                              //aumenta el contador de evento
        bit_compresor = 0;
        bit_segundos_compresor = 0;
        timer_compresor = 1;
        bit_nivel_dinamico = 1;
        digitalWrite(PIN_OUT_COMPRESOR, LOW);  //apaga el compresor
        bit_caudal_especifico = 1;
        //Serial.println("READY_ON");
      }
    }
  } else {
    if (i_dif > 3) {  //inicia de la toma de inmediato
      bit_compresor = 1;
      i_dif = 0;
    }
    if (bit_compresor) {                      /*contador de evento > 3*/
      digitalWrite(PIN_OUT_COMPRESOR, HIGH);  // enciende compresor
      if (bit_segundos_compresor) {           //esperar que el 10 segundos para realizar la lectura
        TomarValor(nivel_alto, PIN_NIVEL);    //almacenar valor de nivel alto
        i_dif = 0;                            //resetea el contador de evento
        bit_compresor = 0;
        bit_segundos_compresor = 0;
        timer_compresor = 1;
        bit_nivel_estatico = 1;
        digitalWrite(PIN_OUT_COMPRESOR, LOW);  //apaga el compresor
        bit_caudal_especifico = 1;
        //Serial.println("READY_OFF");
      }
    }
  }

  //--------------------------------------------------------------------------------------------------------------------
  // medir presion
  if (bit_1m) {
    TomarValor(manometro_1, PIN_PRESION);  //almacenar valor de nivel bajo
    bit_1m = 0;
    contador_1m = 1;
    bit_presion = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // realiza los calculos del caudal especifico
  CalcularQEspecifico();

  //--------------------------------------------------------------------------------------------------------------------
  //guarda en sd

  //--------------------------------------------------------------------------------------------------------------------
  // Pantalla
  if (pant) {
    Tactil_2();
    RefrescarPantalla_2();  // muestra los valores en pantalla
  } else {
    if (contador_200ms) {
      RefrescarPantalla_1();  // muestra los valores en pantalla
    }

    Tactil_1();
  }

  //--------------------------------------------------------------------------------------------------------------------
  FnsIrsAuxiliares();

  //--------------------------------------------------------------------------------------------------------------------
  digitalWrite(PIN_HAPPY_LED, happy_led_estado);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// funcion llama de interrupcion del temporizador
void FnCallback() {

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_200ms = (contador_200ms) ? 0 : 1)) {
  }

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_1s = (contador_1s + 1) % TIME_1S)) {
  }

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_10s = (contador_10s + 1) % TIME_10S)) {
    happy_led_estado = (happy_led_estado) ? 0 : 1;
    // aumenta en 1 segundo el reloj interno
    UnSegundoReloj(reloj_1);
    bit_hora = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  contador_happy_led = contador_1s;

  //--------------------------------------------------------------------------------------------------------------------
  // timer del compresor
  if (bit_compresor == 0) {
    contador_compresor = (contador_compresor + 1) % (100 * TIME_10S);  //TIME_VALOR_COMPRESOR debe ser 1 minuto;
  }
  if (contador_compresor == 0) {
    bit_compresor = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // timer para contar 10 seg despues de iniciar el compresor
  if (bit_compresor) {
    timer_compresor = (timer_compresor + 1) % TIME_10S;
  }
  if (timer_compresor == 0) {
    bit_segundos_compresor = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // timer para contar 1 min para tomar la presion
  contador_1m = (contador_1m + 1) % TIME_10S;
  if (contador_1m == 0) {
    bit_1m = 1;
    bit_caudal_especifico = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // timer para uso de interrpuciones
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
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FnsIrsAuxiliares() {
  if ((a_pulse == 0) && (b_pulse == 1)) {
    b_pulse = 0;
    Serial.println("interrupcion 19");
  }

  //--------------------------------------------------------------------------------------------------------------------
  if ((a_c1 == 0) && (b_c1 == 1)) {
    //digitalWrite(LED_BUILTIN, 1);
    b_c1 = 0;
    Serial.println("interrupcion 20");
    CalculoCaudal(caudalimetro_1);
    PromediarCaudal();
    if (pin_estado_c1) {
      bit_caudal = 1;
    }
  }

  //--------------------------------------------------------------------------------------------------------------------
  if ((a_c2 == 0) && (b_c2 == 1)) {
    //digitalWrite(LED_BUILTIN, 0);
    b_c2 = 0;
    Serial.println("interrupcion 21");
    CalculoCaudal(caudalimetro_2);
    PromediarCaudal();
    if (pin_estado_c2) {
      bit_caudal = 1;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definir la función de interrupción
void ISR_Pin() {
  if (pin_estado_pulse != (pin_auxiliar = digitalRead(INTERRUPCION_PULSE))) {
    pin_estado_pulse = pin_auxiliar;
    if (a_pulse && pin_estado_pulse) {
      a_pulse = 0;
      b_pulse = 1;
      contador_int_pulse = 1;
      bit_compresor = 1;
    }
  }

  if (pin_estado_c1 != (pin_auxiliar = digitalRead(INTERRUPCION_C1))) {
    pin_estado_c1 = pin_auxiliar;
    if (a_c1 && pin_estado_c1) {
      a_c1 = 0;
      b_c1 = 1;
      contador_int_c1 = 1;
      CaudalGuardarTiempo(caudalimetro_1, TomarTiempo(reloj_1));
    }
  }

  if (pin_estado_c2 != (pin_auxiliar = digitalRead(INTERRUPCION_C2))) {
    pin_estado_c2 = pin_auxiliar;
    if (a_c2 && pin_estado_c2) {
      a_c2 = 0;
      b_c2 = 1;
      contador_int_c2 = 1;
      CaudalGuardarTiempo(caudalimetro_2, TomarTiempo(reloj_1));
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcula promedio
void PromediarCaudal() {
  if (caudalimetro_1->caudal_promedio && caudalimetro_2->caudal_promedio) {
    caudal_media = (caudalimetro_1->caudal_promedio + caudalimetro_2->caudal_promedio) / 2;
  } else if (caudalimetro_1->caudal_promedio) {
    caudal_media = caudalimetro_1->caudal_promedio;
  } else if (caudalimetro_2->caudal_promedio) {
    caudal_media = caudalimetro_2->caudal_promedio;
  } else {
    caudal_media = 0;
  }
  bit_caudal_especifico = 1;
  Serial.print("caudalimetro_1->caudal_promedio: ");
  Serial.println(caudalimetro_1->caudal_promedio);
  Serial.print("caudalimetro_2->caudal_promedio: ");
  Serial.println(caudalimetro_2->caudal_promedio);
  Serial.print("caudal_media: ");
  Serial.println(caudal_media);
  Serial.print("delta_altura: ");
  Serial.println(delta_altura);
  Serial.print("caudal_especifico: ");
  Serial.println(caudal_especifico);
  Serial.print("nivel_alto->presion_media: ");
  Serial.println(nivel_alto->presion_media);
  Serial.print("nivel_bajo->presion_media: ");
  Serial.println(nivel_bajo->presion_media);
  Serial.print("manometro_1->presion_media: ");
  Serial.println(manometro_1->presion_media);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcula el caudal especifico
void CalcularQEspecifico() {
  if (nivel_alto->presion_media && nivel_bajo->presion_media && caudal_media) {

    delta_altura = nivel_alto->presion_media - nivel_bajo->presion_media;

    caudal_especifico = (caudal_media) / (delta_altura);  // calcula el caudal especifico
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Refresca el valr de la pantalla 1
void RefrescarPantalla_1() {
  if (bit_hora || bit_cambio) {
    MostrarValorPantalla(reloj_1->segundo, 0);  //hora
    bit_hora = 0;
  }
  if (bit_presion || bit_cambio) {
    MostrarValorPantalla(manometro_1->presion_media, 1);  //presion
    bit_presion = 0;
  }
  if (bit_caudal || bit_cambio) {
    MostrarValorPantalla(caudal_media, 2);  //caudal
    bit_caudal = 0;
  }
  if (bit_nivel_estatico || bit_cambio) {
    MostrarValorPantalla(nivel_alto->presion_media, 3);  //nivel estatico
    bit_nivel_estatico = 0;
  }
  if (bit_nivel_dinamico || bit_cambio) {
    MostrarValorPantalla(nivel_bajo->presion_media, 4);  //nivel dinamico
    bit_nivel_dinamico = 0;
  }
  if (bit_caudal_especifico || bit_cambio) {
    MostrarValorPantalla(caudal_especifico, 5);  //caudal especifico
    bit_caudal_especifico = 0;
  }
  bit_cambio = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Refresca el valr de la pantalla 2
void RefrescarPantalla_2() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Función para obtener las coordenadas del toque
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();  // Obtiene un punto de la pantalla táctil
  pinMode(YP, OUTPUT);        // Restaura los pines compartidos como salidas
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  // Configura los pines de control del TFT
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);  // Comprueba si la presión es válida
  if (pressed) {
    /*  pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); // Mapea las coordenadas táctiles a las de la pantalla
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());//*/

    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, tft.height());  // Mapea las coordenadas táctiles a las de la pantalla
    pixel_y = map(p.x, TS_TOP, TS_BOT, 0, tft.width());
  }
  return pressed;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Pantalla_1() {
  // Inicializa los botones de la matriz 5x2
  int btn_width = tft.width() / 2;
  int btn_height = tft.height() / 6;
  int tam = 1;  // para modificar el tamaño
  for (int i = 0; i < 12; i++) {
    int row = i / 2;
    int col = i % 2;
    int x = col * btn_width;
    int y = row * btn_height;
    if (i == 2 || i == 4) {
      tam = 3;
    } else {
      tam = 2;
    }
    btn_matrix[i].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 2, btn_height - 2, BLACK, CYAN, BLACK, matrix_labels[i], tam);
    btn_matrix[i].drawButton(false);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Pantalla_2() {
  // Inicializa los botones del teclado numérico
  int btn_width = tft.width() / 6;
  int btn_height = tft.height() / 4;
  for (int i = 0; i < 12; i++) {
    int row = i / 3;
    int col = i % 3;
    int x = col * btn_width;
    int y = row * btn_height;
    btn_digits[i].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 2, btn_height - 2, BLACK, CYAN, BLACK, labels[i], 2);
    btn_digits[i].drawButton(false);
  }

  // Inicializa los botones de la mitad derecha
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  for (int i = 0; i < 4; i++) {
    int x = tft.width() / 2 + right_btn_width / 2;
    int y = i * right_btn_height;
    right_btns[i].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, right_labels[i], 2);
    right_btns[i].drawButton(false);
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tactil_1() {
  bool down = Touch_getXY();  // Comprueba si la pantalla está siendo tocada

  btn_matrix[1].press(down && btn_matrix[1].contains(pixel_x, pixel_y));
  if (btn_matrix[1].justReleased())
    btn_matrix[1].drawButton();
  if (btn_matrix[1].justPressed()) {
    btn_matrix[1].drawButton(true);
    tft.fillScreen(WHITE);
    Pantalla_2();
    pant = 1;
    num_aux = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tactil_2() {
  bool down = Touch_getXY();  // Comprueba si la pantalla está siendo tocada

  // Procesa los botones del teclado numérico
  for (int i = 0; i < 12; i++) {
    btn_digits[i].press(down && btn_digits[i].contains(pixel_x, pixel_y));
    if (btn_digits[i].justReleased())
      btn_digits[i].drawButton();
    if (btn_digits[i].justPressed()) {
      btn_digits[i].drawButton(true);
      SeleccionNumerica(i);
    }
  }

  // Procesa los botones de la mitad derecha
  for (int i = 0; i < 2; i++) {
    right_btns[i].press(down && right_btns[i].contains(pixel_x, pixel_y));
    if (right_btns[i].justReleased())
      right_btns[i].drawButton();
    if (right_btns[i].justPressed()) {
      right_btns[i].drawButton(true);
      SeleccionParametro(i);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SeleccionNumerica(int i) {
  if (i == 9) {  // boton back
    if (num_aux) {
      num_aux = 0;
      MostrarValorTeclado(num_aux);
    } else {
      tft.fillScreen(WHITE);
      Pantalla_1();
      pant = 0;
      bit_cambio = 1;
    }
  } else if (i == 10) {  // boton "0"
    num_aux = num_aux * 10;
    MostrarValorTeclado(num_aux);
  } else if (i == 11) {  // boton ok
    INGRESA_PARAMETRO;
    MUESTRA_PARAMETRO;
  } else {
    num_aux = num_aux * 10;
    num_aux = num_aux + (i + 1);
    MostrarValorTeclado(num_aux);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SeleccionParametro(int i) {
  switch (i) {
    case 0:
      CAMBIA_MEDIDOR;
      break;
    case 1:
      CAMBIA_PARAMETRO;
      break;

    default:

      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorPantalla(int valor, int parametro) {
  int aux, aux_1, aux_2;
  dtostrf(valor, N_TOTAL, DECIMALES, buffer);
  switch (parametro) {
    case 0:
      aux = 0;    // columna
      aux_1 = 0;  // fila
      aux_2 = 0;  // elemento
      break;
    case 1:
      aux = 1;
      aux_1 = 1;
      aux_2 = 3;
      break;
    case 2:
      aux = 1;
      aux_1 = 2;
      aux_2 = 5;
      break;
    case 3:
      aux = 1;
      aux_1 = 3;
      aux_2 = 7;
      break;
    case 4:
      aux = 1;
      aux_1 = 4;
      aux_2 = 9;
      break;
    case 5:
      aux = 1;
      aux_1 = 5;
      aux_2 = 11;
      break;
    default:
      aux = 0;
      aux_1 = 0;
      aux_2 = 0;
      break;
  }
  int btn_width = tft.width() / 2;
  int btn_height = tft.height() / 6;
  int x = aux * btn_width;
  int y = aux_1 * btn_height;
  btn_matrix[aux_2].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 2, btn_height - 2, BLACK, CYAN, BLACK, buffer, 2);
  btn_matrix[aux_2].drawButton(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorMedidor(int valor) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorParametro(int valor) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorTeclado(int valor) {
  dtostrf(num_aux, N_TOTAL, DECIMALES, buffer);
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 3 * right_btn_height;
  right_btns[3].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, buffer, 2);
  right_btns[3].drawButton(false);
}
