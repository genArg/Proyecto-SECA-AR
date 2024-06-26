#include "define.h"
#include "caudal.h"
#include "grafica.h"
#include "tiempo.h"
#include "presion.h"

// Macros

caudalimetro_t caudalimetro;  // crea un caudalimetro
char mensaje[] = "mensaje";
char letra;
caudalimetro_t caudalimetro_1;
manometro_t manometro_1;
uint16_t valor_aux;

void setup() {
  // put your setup code here, to run once:
  //pinMode(pin_entrada, INPUT);
  //IniciarValores(caudalimetro);
  Serial.begin(9600);
  valor_aux = 12349;
}

void loop() {
  // put your main code here, to run repeatedly:
  //Verificar(caudalimetro);
  // Leer el valor del pin analógico A0
  //int valorAnalogico = analogRead(A15);

  // Imprimir el valor leído en el monitor serie
  Serial.print("Valor analógico leído: ");

  valor_aux = PruevaFuncion();
  //valor_aux = ValoresManometro(manometro_1, 1);

  Serial.println(valor_aux);

  // Esperar 500 milisegundos antes de la próxima lectura
  delay(1000);
}
