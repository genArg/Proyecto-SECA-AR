#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include <TimerOne.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <Wire.h>
#include "define.h"
#include "caudal.h"
#include "tiempo.h"
#include "presion.h"
#include "tarjetaSD.h"

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

const char* matrix_mediciones[5] = { "Presion ", "Caudal ", "Niv Estatico ", "Niv Dinamico ", "Q Especifico " };
const char* matrix_unidades[5] = { "[bar] ", "[m3/h] ", "[m]", "[m] ", "[m3/(m.h)]" };


// Botones para la pantalla_2
// Botones para el teclado numérico
Adafruit_GFX_Button btn_digits[12];  // Botones 0-9, OK, y BACK
const char* labels[12] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "BK", "0", "OK" };

// Botones para la mitad derecha
Adafruit_GFX_Button right_btns[4];
const char* right_labels[4] = { "Gen", "Color", "Valor", "N_valor" };

// Variables globales para almacenar las coordenadas del toque
uint16_t pixel_x, pixel_y;
// para intercambiar entre pantalla
uint16_t pant;
//usado para el teclado numerico
uint16_t valor_teclado;
// Buffer para almacenar la cadena
char buffer[20];

//
//--------------------------------------------------------------------------------------------------------------------
//Para la modificacion de los parametros
const char* medidores[6] = { "Gen", "Nivel", "Presion", "Caudal", "Tiempo", "Memoria" };

const char* param_gen[1] = { "Color" };
const char* param_nivel[6] = { "Constante", "I min", "I MAX", "Ress", "Val min", "Val MAX" };
const char* param_presion[6] = { "Constante", "I min", "I MAX", "Ress", "Pre min", "Pre MAX" };
const char* param_caudal[3] = { "Constante", "In 1", "In 2" };
const char* param_tiempo[5] = { "Minuto", "Hora", "Dia", "Mes", "Year" };
const char* param_memoria[3] = { "Habilitado", "Codigo", "Intervalo" };

const char** parametros[6] = { param_gen, param_nivel, param_presion, param_caudal, param_tiempo, param_memoria };

// Tamaños
uint16_t Zise[6] = { 1, 6, 6, 3, 5, 3 };
uint16_t indice_medidor = 0;
uint16_t indice_parametro = 0;

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
uint8_t bit_pantalla_actualizar = 0;
uint16_t contador_1s = 1;
uint16_t contador_10s = 1;
uint16_t contador_60s = 1;
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
tarjeta_t tarjeta_1;
// bit para habilitar el guardado de datos
uint16_t bit_SD = 0;
// contador para guradar datos en sd
uint32_t contador_SD = 1;

//--------------------------------------------------------------------------------------------------------------------
// variables RTC
DS3231 myRTC;
uint8_t sec_up_RTC = 0;
//*
// Variables for use in method parameter lists for interrupt
byte alarmDay;
byte alarmHour;
byte alarmMinute;
byte alarmSecond;
byte alarmBits;
bool alarmDayIsDay;
bool alarmH12;
bool alarmPM;
// bit de signal para reiniciar la alarma
volatile byte tick = 1;
// myRTC interrupt pin
#define CLINT 19
//*/
//--------------------------------------------------------------------------------------------------------------------
uint8_t color_pantalla = 0;  // cambia la pantalla entre ocuro y claro
uint16_t valor_aux;          // varibale de pruebas
byte state = false;          // para la prueba de la interrupcion del rtc
int contador_rtc = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

#if DEBUG == TRUE
  Serial.begin(9600);
  Serial.println("OK");
#endif

  //--------------------------------------------------------------------------------------------------------------------
  // timer
  Timer1.initialize(TIME_BASE);        // Inicializa el temporizador a 1 segundo (100000 µs)
  Timer1.attachInterrupt(FnCallback);  // Adjunta la función de callback

  //--------------------------------------------------------------------------------------------------------------------
  // Configurar el del compresor led como salida
  pinMode(PIN_OUT_COMPRESOR, OUTPUT);
  // Configurar el pin PIN_READY como entrada
  pinMode(PIN_READY, INPUT);
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
  //attachInterrupt(digitalPinToInterrupt(INTERRUPCION_PULSE), ISR_Pin, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPCION_C1), ISR_Pin, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPCION_C2), ISR_Pin, CHANGE);
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
  // objetos del codigo
  nivel_alto = InicializarManometro();
  nivel_bajo = InicializarManometro();
  manometro_1 = InicializarManometro();
  caudalimetro_1 = InicializarCaudal();
  caudalimetro_2 = InicializarCaudal();
  reloj_1 = InicializarReloj();
  tarjeta_1 = InicializarTarjeta();

  //--------------------------------------------------------------------------------------------------------------------
  //apaga el compresor
  digitalWrite(PIN_OUT_COMPRESOR, LOW);  //apaga el compresor


  //--------------------------------------------------------------------------------------------------------------------
  //RTC
  // Start the I2C interface
  Wire.begin();
  bool mode12 = false;         // use 24-hour clock mode
  myRTC.setClockMode(mode12);  // uploads 'true' (1) to bit 6 of register 0x02
  CopiarDate();                // carga la fecha del RTC a reloj_1
  IntRTC();                    // Configura la interrupcion desde el rtc

  //--------------------------------------------------------------------------------------------------------------------
  //inicia conexion con el sd
  if (tarjeta_1->activo) {
#if SD_ACTIVE == TRUE
    if (AbrirSD(tarjeta_1)) {
      tarjeta_1->activo = 1;
    } else {
      tarjeta_1->activo = 0;
    }
#endif
  }
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

#if DEBUG == TRUE
        Serial.println("READY_ON");
        bit_SD = 1;
#endif
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

#if DEBUG == TRUE
        Serial.println("READY_OFF");
        bit_SD = 1;
#endif
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
#if SD_ACTIVE == TRUE
  if (bit_SD) {
    if (tarjeta_1->activo) {
      bit_SD = 0;
      contador_SD = 1;
      AbrirSD(tarjeta_1);
      GuardarDatos();
    }
  }
#endif
  //--------------------------------------------------------------------------------------------------------------------
  // Pantalla
  if (pant) {
    Tactil_2();
    RefrescarPantalla_2();  // muestra los valores en pantalla Configuracion
  } else {
    if (bit_pantalla_actualizar) {
      RefrescarPantalla_1();  // muestra los valores en pantalla Home
      bit_pantalla_actualizar = 0;
    }

    Tactil_1();
  }

  //--------------------------------------------------------------------------------------------------------------------
  FnsIrsAuxiliares();

  //--------------------------------------------------------------------------------------------------------------------
  digitalWrite(PIN_HAPPY_LED, happy_led_estado);
  //--------------------------------------------------------------------------------------------------------------------
  //*
  // reinicia la alarma del rtc
  if (tick) {
    LimpiarAlarma();
  }
  //*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// funcion llama de interrupcion del temporizador
void FnCallback() {
  contador_rtc++;

  //--------------------------------------------------------------------------------------------------------------------
  if (sec_up_RTC) {
    // aumenta en 1 segundo el reloj interno
    UnSegundoReloj(reloj_1);
    // bandera indica que se modifico la hora
    bit_hora = 1;
    // reinicia la bandera de la interrupcion de 1 segundo
    sec_up_RTC = 0;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // comprobacion cada 200 ms
  if (0 == (contador_200ms = (contador_200ms + 1) % (2 * TIME_100MS))) {
    // bit para actualizar la pantalla
    bit_pantalla_actualizar = 1;

    // comprueba si cambio el estado de los pulsadores
    ISR_Pin();
  }

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_1s = (contador_1s + 1) % TIME_1S)) {
    happy_led_estado = (happy_led_estado) ? 0 : 1;
    bit_1m = 1;
    bit_caudal_especifico = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_10s = (contador_10s + 1) % TIME_10S)) {
    bit_hora = 1;
  }

  //--------------------------------------------------------------------------------------------------------------------
  if (0 == (contador_60s = (contador_60s + 1) % TIME_60S)) {
  }

  //--------------------------------------------------------------------------------------------------------------------
  contador_happy_led = contador_1s;

  //--------------------------------------------------------------------------------------------------------------------
  // timer del compresor
  if (bit_compresor == 0) {
    contador_compresor = (contador_compresor + 1) % (6 * TIME_10S);  //TIME_VALOR_COMPRESOR debe ser 1 minuto;
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

  //--------------------------------------------------------------------------------------------------------------------
  if (tarjeta_1->activo) {
    contador_SD = (contador_SD + 1) % (tarjeta_1->tiempo * TIME_60S);
    if (contador_SD == 0) {
      bit_SD = 1;
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuardarDatos() {
  // Deshabilita todas las interrupciones
  noInterrupts();
  File myFile;
  myFile = SD.open(tarjeta_1->nombre_file, FILE_WRITE);
  if (myFile) {
    myFile.print(String(reloj_1->dia) + "/" + String(reloj_1->mes) + "/" + String(reloj_1->year) + ", ");         // fecha
    myFile.print(String(reloj_1->hora) + "/" + String(reloj_1->minuto) + "/" + String(reloj_1->segundo) + ", ");  // hora                                                                        // hora
    myFile.print(String(manometro_1->presion_media) + ", ");                                                      // presion
    myFile.print(String(caudal_media) + ", ");                                                                    // caudal
    myFile.print(String(nivel_alto->presion_media) + ", ");                                                       // nivel estatico
    myFile.print(String(nivel_bajo->presion_media) + ", ");                                                       // nivel Dinamico
    myFile.println(String(caudal_especifico) + ", ");                                                             // Caudal especifico
    myFile.close();                                                                                               // Cerramos el archivo
#if DEBUG == TRUE
    Serial.println("TERMINO ESCRITURA");
#endif
  } else {
#if DEBUG == TRUE
    Serial.println("Error al abrir el archivo para escritura");
#endif
  }
  // Habilita todas las interrupciones
  interrupts();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FnsIrsAuxiliares() {
  if ((a_pulse == 0) && (b_pulse == 1)) {
    b_pulse = 0;

#if DEBUG == TRUE
    Serial.println("interrupcion 19");
#endif
  }

  //--------------------------------------------------------------------------------------------------------------------
  if ((a_c1 == 0) && (b_c1 == 1)) {
    b_c1 = 0;
#if DEBUG == TRUE
    Serial.println("interrupcion 20");
#endif
    CalculoCaudal(caudalimetro_1);
    PromediarCaudal();
    if (pin_estado_c1) {
      bit_caudal = 1;
    }
  }

  //--------------------------------------------------------------------------------------------------------------------
  if ((a_c2 == 0) && (b_c2 == 1)) {
    b_c2 = 0;
#if DEBUG == TRUE
    Serial.println("interrupcion 21");
#endif
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
#if DEBUG == TRUE
    Serial.println("interrupcion 19a");
#endif
    if (a_pulse && pin_estado_pulse) {
#if DEBUG == TRUE
      Serial.println("interrupcion 19b");
#endif
      a_pulse = 0;
      b_pulse = 1;
      contador_int_pulse = 1;
      bit_compresor = 1;
    }
  }

  if (pin_estado_c1 != (pin_auxiliar = digitalRead(INTERRUPCION_C1))) {
    pin_estado_c1 = pin_auxiliar;
#if DEBUG == TRUE
    Serial.println("interrupcion 20a");
#endif
    if (a_c1 && pin_estado_c1) {
#if DEBUG == TRUE
      Serial.println("interrupcion 20b");
#endif
      a_c1 = 0;
      b_c1 = 1;
      contador_int_c1 = 1;
      //CaudalGuardarTiempo(caudalimetro_1, TomarTiempo(reloj_1));
      TomarTiempo(caudalimetro_1);
    }
  }

  if (pin_estado_c2 != (pin_auxiliar = digitalRead(INTERRUPCION_C2))) {
    pin_estado_c2 = pin_auxiliar;
#if DEBUG == TRUE
    Serial.println("interrupcion 21a");
#endif
    if (a_c2 && pin_estado_c2) {
#if DEBUG == TRUE
      Serial.println("interrupcion 21b");
#endif
      a_c2 = 0;
      b_c2 = 1;
      contador_int_c2 = 1;
      //CaudalGuardarTiempo(caudalimetro_2, TomarTiempo(reloj_1));
      TomarTiempo(caudalimetro_2);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcula promedio
void PromediarCaudal() {
  if ((caudalimetro_1->caudal_promedio && caudalimetro_1->habilitacion) && (caudalimetro_2->caudal_promedio && caudalimetro_2->habilitacion)) {
    caudal_media = (caudalimetro_1->caudal_promedio + caudalimetro_2->caudal_promedio) / 2;
  } else if ((caudalimetro_1->caudal_promedio && caudalimetro_1->habilitacion)) {
    caudal_media = caudalimetro_1->caudal_promedio;
  } else if ((caudalimetro_2->caudal_promedio && caudalimetro_2->habilitacion)) {
    caudal_media = caudalimetro_2->caudal_promedio;
  } else {
    caudal_media = 0;
  }
  bit_caudal_especifico = 1;

#if DEBUG == TRUE
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
  Serial.print("caudalimetro_1->tiempo 1: ");
  Serial.println(caudalimetro_1->delta_tiempo);
  Serial.print("caudalimetro_2->tiempo 2: ");
  Serial.println(caudalimetro_2->delta_tiempo);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcula el caudal especifico
void CalcularQEspecifico() {
  float auxiliar;
  if (nivel_alto->presion_media && nivel_bajo->presion_media && caudal_media) {

    delta_altura = nivel_alto->presion_media - nivel_bajo->presion_media;

    auxiliar = (caudal_media) / (delta_altura);  // calcula el caudal especifico
    if (auxiliar > 0) {
      caudal_especifico = auxiliar;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Refresca el valr de la pantalla 1 Home
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
  int tam = 2;  // para modificar el tamaño

  tft.fillScreen(CYAN);
  btn_matrix[1].initButton(&tft, btn_width + btn_width / 2, 0 + btn_height / 2, btn_width - 2, btn_height - 2, WHITE, BLACK, WHITE, matrix_labels[1], tam);
  btn_matrix[1].drawButton(false);
  tft.setTextColor(BLACK);
  for (int i = 0; i < 5; i++) {
    tft.setCursor(OFFSET, OFFSET + (1 + i) * btn_height);
    tft.setTextSize(2);
    tft.print(matrix_mediciones[i]);
    tft.setCursor(OFFSET + 1.1 * btn_width, OFFSET + (1 + i) * btn_height);
    tft.setTextSize(2);
    tft.print(String(0));
    tft.setCursor(OFFSET + 1.57 * btn_width, OFFSET + (1 + i) * btn_height);
    tft.setTextSize(1);
    tft.print(matrix_unidades[i]);
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
    btn_digits[i].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 2, btn_height - 2, WHITE, CYAN, BLACK, labels[i], 2);
    btn_digits[i].drawButton(false);
  }

  // Inicializa los botones de la mitad derecha
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  for (int i = 0; i < 4; i++) {
    int x = tft.width() / 2 + right_btn_width / 2;
    int y = i * right_btn_height;
    right_btns[i].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, WHITE, CYAN, BLACK, right_labels[i], 2);
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
    tft.fillScreen(BLACK);
    indice_medidor = 0;
    indice_parametro = 0;
    pant = 1;
    valor_teclado = 0;
    Pantalla_2();
    MostrarMedidor();
    MostrarParametro();
    MostrarValor();
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
// establece el numero de teclado presionado
void SeleccionNumerica(int i) {
  if (i == 9) {  // boton back
    if (valor_teclado) {
      valor_teclado = 0;
      MostrarValorTeclado(valor_teclado);
    } else {
      bit_cambio = 1;
      tft.fillScreen(WHITE);
      Pantalla_1();
      pant = 0;
      indice_medidor = 0;
      indice_parametro = 0;
    }
  } else if (i == 10) {  // boton "0"
    valor_teclado = valor_teclado * 10;
    MostrarValorTeclado(valor_teclado);
  } else if (i == 11) {  // boton ok
    GuardarParametro(valor_teclado);
    MostrarMedidor();
    MostrarParametro();
    MostrarValor();
    valor_teclado = 0;
    MostrarValorTeclado(valor_teclado);
  } else {
    valor_teclado = valor_teclado * 10;
    valor_teclado = valor_teclado + (i + 1);
    MostrarValorTeclado(valor_teclado);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// discrimina entre los dos botones presionados
void SeleccionParametro(int i) {
  switch (i) {
    case 0:
      indice_medidor = (indice_medidor + 1) % (sizeof(Zise) / sizeof(Zise[0]));  //permite realizar un bucle entre los elementos
      valor_teclado = 0;
      indice_parametro = 0;
      MostrarMedidor();
      MostrarParametro();
      MostrarValor();
      MostrarValorTeclado(valor_teclado);
#if DEBUG == TRUE
      Serial.println("presiono medidor");
#endif
      break;
    case 1:
      indice_parametro = (indice_parametro + 1) % Zise[indice_medidor];  //permite realizar un bucle entre los parametros
      valor_teclado = 0;
      MostrarParametro();
      MostrarValor();
      MostrarValorTeclado(valor_teclado);
#if DEBUG == TRUE
      Serial.println("presiono cambiar parametro");
#endif
      break;

    default:

      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// muestra los valores en la primera pantalla
void MostrarValorPantalla(float valor, int parametro) {
  //*
  int btn_width = tft.width() / 2;
  int btn_height = tft.height() / 6;
  tft.setTextColor(BLACK);
  if (parametro == 0) {
    tft.fillRect(OFFSET, OFFSET, 100, 33, CYAN);
    tft.setTextSize(2);
    tft.setCursor(OFFSET, OFFSET);
    tft.println(String(reloj_1->dia) + "/" + String(reloj_1->mes) + "/" + String(reloj_1->year));
    tft.setCursor(OFFSET, 17 + OFFSET);
    tft.println(String(reloj_1->hora) + ":" + String(reloj_1->minuto) + ":" + String(reloj_1->segundo));
  } else {
    tft.fillRect(OFFSET + 1 * btn_width, OFFSET + (parametro)*btn_height, 70, 33, CYAN);
    tft.setCursor(OFFSET + 1 * btn_width, OFFSET + (parametro)*btn_height);
    tft.setTextSize(2);
    tft.print(String(valor));
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// muestra los medidores en la segunda pantalla
void MostrarMedidor() {

  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 0 * right_btn_height;
  right_btns[0].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, medidores[indice_medidor], 2);
  right_btns[0].drawButton(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Muestra el parametro activo
void MostrarParametro() {

  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 1 * right_btn_height;
  right_btns[1].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, parametros[indice_medidor][indice_parametro], 2);
  right_btns[1].drawButton(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// muestra el valor del parametro en la pantalla
void MostrarValor() {
  float valor = 0;
  valor = ObtenerParametro();
  dtostrf(valor, N_TOTAL, DECIMALES, buffer);

  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 2 * right_btn_height;
  right_btns[2].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, buffer, 2);
  right_btns[2].drawButton(false);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// muestra el valor del teclado numerico en la pantalla
void MostrarValorTeclado(uint16_t valor) {
  dtostrf(valor, N_TOTAL, DECIMALES, buffer);
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 4;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 3 * right_btn_height;
  right_btns[3].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 2, right_btn_height - 2, BLACK, CYAN, BLACK, buffer, 2);
  right_btns[3].drawButton(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// obtiene el valor de los parametros
float ObtenerParametro() {

  float auxiliar = 0;

  switch (indice_medidor) {
    case 0:  // General
      auxiliar = color_pantalla;
      break;
    case 1:  // nivel
      switch (indice_parametro) {
        case 0:  // constante
          auxiliar = nivel_alto->constante;
          break;
        case 1:  // corriente minima
          auxiliar = nivel_alto->corriente_min;
          break;
        case 2:  // corriente maxima
          auxiliar = nivel_alto->corriente_max;
          break;
        case 3:  // resistencia
          auxiliar = nivel_alto->resistencia;
          break;
        case 4:  // valor Minimo
          auxiliar = nivel_alto->presion_min;
          break;
        case 5:  // valor maximo
          auxiliar = nivel_alto->presion_max;
          break;
        default:

          break;
      }
      break;
    case 2:  // presion
      switch (indice_parametro) {
        case 0:  // constante
          auxiliar = manometro_1->constante;
          break;
        case 1:  // corriente minima
          auxiliar = manometro_1->corriente_min;
          break;
        case 2:  // corriente maxima
          auxiliar = manometro_1->corriente_max;
          break;
        case 3:  // resistencia
          auxiliar = manometro_1->resistencia;
          break;
        case 4:  // presion minima
          auxiliar = manometro_1->presion_min;
          break;
        case 5:  // presion maxima
          auxiliar = manometro_1->presion_max;
          break;
        default:

          break;
      }
      break;
    case 3:  // caudal
      switch (indice_parametro) {
        case 0:  // constante
          auxiliar = caudalimetro_1->constante;
          break;
        case 1:  // entrada 1
          auxiliar = caudalimetro_1->habilitacion;
          break;
        case 2:  // entrada 2
          auxiliar = caudalimetro_2->habilitacion;
          break;
        default:

          break;
      }
      break;
    case 4:  // tiempo
      switch (indice_parametro) {
        case 0:  // minuto
          auxiliar = reloj_1->minuto;
          break;
        case 1:  // hora
          auxiliar = reloj_1->hora;
          break;
        case 2:  // dia
          auxiliar = reloj_1->dia;
          break;
        case 3:  // mes
          auxiliar = reloj_1->mes;
          break;
        case 4:  // año
          auxiliar = reloj_1->year;
          break;
        default:

          break;
      }
      break;
    case 5:  //memoria
      switch (indice_parametro) {
        case 0:  // habilitar
          auxiliar = tarjeta_1->activo;
          break;
        case 1:  // codigo
          auxiliar = tarjeta_1->codigo;
          break;
        case 2:  // tiempo
          auxiliar = tarjeta_1->tiempo;
          break;
        default:

          break;
      }
      break;
    default:

      break;
  }
#if DEBUG == TRUE
  Serial.print("indice_medidor--------------> ");
  Serial.println(indice_medidor);
  Serial.print("indice_parametro--------------> ");
  Serial.println(indice_parametro);
#endif
  return auxiliar;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarda el valor de los parametros
void GuardarParametro(uint16_t valor) {
  uint16_t auxiliar;

  switch (indice_medidor) {
    case 0:  // General
      color_pantalla = valor;
      tft.invertDisplay(color_pantalla);
      break;
    case 1:  // nivel
      switch (indice_parametro) {
        case 0:  // constante
          nivel_alto->constante = valor;
          nivel_bajo->constante = valor;
          break;
        case 1:  // corriente minima
          nivel_alto->corriente_min = valor;
          nivel_bajo->corriente_min = valor;
          break;
        case 2:  // corriente maxima
          nivel_alto->corriente_max = valor;
          nivel_bajo->corriente_max = valor;
          break;
        case 3:  // resistencia
          nivel_alto->resistencia = valor;
          nivel_bajo->resistencia = valor;
          break;
        case 4:  // valor Minimo
          nivel_alto->presion_min = valor;
          nivel_bajo->presion_min = valor;
          break;
        case 5:  // valor maximo
          nivel_alto->presion_max = valor;
          nivel_bajo->presion_max = valor;
          break;
        default:

          break;
      }
      Recalcular(nivel_alto);
      Recalcular(nivel_bajo);
      break;
    case 2:  // presion
      switch (indice_parametro) {
        case 0:  // constante
          manometro_1->constante = valor;
          break;
        case 1:  // corriente minima
          manometro_1->corriente_min = valor;
          break;
        case 2:  // corriente maxima
          manometro_1->corriente_max = valor;
          break;
        case 3:  // resistencia
          manometro_1->resistencia = valor;
          break;
        case 4:  // presion minima
          manometro_1->presion_min = valor;
          break;
        case 5:  // presion maxima
          manometro_1->presion_max = valor;
          break;
        default:

          break;
      }
      Recalcular(manometro_1);
      break;
    case 3:  // caudal
      switch (indice_parametro) {
        case 0:  // constante
          auxiliar = caudalimetro_1->constante;
          caudalimetro_1->constante = valor;
          caudalimetro_2->constante = valor;
          RecalcularCaudal(caudalimetro_1, auxiliar);
          RecalcularCaudal(caudalimetro_2, auxiliar);
          break;
        case 1:  // entrada 1
          caudalimetro_1->habilitacion = valor;
          break;
        case 2:  // entrada 2
          caudalimetro_2->habilitacion = valor;
          break;
        default:

          break;
      }
      break;
    case 4:  // tiempo
      switch (indice_parametro) {
        case 0:  // minuto
          myRTC.setSecond(0);
          reloj_1->segundo = 0;
          myRTC.setMinute(valor);
          reloj_1->minuto = valor;
          break;
        case 1:  // hora
          myRTC.setHour(valor);
          reloj_1->hora = valor;
          break;
        case 2:  // dia
          myRTC.setDate(valor);
          reloj_1->dia = valor;
          break;
        case 3:  // mes
          myRTC.setMonth(valor);
          reloj_1->mes = valor;
          break;
        case 4:  // año
          if (valor > 2000) {
            myRTC.setYear(valor - 2000);
            reloj_1->year = valor;
          }
          break;
        default:

          break;
      }
      break;
    case 5:  //memoria
      switch (indice_parametro) {
        case 0:  // habilitar
          if (valor) {
            if (AbrirSD(tarjeta_1)) {
              tarjeta_1->activo = 1;
            } else {
              tarjeta_1->activo = 0;
            }
          } else {
            tarjeta_1->activo = 0;
          }
          break;
        case 1:  // codigo
          tarjeta_1->codigo = valor;
          break;
        case 2:  // tiempo
          tarjeta_1->tiempo = valor;
          break;
        default:

          break;
      }
      break;
    default:

      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trasferir hora desde RTC
void CopiarDate() {
  // segundo
  reloj_1->segundo = myRTC.getSecond();
  // minuto
  reloj_1->minuto = myRTC.getMinute();
  // hora
  bool h12 = false;  // formato de 24 horas
  bool hPM;
  reloj_1->hora = myRTC.getHour(h12, hPM);
  // dia
  reloj_1->dia = myRTC.getDate();
  // mes
  bool CenturyBit;
  reloj_1->mes = myRTC.getMonth(CenturyBit);
  // año
  reloj_1->year = 2000 + myRTC.getYear();
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*
// Preparacion de la interrupcion para RTC
void IntRTC() {
  // Assign parameter values for Alarm 1
  alarmDay = 0;
  alarmHour = 0;
  alarmMinute = 0;
  alarmSecond = 0;
  alarmBits = 0b00001111;  // Alarm 1 every second
  alarmDayIsDay = false;
  alarmH12 = false;
  alarmPM = false;

  // Set alarm 1 to fire at one-second intervals
  myRTC.turnOffAlarm(1);
  myRTC.setA1Time(
    alarmDay, alarmHour, alarmMinute, alarmSecond,
    alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  // enable Alarm 1 interrupts
  myRTC.turnOnAlarm(1);
  // clear Alarm 1 flag
  myRTC.checkIfAlarm(1);
  // disable Alarm 2 interrupt
  myRTC.turnOffAlarm(2);
  // clear Alarm 2 flag
  myRTC.checkIfAlarm(2);

  // NOTE: both of the alarm flags must be clear
  // to enable output of a FALLING interrupt

  // attach clock interrupt
  pinMode(CLINT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLINT), isr_TickTock, FALLING);
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion interrupcion para RTC
void isr_TickTock() {
  // interrupt signals to loop
  tick = 1;
  // flag para incrementar 1 segundo en el reloj del arduino
  sec_up_RTC = 1;
#if DEBUG_RTC == TRUE
  state = ~state;
  Serial.println((state ? "ON" : "OFF"));
#endif
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion interrupcion para RTC
void LimpiarAlarma() {
  tick = 0;
  // Clear Alarm 1 flag
  myRTC.checkIfAlarm(1);
#if DEBUG_RTC == TRUE
  Serial.println(contador_rtc);
#endif
  contador_rtc = 0;
}
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para guardar el tiempo
void TomarTiempo(caudalimetro_t caudalimetro) {
  caudalimetro->milisegundo_ant = caudalimetro->milisegundo;  // son decenas de milisegundos
  caudalimetro->segundo_ant = caudalimetro->segundo;
  caudalimetro->minuto_ant = caudalimetro->minuto;
  caudalimetro->hora_ant = caudalimetro->hora;
  caudalimetro->dia_ant = caudalimetro->dia;
  caudalimetro->mes_ant = caudalimetro->mes;
  caudalimetro->year_ant = caudalimetro->year;

  caudalimetro->milisegundo = contador_rtc;  // son decenas de milisegundos
  caudalimetro->segundo = reloj_1->segundo;
  caudalimetro->minuto = reloj_1->minuto;
  caudalimetro->hora = reloj_1->hora;
  caudalimetro->dia = reloj_1->dia;
  caudalimetro->mes = reloj_1->mes;
  caudalimetro->year = reloj_1->year;
}
