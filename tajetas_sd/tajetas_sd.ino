#include <SPI.h>
#include <SD.h>

#define DEBUG TRUE
#define PIN_CS 53 // 10  //53

File myFile;
char nombre_file[20];
int codigo = 1234;

void setup() {
#if DEBUG == TRUE
  Serial.begin(9600);
#endif
  delay(100);
#if DEBUG == TRUE
  Serial.println("Ingresar 0 para guardar datos y 1 para dar formato");
#endif
}

void loop() {
  // Lee la cadena de texto ingresada si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    String inputString = Serial.readString();  // Lee la cadena de texto ingresada
    inputString.trim();                        // Elimina los posibles espacios en blanco al inicio y al final
    if (inputString == "0") {
      if (AbrirSD()) {
        GuardarDatos(nombre_file);
      }
    }
    if (inputString == "1") {
      if (AbrirSD()) {
        IniciarDocumento(nombre_file);
      }
    }
#if DEBUG == TRUE
    Serial.println("Ingresar 0 para guardar datos y 1 para dar formato");
#endif
  }
}

int AbrirSD() {
  delay(100);
#if DEBUG == TRUE
  Serial.println("Initializing SD card...");
#endif
  sprintf(nombre_file, "%d.csv", codigo);
  if (!SD.begin(PIN_CS)) {
#if DEBUG == TRUE
    Serial.println("No inicio SD");
#endif
    return 0;
  }
  delay(100);
#if DEBUG == TRUE
  Serial.println("initialization done.");
#endif
  return 1;
}

void IniciarDocumento(char nombre_file[]) {
  myFile = SD.open(nombre_file, FILE_WRITE);
  if (myFile) {
    myFile.println(nombre_file);
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
}
void GuardarDatos(char nombre_file[]) {
  if (SD.exists(nombre_file)) {
    myFile = SD.open(nombre_file, FILE_WRITE);
  } else {
    myFile = SD.open(nombre_file, FILE_WRITE);
  }
  if (myFile) {
    myFile.print("dato 1, ");
    myFile.print("dato 2, ");
    myFile.print("dato 3, ");
    myFile.print("dato 4, ");
    myFile.print("dato 5, ");
    myFile.print("dato 6, ");
    myFile.println("dato 7");
    myFile.close();  // Cerramos el archivo
#if DEBUG == TRUE
    Serial.println("TERMINO ESCRITURA");
#endif
  } else {
#if DEBUG == TRUE
    Serial.println("Error al abrir el archivo para escritura");
#endif
  }
}
