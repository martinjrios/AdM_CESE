.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_sum
.global asm_svc
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_productoEscalar12usat
.global asm_filtroVentana10
.global asm_pack32to16
.global asm_max
.global asm_invertir
.global asm_eco

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_zero (uint32_t *vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: *vector
@   r1: longitud
@
.thumb_func
    asm_zeros:
        @ Registros usados
        @   r2: valor cero
        mov r2, 0
    .loop:
        str r2, [r0], 4    @ sizeof(uint32_t) == 4 , *vector++ = r2
        subs r1, 1           @ --longitud        
        bne .loop
        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_productoEscalar32:
        @ Registros usados
        @   r4: resultado de la multiplicacion
        @   r5: contenido de cada posicion del vectorIn
        push {r4, r5}
        .ej2_loop:
            ldr r5, [r0], 4     @ r5 = *r0; r0+=4
            mul r4, r5, r3      @ r4 = r5*r3
            str r4, [r1], 4     @ *r1 = r4; r1+=4
            subs r2, 1          @ r2-=1
            bne .ej2_loop
        pop {r4, r5}
        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_productoEscalar16:
        @ Registros usados
        @   r4: resultado de la multiplicacion
        push {r4}
        .ej3_loop:
            ldrh r4, [r0], 2    @ r4 = *r0; r0+=2 (half word)
            mul r4, r3          @ r4 = r4*r3
            strh r4, [r1], 2    @ *r1 = r4; r1+=2 (half word)
            subs r2, 1          @ r2-=1
            bne .ej3_loop
        pop {r4}
        bx lr            

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_productoEscalar12:
        @ Registros usados
        @   r4: resultado de la multiplicacion
        @   r5: aulixiar para comparar contra cero
        push {r4, r5}
        mov r5, 0
        .ej4_loop:
            ldrh r4, [r0], 2    @ r4 = *r0; r0+=2 (half word)
            mul r4, r3          @ r4 = r4*r3
            cmp r5, r4, lsr #12 @ if(r4 >> 12 == 0)
            beq .not_saturate
            mov r4, #4095
        .not_saturate:
            strh r4, [r1], 2    @ *r1 = r4; r1+=2 (half word)
            subs r2, 1          @ r2-=1
            bne .ej4_loop
        pop {r4, r5}
        bx lr    

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar12usat (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
@   Se realiza la saturacion utilizando las instrucciones de saturacion.
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@   r3: escalar
@
.thumb_func
    asm_productoEscalar12usat:
        @ Registros usados
        @   r4: resultado de la multiplicacion
        push {r4}
        .ej4bis_loop:
            ldrh r4, [r0], 2    @ r4 = *r0; r0+=2 (half word)
            mul r4, r3          @ r4 = r4*r3
            usat r4,12,r4
            strh r4, [r1], 2    @ *r1 = r4; r1+=2 (half word)
            subs r2, 1          @ r2-=1
            bne .ej4bis_loop
        pop {r4}
        bx lr  

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@
@ Constante para Assembler
.equ    WINDOWS_SIZE,10 @ Longitud de la ventana 

.thumb_func
    asm_filtroVentana10:
        push {r4-r11}
        mov r5, r0
        mov r7, WINDOWS_SIZE
        mov r6, 0                  @ i = 0
        .vector_loop:
            mov r3, 0               @ suma parcial = 0
            mov r9, r6              @ j = i
            mov r5, r0   
            .windows_loop:
                @ calculo el modulo sobre la longitud del vector: j % longitud
                udiv r10, r9, r2      
                mul r11, r10, r2      
                sub r10, r9, r11        
                
                add r5, r0, r10, LSL #1     @ Incremento la direccion de la posicion a leer del VectorIn 
                add r9, 1                   @ j++

                ldrh r4, [r5]       @ cargo el valor del VectorIn 
                add r3, r4          @ lo sumo a lo acumulado
                
                subs r7, 1
                bne .windows_loop
            mov r7, WINDOWS_SIZE

            udiv r8, r3, r7         @ hago el promedio de los valores sumados
            strh r8, [r1], 2        @ lo guardo en el vector de salida
            add r6,1                @ i++
            cmp r2, r6              @ comparo si ya se alcanzo la longitud del vector
            bne .vector_loop
        pop {r4-r11}
        bx lr          

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: *vectorOut
@   r2: longitud
@
.thumb_func
    asm_pack32to16:
        push {r4}
        .ej6_loop:
            ldr r4, [r0], 4     @ guardo el valor del vector de entrada
            lsr r4, #16         @ me quedo con los 2 MSB
            strh r4, [r1], 2    @ guardo la parte baja de r4 en el vector de salida
            subs r2, 1          @ r2-=1
            bne .ej6_loop
        pop {r4}
        bx lr         

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@  int32_t max (int32_t * vectorIn, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: longitud
@
@   return r0

.thumb_func
    asm_max:
        @ Registros usados
        @   r2: maximo temporal
        @   r3: posicion del maximo   
        @   r5: indice del vector
        push {r4, r5}
        ldr r2, [r0]            @ inicializo el maximo con el valor de la primera posicion del vector
        mov r3, 0               @ inicializo en 0
        mov r5, 0               @ inicializo en 0
        .ej7_loop:
            ldr r4, [r0], 4     @ guardo el valor del vector de entrada
            cmp r4, r2          @ comparo si el valor leido es mayor al maximo temporal
            blt .is_less        
            mov r2, r4          @ si es mayor guardo el nuevo maximo
            mov r3, r5          @ actualizo la posicion del maximo
        .is_less:
            add r5, 1           @ incremento el indice del vector
            subs r1, 1          @ decremento el contador del loop
            bne .ej7_loop
        mov r0, r3            @ return max pos
        pop {r4, r5}
        bx lr          

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void invertir (uint16_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: longitud
@
.thumb_func
    asm_invertir:
        @ Registros usados
        @   r2: utilizado para recorrer el vector de atras hacia adelante
        @   r3: utilizado para guardar el valor de adelante
        @   r4: utilizado para guardar el valor de atras

        push {r4}
        mov r2, r0              @ hago una copia de la direccion del vector
        add r2, r2, r1, lsl 1   @ me posiciono en la ultima posicion
        sub r2, 2               @ la ultima posicion esta en longitud-1
        lsr r1, 1               @ La cantidad de iteraciones es la mitad de la longitud ya que
                                @ voy cambiando los valores de adelante hacia atras y atras hacia
                                @ adelante en cada ciclo.
        .ej9_loop:
            ldrh r3, [r0]        @ guardo el valor de adelante en una variable temporal
            ldrh r4, [r2]        @ guardo el valor de atras en una variable temporal
            strh r4, [r0], 2     @ el valor de atras pasa adelante
            strh r3, [r2], -2    @ el valor de adelante pasa atras
            subs r1, 1
            bne .ej9_loop
        pop {r4}
        bx lr          

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void c_eco (int16_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: *vectorIn
@   r1: longitud
@
.equ    SAMPLE_RATE,44100
.equ    SAMPLE_DELAY,882

.thumb_func
    asm_eco:
        push {r4}
        mov r3, SAMPLE_DELAY           @ i = SAMPLE_DELAY
        lsl r3, 1
        add r3, r3, r0
        .ej10_loop:
            ldr r4, [r0], 2
            ldr r5, [r3], 2

            lsr r5, 2                  @ r5/2
            sadd16 r4, r4, r5

            subs r1, 1
            bne .ej10_loop
        pop {r4}
        bx lr             