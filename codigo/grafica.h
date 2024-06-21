//! Macros
#ifndef GRAFICA_H
#define GRAFICA_H

#include "define.h"
#include <UTFTGLUE.h>

extern uint8_t BigFont[];
PANTALLA

void InicioPantalla() {

  myGLCD.InitLCD();
  myGLCD.clrScr();

  // Usar la fuente BigFont
  myGLCD.setFont(BigFont);

  // Establecer el color del texto (por ejemplo, blanco)
  myGLCD.setColor(255, 255, 255);

  // Establecer el color de fondo del texto (por ejemplo, negro)
  myGLCD.setBackColor(0, 0, 0);

  // Imprimir "Hola Mundo" en el centro de la pantalla
  myGLCD.print("Hola", CENTER, 120);  // 120 es la coordenada Y para centrar verticalmente en una pantalla de 240 píxeles de alto
}

void Imprimir(char cadena[]) {
  myGLCD.print(cadena, CENTER, 120);
}

#endif