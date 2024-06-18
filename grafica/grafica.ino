#include <UTFTGLUE.h>

// Declarar la fuente BigFont
extern uint8_t BigFont[];

UTFTGLUE myGLCD(0, A2, A1, A3, A4); // Asegúrate de usar el controlador y pines correctos para tu pantalla

void setup() {
  myGLCD.InitLCD();
  myGLCD.clrScr();

  // Usar la fuente BigFont
  myGLCD.setFont(BigFont);

  // Establecer el color del texto (por ejemplo, blanco)
  myGLCD.setColor(255, 255, 255);

  // Establecer el color de fondo del texto (por ejemplo, negro)
  myGLCD.setBackColor(0, 0, 0);

  // Imprimir "Hola Mundo" en el centro de la pantalla
  myGLCD.print("Hola Mundo", CENTER, 120); // 120 es la coordenada Y para centrar verticalmente en una pantalla de 240 píxeles de alto
}

void loop() {
  // No se necesita nada aquí para este ejemplo
}

