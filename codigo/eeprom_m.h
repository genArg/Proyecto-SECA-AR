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
    uint8_t automatico;
  };
  // genera la estructura inicila de variables
  gen_t InicializarGen();

  // carga los datos desde la eeprom a programa
  bool ActualizarDatosEeprom(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2, tarjeta_t tarjeta_1);

  // guarda valores en la memoria eeprom actuales
  bool GuardarDatosEeprom(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2, tarjeta_t tarjeta_1);


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gen_t InicializarGen() {
    gen_t gen = (gen_t)malloc(sizeof(struct gen_s));

    if (gen) {
      gen->color_pantalla = 0;
      gen->eeprom_activo = 0;
      gen->parametro_temporal = 0;
      gen->automatico = 1;
    }

    return &gen[0];
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // carga los datos desde la eeprom a programa
  bool ActualizarDatosEeprom(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2, tarjeta_t tarjeta_1) {
    // Del menu gen
    gen->color_pantalla = EEPROM.read(GEN_COLOR);
    gen->eeprom_activo = EEPROM.read(GEN_PARAM_MEM);
    gen->automatico = EEPROM.read(GEN_AUTOMATICO);

    // Del menu nivel
    uint8_t aux_mult = EEPROM.read(NIV_MULTIPLICADOR);
    uint8_t aux_sum = EEPROM.read(NIV_SUMADOR);

    nivel_alto->constante = EEPROM.read(NIV_CONSTANTE) * aux_mult + aux_sum;
    nivel_bajo->constante = EEPROM.read(NIV_CONSTANTE) * aux_mult + aux_sum;

    nivel_alto->corriente_min = EEPROM.read(NIV_I_MIN) * aux_mult + aux_sum;
    nivel_bajo->corriente_min = EEPROM.read(NIV_I_MIN) * aux_mult + aux_sum;

    nivel_alto->corriente_max = EEPROM.read(NIV_I_MAX) * aux_mult + aux_sum;
    nivel_bajo->corriente_max = EEPROM.read(NIV_I_MAX) * aux_mult + aux_sum;

    nivel_alto->resistencia = EEPROM.read(NIV_RESITENCIA) * aux_mult + aux_sum;
    nivel_bajo->resistencia = EEPROM.read(NIV_RESITENCIA) * aux_mult + aux_sum;

    nivel_alto->presion_min = EEPROM.read(NIV_VAL_MIN) * aux_mult + aux_sum;
    nivel_bajo->presion_min = EEPROM.read(NIV_VAL_MIN) * aux_mult + aux_sum;

    nivel_alto->presion_max = EEPROM.read(NIV_VAL_MAX) * aux_mult + aux_sum;
    nivel_bajo->presion_max = EEPROM.read(NIV_VAL_MAX) * aux_mult + aux_sum;

    nivel_alto->intervalo_nivel = EEPROM.read(NIV_INTERVALO) * aux_mult + aux_sum;
    nivel_bajo->intervalo_nivel = EEPROM.read(NIV_INTERVALO) * aux_mult + aux_sum;

    // Del menu presion
    aux_mult = EEPROM.read(PRESION_MULTIPLICADOR);
    aux_sum = EEPROM.read(PRESION_SUMADOR);

    manometro_1->constante = EEPROM.read(PRESION_CONSTANTE) * aux_mult + aux_sum;
    manometro_1->corriente_min = EEPROM.read(PRESION_I_MIN) * aux_mult + aux_sum;
    manometro_1->corriente_max = EEPROM.read(PRESION_I_MAX) * aux_mult + aux_sum;
    manometro_1->resistencia = EEPROM.read(PRESION_RESISTENCIA) * aux_mult + aux_sum;
    manometro_1->presion_min = EEPROM.read(PRESION_PRE_MIN) * aux_mult + aux_sum;
    manometro_1->presion_max = EEPROM.read(PRESION_PRE_MAX) * aux_mult + aux_sum;

    // Del menu caudal
    aux_mult = EEPROM.read(CAUDAL_MULTIPLICADOR);
    aux_sum = EEPROM.read(CAUDAL_SUMADOR);

    caudalimetro_1->constante = EEPROM.read(CAUDAL_CONSTANTE) * aux_mult + aux_sum;
    caudalimetro_2->constante = EEPROM.read(CAUDAL_CONSTANTE) * aux_mult + aux_sum;
    caudalimetro_1->habilitacion = EEPROM.read(CAUDAL_ENTRADA_1) * aux_mult + aux_sum;
    caudalimetro_2->habilitacion = EEPROM.read(CAUDAL_ENTRADA_2) * aux_mult + aux_sum;

    // del menu sd
    aux_mult = EEPROM.read(MEMORIA_MULTIPLICADOR);
    aux_sum = EEPROM.read(MEMORIA_SUMADOR);

    tarjeta_1->activo = EEPROM.read(MEMORIA_HABILITADA) * aux_mult + aux_sum;
    tarjeta_1->codigo = EEPROM.read(MEMORIA_CODIGO) * aux_mult + aux_sum;
    tarjeta_1->tiempo = EEPROM.read(MEMORIA_INTERVALO) * aux_mult + aux_sum;

    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // carga los datos desde la eeprom a programa
  bool GuardarDatosEeprom(gen_t gen, manometro_t nivel_alto, manometro_t nivel_bajo, manometro_t manometro_1, caudalimetro_t caudalimetro_1, caudalimetro_t caudalimetro_2, tarjeta_t tarjeta_1) {
    // Del menu gen
    EEPROM.update(GEN_COLOR, gen->color_pantalla);
    EEPROM.update(GEN_PARAM_MEM, gen->eeprom_activo);
    EEPROM.update(GEN_AUTOMATICO, gen->automatico);

    uint8_t aux_mult = 1;
    uint8_t aux_sum = 0;

    // Del menu nivel

    EEPROM.update(NIV_CONSTANTE, nivel_alto->constante);
    EEPROM.update(NIV_I_MIN, nivel_alto->corriente_min);
    EEPROM.update(NIV_I_MAX, nivel_alto->corriente_max);
    EEPROM.update(NIV_RESITENCIA, nivel_alto->resistencia);
    EEPROM.update(NIV_VAL_MIN, nivel_alto->presion_min);
    EEPROM.update(NIV_VAL_MAX, nivel_alto->presion_max);
    EEPROM.update(NIV_INTERVALO, nivel_alto->intervalo_nivel);
    EEPROM.update(NIV_MULTIPLICADOR, aux_mult);
    EEPROM.update(NIV_SUMADOR, aux_sum);

    // Del menu presion

    EEPROM.update(PRESION_CONSTANTE, manometro_1->constante);
    EEPROM.update(PRESION_I_MIN, manometro_1->corriente_min);
    EEPROM.update(PRESION_I_MAX, manometro_1->corriente_max);
    EEPROM.update(PRESION_RESISTENCIA, manometro_1->resistencia);
    EEPROM.update(PRESION_PRE_MIN, manometro_1->presion_min);
    EEPROM.update(PRESION_PRE_MAX, manometro_1->presion_max);
    EEPROM.update(PRESION_MULTIPLICADOR, aux_mult);
    EEPROM.update(PRESION_SUMADOR, aux_sum);

    // Del menu caudal

    EEPROM.update(CAUDAL_CONSTANTE, caudalimetro_1->constante);
    EEPROM.update(CAUDAL_CONSTANTE, caudalimetro_2->constante);
    EEPROM.update(CAUDAL_ENTRADA_1, caudalimetro_1->habilitacion);
    EEPROM.update(CAUDAL_ENTRADA_2, caudalimetro_2->habilitacion);
    EEPROM.update(CAUDAL_MULTIPLICADOR, aux_mult);
    EEPROM.update(CAUDAL_SUMADOR, aux_sum);

    // del menu sd

    EEPROM.update(MEMORIA_HABILITADA, tarjeta_1->activo);
    EEPROM.update(MEMORIA_CODIGO, tarjeta_1->codigo);
    EEPROM.update(MEMORIA_INTERVALO, tarjeta_1->tiempo);
    EEPROM.update(MEMORIA_MULTIPLICADOR, aux_mult);
    EEPROM.update(MEMORIA_SUMADOR, aux_sum);

    return 1;
  }


#ifdef __cplusplus
}
#endif

#endif