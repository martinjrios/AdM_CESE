#pragma once

#include <stdint.h>


uint32_t c_sum (uint32_t firstOperand, uint32_t secondOperand);
void c_zeros (uint32_t *vector, uint32_t longitud);  // Ejercicio 1
void c_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar); // Ejercicio 2
void c_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); // Ejercicio 3
void c_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); // Ejercicio 4
void c_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);   // Ejercicio 5
void c_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);    // Ejercicio 6
int32_t c_max (int32_t * vectorIn, uint32_t longitud);    // Ejercicio 7
void c_invertir (uint16_t * vector, uint32_t longitud);   // Ejercicio 9
void c_eco (int16_t * vector, uint32_t longitud);   // Ejercicio 10