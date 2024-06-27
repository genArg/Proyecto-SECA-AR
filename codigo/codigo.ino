#include "define.h"
#include "caudal.h"
#include "grafica.h"
#include "tiempo.h"
#include "presion.h"

// Macros

caudalimetro_t caudalimetro;  // crea un caudalimetro
char mensaje[] = "mensaje";
char letra;
static manometro_t manometro_1;
static caudalimetro_t caudalimetro_1;
uint16_t valor_aux;

void setup() {
  // put your setup code here, to run once:
  //pinMode(pin_entrada, INPUT);
  //IniciarValores(caudalimetro);
  Serial.begin(9600);
  valor_aux = 12349;
  manometro_1 = InicializarManometro();
  ParametrizarCorrienteMin(manometro_1, 70);
  caudalimetro_1 = InicializarCaudal();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Verificar(caudalimetro);
  // Leer el valor del pin analógico A0
  //int valorAnalogico = analogRead(A15);

  // Imprimir el valor leído en el monitor serie
  

  //valor_aux = PruevaFuncion();
  

  //Serial.println(valor_aux);

  // Esperar 500 milisegundos antes de la próxima lectura
  Serial.print("Valor manometro: ");
  valor_aux = ValoresManometro(manometro_1, 4);
  Serial.println(valor_aux);
  delay(1000);
  Serial.print("Valor caudalimetro: ");
  valor_aux = ValoresCaudal(caudalimetro_1, 2);
  Serial.println(valor_aux);
  delay(1000);
}
