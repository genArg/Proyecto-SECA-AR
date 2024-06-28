#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#include <TimerOne.h>
#include "define.h"
#include "caudal.h"
#include "tiempo.h"
#include "presion.h"

// Pantalla

// Calibración de la pantalla táctil para landscape
const int XP = 8, XM = A2, YP = A3, YM = 9;  // Pines del touch panel
//portrait
//const int TS_LEFT = 67, TS_RT = 897, TS_TOP = 924, TS_BOT = 103;
//landscape
const int TS_LEFT = 70, TS_RT = 893, TS_TOP = 918, TS_BOT = 10;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Botones para la matriz de 5x2 de la pantalla_1
Adafruit_GFX_Button btn_matrix[10];  // Botones 5 filas x 2 columnas
const char* matrix_labels[10] = { "Hora", "Config", "Presion", "x", "Caudal", "x", "Nivel Estatico", "x", "Nivel Dinamico", "x" };


// Botones para la pantalla_2
// Botones para el teclado numérico
Adafruit_GFX_Button btn_digits[12];  // Botones 0-9, OK, y BACK
const char* labels[12] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "BK", "0", "OK" };

// Botones para la mitad derecha
Adafruit_GFX_Button right_btns[3];
const char* right_labels[3] = { "Caudalimetro", "Parametro", "Valor" };

int pixel_x, pixel_y;  // Variables globales para almacenar las coordenadas del toque
int pant;              // para intercambiar entre pantalla
int num_aux;           //usado para el teclado numerico
char buffer[20];       // Buffer para almacenar la cadena

//

caudalimetro_t caudalimetro;  // crea un caudalimetro
char mensaje[] = "mensaje";
char letra;
static manometro_t manometro_1;
static caudalimetro_t caudalimetro_1;
static reloj_t reloj_1;
int happy = 0; //para el led feliz
uint16_t valor_aux;  // varibale de pruebas

void setup() {
  // pantalla
  uint16_t ID = tft.readID();     // Lee el ID de la pantalla TFT
  if (ID == 0xD3D3) ID = 0x9486;  // Ajuste para pantallas de solo escritura
  tft.begin(ID);                  // Inicializa la pantalla TFT
  tft.setRotation(1);             // Configura la orientación de la pantalla (1 = landscape)
  tft.fillScreen(WHITE);          // Rellena la pantalla de blanco
  pant = 0;
  Pantalla_1();


  Serial.begin(9600);
  //Serial.println("OK");

  manometro_1 = InicializarManometro();
  caudalimetro_1 = InicializarCaudal();
  reloj_1 = InicializarReloj();


  // configuracion de los pines digitales
  pinMode(PIN_READY, INPUT_PULLUP);
  pinMode(PIN_FORZAR_MEDICION, INPUT_PULLUP);
  pinMode(PIN_OUT_COMPRESOR, OUTPUT);
  pinMode(PIN_HAPPY, OUTPUT);

  //analogRead(pin_analogico_0);
  Timer1.initialize(1000000);          // Inicializa el temporizador a 1 segundo (1000000 µs)
  Timer1.attachInterrupt(FnCallback);  // Adjunta la función de callback
}

void loop() {

  if(digitalRead(PIN_READY)){
    digitalWrite(PIN_OUT_COMPRESOR, HIGH);
  }

// Pantalla
  if (pant) {
    Tactil_2();
  } else {
    Tactil_1();
  }

  digitalWrite(PIN_HAPPY, happy);
}

// funcion llama de interrupcion del temporizador
void FnCallback() {
  // para el led happy
  happy = (happy) ? 0 : 1;
  // aumenta en 1 segundo el reloj interno
  UnSegundoReloj(reloj_1);
  
}









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

void Pantalla_1() {
  // Inicializa los botones de la matriz 5x2
  int btn_width = tft.width() / 2;
  int btn_height = tft.height() / 5;
  int tam = 1;  // para modificar el tamaño
  for (int i = 0; i < 10; i++) {
    int row = i / 2;
    int col = i % 2;
    int x = col * btn_width;
    int y = row * btn_height;
    if (i == 2 || i == 4) {
      tam = 3;
    } else {
      tam = 2;
    }
    btn_matrix[i].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 10, btn_height - 10, CYAN, CYAN, BLACK, matrix_labels[i], tam);
    btn_matrix[i].drawButton(false);
  }
}

void Pantalla_2() {
  // Inicializa los botones del teclado numérico
  int btn_width = tft.width() / 6;
  int btn_height = tft.height() / 4;
  for (int i = 0; i < 12; i++) {
    int row = i / 3;
    int col = i % 3;
    int x = col * btn_width;
    int y = row * btn_height;
    btn_digits[i].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 10, btn_height - 10, CYAN, CYAN, BLACK, labels[i], 2);
    btn_digits[i].drawButton(false);
  }

  // Inicializa los botones de la mitad derecha
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 3;
  for (int i = 0; i < 3; i++) {
    int x = tft.width() / 2 + right_btn_width / 2;
    int y = i * right_btn_height;
    right_btns[i].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 10, right_btn_height - 10, CYAN, CYAN, BLACK, right_labels[i], 2);
    right_btns[i].drawButton(false);
  }
}

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
  for (int i = 0; i < 3; i++) {
    right_btns[i].press(down && right_btns[i].contains(pixel_x, pixel_y));
    if (right_btns[i].justReleased())
      right_btns[i].drawButton();
    if (right_btns[i].justPressed()) {
      right_btns[i].drawButton(true);
      SeleccionParametro(i);
    }
  }
}


void SeleccionNumerica(int i) {
  if (i == 9) {
    if (num_aux) {
      num_aux = 0;
      MostrarValorTeclado(num_aux);
    } else {
      tft.fillScreen(WHITE);
      Pantalla_1();
      pant = 0;
    }
  } else if (i == 10) {
    num_aux = num_aux * 10;
    MostrarValorTeclado(num_aux);
  } else if (i == 11) {
    INGRESA_PARAMETRO;
    MUESTRA_PARAMETRO;
  } else {
    num_aux = num_aux * 10;
    num_aux = num_aux + (i + 1);
    MostrarValorTeclado(num_aux);
  }
}

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
////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorPantalla(int valor, int parametro) {
  int aux, aux_1, aux_2;
  dtostrf(valor, N_TOTAL, DECIMALES, buffer);
  switch (parametro) {
    case 0:
      aux = 0;
      aux_1 = 0;
      aux_2 = 0;
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
    default:
      aux = 0;
      aux_1 = 0;
      aux_2 = 0;
      break;
  }
  int btn_width = tft.width() / 2;
  int btn_height = tft.height() / 5;
  int x = aux * btn_width;
  int y = aux_1 * btn_height;
  btn_matrix[aux_2].initButton(&tft, x + btn_width / 2, y + btn_height / 2, btn_width - 10, btn_height - 10, CYAN, CYAN, BLACK, buffer, 2);
  btn_matrix[aux_2].drawButton(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
void MostrarValorMedidor(int valor) {}

void MostrarValorParametro(int valor) {}

void MostrarValorTeclado(int valor) {
  dtostrf(num_aux, N_TOTAL, DECIMALES, buffer);
  int right_btn_width = tft.width() / 2;
  int right_btn_height = tft.height() / 3;
  int x = tft.width() / 2 + right_btn_width / 2;
  int y = 2 * right_btn_height;
  right_btns[2].initButton(&tft, x, y + right_btn_height / 2, right_btn_width - 10, right_btn_height - 10, CYAN, CYAN, BLACK, buffer, 2);
  right_btns[2].drawButton(false);
}
