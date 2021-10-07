#pragma once

#include <stdint.h>


extern uint32_t asm_sum (uint32_t firstOperand, uint32_t secondOperand);
extern void asm_svc (void);
extern void asm_zeros (uint32_t *vector, uint32_t longitud);
extern void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar); // Ejercicio 2
extern void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); // Ejercicio 3
extern void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); // Ejercicio 4
extern void asm_productoEscalar12usat (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); // Ejercicio 4
