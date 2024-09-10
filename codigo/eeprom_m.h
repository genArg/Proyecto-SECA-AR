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

  // guarda valores en la memoria eeprom actuales
  bool GuardarDatos(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2);


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
    gen->eeprom_activo = EEPROM.read(GEN_PARAM_MEM);
    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // carga los datos desde la eeprom a programa
  bool GuardarDatos(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2) {
    EEPROM.update(GEN_COLOR, gen->color_pantalla);
    EEPROM.update(GEN_PARAM_MEM, gen->eeprom_activo);
    return 1;
  }


#ifdef __cplusplus
}
#endif

#endif