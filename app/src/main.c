#include "board.h"
#include "c_func.h"
#include "asm_func.h"

#include <stdlib.h>
#include <stdnoreturn.h>

#define EJERCICIO9

#define DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
#define DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
#define DWT_CYCCNTENA_BIT       (1UL<<0)
#define DWT_DEMCR               (*((volatile uint32_t*)0xE000EDFC))

#define EnableCycleCounter()    DWT_CONTROL |= DWT_CYCCNTENA_BIT
#define GetCycleCounter()       DWT_CYCCNT
#define ResetCycleCounter()     DWT_CYCCNT = 0
#define DisableCycleCounter()   DWT_CONTROL &= ~DWT_CYCCNTENA_BIT
#define EnableAllDwt()          DWT_DEMCR |= 1<<24

// Variable que se incrementa cada vez que se llama al handler de interrupcion
// del SYSTICK.
static volatile uint32_t s_ticks = 0;


// Inicia soporte de la placa y periodo de la interrupcion del SYSTICK
// cada 1 milisegundo.
static void Inicio (void)
{
    Board_Init ();
    SystemCoreClockUpdate ();
    SysTick_Config (SystemCoreClock / 1000);

    EnableAllDwt();
    EnableCycleCounter();
}


// Segun la configuracion realizada en Inicio(), este handler de interrupcion
// se ejecutara cada 1 milisegundo.
void SysTick_Handler (void)
{
    ++ s_ticks;
}


static void Suma (void)
{
    const uint32_t A = 20;
    const uint32_t B = 30;

    const uint32_t SumResult_C = c_sum (A, B);
    const uint32_t SumResult_Asm = asm_sum (A, B);

    // Actividad de debug: SumResult_C y SumResult_Asm deberian contener el
    // mismo valor.
    __BKPT (0);

    (void) SumResult_C;
    (void) SumResult_Asm;
}

static void Zeros (void)
{
    uint32_t vector[5] = {0xFFFFFF, 1, 2, 10, 15};
    uint32_t longitud = 5;

    //c_zeros (vector, longitud);
    asm_zeros (vector, longitud);

    // Actividad de debug: SumResult_C y SumResult_Asm deberian contener el
    // mismo valor.
    __BKPT (0);
}

static void ProductoEscalar (void)
{
    uint32_t vectorIn[] = {2, 3, 4};
    uint32_t vectorOut[3] = {0};
    uint32_t longitud = 3;
    uint32_t escalar = 2;
    char printOutput[128];

    ResetCycleCounter();
    asm_productoEscalar32 (vectorIn, vectorOut, longitud, escalar);   // Implementacion en assembler
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_productoEscalar32: %d\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    

    c_zeros(vectorOut, longitud);  // limpio vectorOut
    ResetCycleCounter();
    c_productoEscalar32 (vectorIn, vectorOut, longitud, escalar); // Implementacion en C
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_productoEscalar32: %d\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    
}

static void ProductoEscalar16 (void)
{
    uint16_t vectorIn[] = {2, 3, 4};
    uint16_t vectorOut[3] = {0};
    uint32_t longitud = 3;
    uint16_t escalar = 2;
    char printOutput[128];

    ResetCycleCounter();
    asm_productoEscalar16 (vectorIn, vectorOut, longitud, escalar);   // Implementacion en assembler
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_productoEscalar16: %d\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);

    //c_zeros(vectorOut, longitud);  // limpio vectorOut
    ResetCycleCounter();
    c_productoEscalar16 (vectorIn, vectorOut, longitud, escalar); // Implementacion en C
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_productoEscalar16: %d\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    
}

static void ProductoEscalar12 (void)
{
    uint16_t vectorIn[1000];
    uint16_t vectorOut[1000] = {0};
    uint32_t longitud = 1000;
    uint16_t escalar = 2;
    char printOutput[128];

    for(uint32_t i = 0; i < 1000; i++) // Inicializacion de vectorIn
        vectorIn[i] = 0x0FFF;

    ResetCycleCounter();
    asm_productoEscalar12 (vectorIn, vectorOut, longitud, escalar);   // Implementacion en assembler
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_productoEscalar12: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);
    
    ResetCycleCounter();
    asm_productoEscalar12usat (vectorIn, vectorOut, longitud, escalar);   // Implementacion en assembler
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_productoEscalar12usat: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);

    //c_zeros(vectorOut, longitud);  // limpio vectorOut
    ResetCycleCounter();
    c_productoEscalar12 (vectorIn, vectorOut, longitud, escalar); // Implementacion en C
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_productoEscalar12: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);
}

static void FiltroVentana10 (void)
{
    uint16_t vectorIn[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint16_t vectorOut[10] = {0};
    uint32_t longitud = 10;
    char printOutput[128];

    ResetCycleCounter();
    c_filtroVentana10(vectorIn, vectorOut, longitud);
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_filtroVentana10: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    

    ResetCycleCounter();
    asm_filtroVentana10(vectorIn, vectorOut, longitud);
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_filtroVentana10: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    
}

static void Pack32to16 (void)
{
    int32_t vectorIn[] = {70000, -70000, 65536, 0xFFFFFFFF, 0x0001FFFF};
    int16_t vectorOut[5] = {0};
    uint32_t longitud = 5;
    char printOutput[128];

    ResetCycleCounter();
    c_pack32to16(vectorIn, vectorOut, longitud);
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_pack32to16: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    

    ResetCycleCounter();
    asm_pack32to16(vectorIn, vectorOut, longitud);
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_pack32to16: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);    
}

static void Max (void)
{
    int32_t vectorIn[] = {5000, -5000, 10, 10000, -10};
    uint32_t longitud = 5;
    int32_t maxPos;
    char printOutput[128];

    ResetCycleCounter();
    maxPos = c_max(vectorIn, longitud);
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Posicion maximo en C: %i\r\n", maxPos);
    Board_UARTPutSTR(printOutput);
    sprintf(printOutput, "Cycles c_max: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);     

    ResetCycleCounter();
    maxPos = asm_max(vectorIn, longitud);
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Posicion maximo en Asm: %i\r\n", maxPos);
    Board_UARTPutSTR(printOutput);  
    sprintf(printOutput, "Cycles asm_max: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);         
}

static void Invertir (void)
{
    uint16_t vectorIn[] = {1, 2, 3, 4, 5};
    uint32_t longitud = 5;
    char printOutput[128];

    ResetCycleCounter();
    c_invertir(vectorIn, longitud);
    volatile uint32_t cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles c_invertir: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);     

    ResetCycleCounter();
    asm_invertir(vectorIn, longitud);
    cycleCounter = GetCycleCounter();
    sprintf(printOutput, "Cycles asm_invertir: %u\r\n", cycleCounter);
    Board_UARTPutSTR(printOutput);     
}

static void LlamandoAMalloc (void)
{
    // De donde saca memoria malloc?
    // (se vera en clase)
    void * ptr = malloc (2048);

    (void) ptr;
}


static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo suma.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}


// Handler de la interrupcion "SVC" (Supervisor Call).
// Usado por el ejemplo "EjemploPrivilegiosSVC".
void SVC_Handler (void)
{
    // Se obtiene el valor del registro "control". El bit 0 indica el nivel
    // de privilegio en modo "Thread". Deberia ser 1: No privilegiado.
    uint32_t x = __get_CONTROL ();

    // Borra el bit 0. Nuevo valor 0: privilegiado.
    x &= ~1u;

    // Asigna el nuevo valor al registro "control". Esta operacion deberia
    // ejecutarse ya que todo manejador de interrupciones se ejecuta en modo
    // "Handler" con total privilegio.
    __set_CONTROL (x);
}


noreturn void LoopInfinito (void)
{
    while (1)
    {
        // Procesador en modo espera hasta que ocurra una interrupcion
        // (Bajo consumo)
        __WFI();
    }
}


int main (void)
{
    Inicio ();

    #ifdef EJERCICIO1 
    Zeros();
    #endif

    #ifdef EJERCICIO2 
    ProductoEscalar();
    #endif

    #ifdef EJERCICIO3 
    ProductoEscalar16();
    #endif

    #ifdef EJERCICIO4 
    ProductoEscalar12();
    #endif    

    #ifdef EJERCICIO5 
    FiltroVentana10();
    #endif    

    #ifdef EJERCICIO6 
    Pack32to16();
    #endif       

    #ifdef EJERCICIO7
    Max();
    #endif     

    #ifdef EJERCICIO9
    Invertir();
    #endif      

    //Suma ();

    //PrivilegiosSVC ();

    //LlamandoAMalloc ();

    LoopInfinito ();
}
