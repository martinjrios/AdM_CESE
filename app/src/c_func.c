#include "c_func.h"


uint32_t c_sum (uint32_t firstOperand, uint32_t secondOperand)
{
    return (firstOperand + secondOperand);
}

// Ejercicio 1
void c_zeros (uint32_t *vector, uint32_t longitud)
{
    for(uint32_t i = 0; i < longitud; i++) vector[i] = 0;
}

// Ejercicio 2
void c_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
{
    for(uint32_t i = 0; i < longitud; i++) vectorOut[i] = vectorIn[i] * escalar;
}

// Ejercicio 3
void c_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
    for(uint32_t i = 0; i < longitud; i++) vectorOut[i] = vectorIn[i] * escalar;
}

// Ejercicio 4
void c_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
    uint16_t prodTemp;
    for(uint32_t i = 0; i < longitud; i++)
    {
        prodTemp = vectorIn[i] * escalar;
        vectorOut[i] = (prodTemp & 0xF000) >> 12 != 0 ? 0xFFF : prodTemp ;
    }
}

// Ejercicio 5
void c_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn)
{
    static const uint8_t windows_size = 10; 
    int suma_parcial = 0;
    uint32_t i = 0;
    uint32_t pos = 0;

    for (i = 0; i < longitudVectorIn; i++) {
        suma_parcial = 0;
        uint8_t j = i;

        for (j = i; j < i + windows_size; j++) {
            pos = j % longitudVectorIn;
            suma_parcial += vectorIn[pos];
        }
        vectorOut[i] = suma_parcial/windows_size;
    }
}

// Ejercicio 6
void c_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud)
{
    for(uint32_t i = 0; i < longitud; i++) vectorOut[i] = (int16_t)(vectorIn[i] >> 16);
}

// Ejercicio 7
int32_t c_max (int32_t * vectorIn, uint32_t longitud)
{
    int32_t current_max = vectorIn[0], max_pos = 0;

    for(uint32_t i = 0; i < longitud; i++)
    {
        if(vectorIn[i] > current_max)
        {
            current_max = vectorIn[i];
            max_pos = i;
        }
    }

    return max_pos;
}