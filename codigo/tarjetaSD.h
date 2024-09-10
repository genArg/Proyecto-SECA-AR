//! Macros
#ifndef TARJETASD_H
#define TARJETASD_H

#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif
  //! puntero a una tarjeta SD
  typedef struct tarjeta_s* tarjeta_t;


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Estructra
  struct tarjeta_s {
    uint16_t codigo;
    uint8_t activo;
    uint32_t tiempo;
    char nombre_file[20];
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Inicia los valores de la tarjeta SD
  tarjeta_t InicializarTarjeta() {
    tarjeta_t tarjeta = (tarjeta_t)malloc(sizeof(struct tarjeta_s));

    if (tarjeta) {
      memset(tarjeta, 0, sizeof(tarjeta));
      tarjeta->codigo = 1;
      tarjeta->activo = 0;
      tarjeta->tiempo = TIEMPO_SD;
      sprintf(tarjeta->nombre_file, "%d.csv", tarjeta->codigo);
    }
    return &tarjeta[0];
  }

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int AbrirSD(tarjeta_t tarjeta) {
  // Deshabilita todas las interrupciones
  //noInterrupts();
  int auxiliar;
#if DEBUG == TRUE
  Serial.print("Initializing SD card...");
#endif
  for (int i = 0; i < INTENTOS; i++) {
    if (!SD.begin(PIN_CS)) {
      tarjeta->activo = 0;
#if DEBUG == TRUE
      Serial.println("No inicio SD");
#endif
      auxiliar = 0;
    } else {
      tarjeta->activo = 1;
      auxiliar = 1;
      sprintf(tarjeta->nombre_file, "%d.txt", tarjeta->codigo);
      break;
    }
  }

#if DEBUG == TRUE
  Serial.println("initialization done.");
#endif
  // Habilita todas las interrupciones
  //interrupts();
  return auxiliar;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IniciarDocumento(tarjeta_t tarjeta) {
  // Habilita todas las interrupciones
  //interrupts();
  File myFile;
  myFile = SD.open(tarjeta->nombre_file, FILE_WRITE);
  if (myFile) {
    myFile.println(tarjeta->nombre_file);
    myFile.print("Fecha, ");
    myFile.print("Hora, ");
    myFile.print("Presion, ");
    myFile.print("Caudal, ");
    myFile.print("Nivel Estatico, ");
    myFile.print("Nivel Dinamico, ");
    myFile.println("Caudal Especifico");
    myFile.close();  // Cerramos el archivo
#if DEBUG == TRUE
    Serial.println("TERMINO INICIO");
#endif
  } else {
#if DEBUG == TRUE
    Serial.println("Error al abrir el archivo para escritura");
#endif
  }
  // Habilita todas las interrupciones
  //interrupts();
  return;
}


#endif