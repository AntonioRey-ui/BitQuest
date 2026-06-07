; ============================================================================
;  funciones.asm  -  Funciones obligatorias de BitQuest implementadas en NASM
; ----------------------------------------------------------------------------
;  Se ensambla para 64 bits en Windows (win64) y macOS (macho64).
;
;  Convencion de llamada (segun el formato de salida):
;    win64        ->  argumentos en RCX, RDX, R8, R9   (Microsoft x64)
;    macho64/elf  ->  argumentos en RDI, RSI, RDX, RCX  (System V AMD64)
;
;  En macOS los simbolos llevan guion bajo inicial; eso se resuelve al
;  ensamblar con la opcion "--prefix _" (ver build.bat / Makefile), por lo
;  que aqui los nombres se escriben sin guion bajo.
; ============================================================================

%ifidn __OUTPUT_FORMAT__, win64
    %define ARG1 rcx          ; primer  argumento
    %define ARG2 rdx          ; segundo argumento
    %define ARG3 r8           ; tercer  argumento
    %define ARG4 r9           ; cuarto  argumento
    %define ARG1d ecx
    %define ARG2d edx
    %define ARG3d r8d
    %define ARG4d r9d
    %define ARG3b r8b
    %define ARG4b r9b
%else
    %define ARG1 rdi
    %define ARG2 rsi
    %define ARG3 rdx
    %define ARG4 rcx
    %define ARG1d edi
    %define ARG2d esi
    %define ARG3d edx
    %define ARG4d ecx
    %define ARG3b dl
    %define ARG4b cl
%endif

global asm_contar
global asm_celda
global asm_clamp
global asm_indice

section .text

; ----------------------------------------------------------------------------
; long asm_contar(const char *buf, long len, char objetivo)
;   Recorre 'buf' contando cuantos bytes son iguales a 'objetivo'.
;   Se usa para contar las monedas que quedan en el mapa.
;   Retorno: numero de coincidencias en RAX.
; ----------------------------------------------------------------------------
asm_contar:
    xor     rax, rax            ; contador = 0
    test    ARG1, ARG1          ; buf == NULL ?
    jz      .fin
    test    ARG2, ARG2          ; len <= 0 ?
    jle     .fin
    mov     r10, ARG1           ; r10 = puntero recorre el buffer
    mov     r11, ARG2           ; r11 = bytes restantes
    movzx   r9d, ARG3b          ; r9b = byte objetivo a buscar (3er arg)
.bucle:
    mov     dl, [r10]           ; byte actual
    cmp     dl, r9b
    jne     .siguiente
    inc     rax                 ; coincide -> contador++
.siguiente:
    inc     r10
    dec     r11
    jnz     .bucle
.fin:
    ret

; ----------------------------------------------------------------------------
; char asm_celda(const char *mapa, int ancho, int x, int y)
;   Devuelve el caracter ubicado en (x, y) dentro de un mapa lineal
;   guardado por filas:  indice = y * ancho + x
;   Retorno: el byte del mapa en AL.
; ----------------------------------------------------------------------------
asm_celda:
    movsxd  rax, ARG4d          ; rax = y
    movsxd  r10, ARG2d          ; r10 = ancho
    imul    rax, r10            ; rax = y * ancho
    movsxd  r11, ARG3d          ; r11 = x
    add     rax, r11            ; rax = y * ancho + x
    add     rax, ARG1           ; rax = &mapa[indice]
    movzx   eax, byte [rax]     ; al = mapa[indice]
    ret

; ----------------------------------------------------------------------------
; int asm_indice(int ancho, int x, int y)
;   Calcula el indice lineal y * ancho + x. Retorno en EAX.
; ----------------------------------------------------------------------------
asm_indice:
    mov     eax, ARG3d          ; eax = y
    imul    eax, ARG1d          ; eax = y * ancho
    add     eax, ARG2d          ; eax = y * ancho + x
    ret

; ----------------------------------------------------------------------------
; int asm_clamp(int valor, int min, int max)
;   Limita 'valor' al rango [min, max]. Se usa para mantener la ventana
;   visible (camara) dentro de los bordes del mapa. Retorno en EAX.
; ----------------------------------------------------------------------------
asm_clamp:
    mov     eax, ARG1d          ; eax = valor
    cmp     eax, ARG2d          ; valor < min ?
    jge     .chk_max
    mov     eax, ARG2d          ; eax = min
    jmp     .fin
.chk_max:
    cmp     eax, ARG3d          ; valor > max ?
    jle     .fin
    mov     eax, ARG3d          ; eax = max
.fin:
    ret
