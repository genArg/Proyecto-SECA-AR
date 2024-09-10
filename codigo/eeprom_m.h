//! Macros
#ifndef EEPROM_M_H
#define EEPROM_M_H

#include <EEPROM.h>
#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct gen_s* gen_t;

  struct gen_s {
    uint8_t color_pantalla;  // cambia la pantalla entre ocuro y claro
    uint8_t eeprom_activo;
    uint32_t parametro_temporal;
  };
  // genera la estructura inicila de variables
  gen_t InicializarGen();

  // carga los datos desde la eeprom a programa
  bool ActualizarDatos(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2);

  // guarda valores de caudalimetros
  bool GuardarEepromCaudal(caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2);

  // guarda valores de transmisor de presion
  bool GuardarEepromPresion(manometro_t manometro_1);

  // guarda valores de memoria SD
  bool GuardarEepromTarjetaSD(tarjeta_t tarjeta_1);

  // guarda valores de gen
  bool GuardarEepromGen(gen_t gen, uint8_t valor);

  // Lee valores de gen
  uint32_t LeerEepromGen(gen_t gen);

  //guarda valores de nivel
  bool GuardarEepromNivel(manometro_t nivel_alto, manometro_t nivel_bajo);


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gen_t InicializarGen() {
    gen_t gen = (gen_t)malloc(sizeof(struct gen_s));

    if (gen) {
      gen->color_pantalla = 0;
      gen->eeprom_activo = 0;
      gen->parametro_temporal = 0;
    }

    return &gen[0];
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // carga los datos desde la eeprom a programa
  bool ActualizarDatos(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2) {
    gen->color_pantalla = EEPROM.read(GEN_COLOR);
    return 1;
  }



#ifdef __cplusplus
}
#endif

#endif