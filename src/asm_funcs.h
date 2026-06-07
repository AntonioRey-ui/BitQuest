/* asm_funcs.h - Prototipos de las funciones implementadas en NASM
 * (ver asm/funciones.asm). Estas funciones son las "funciones obligatorias"
 * en ensamblador exigidas por el proyecto.
 */
#ifndef ASM_FUNCS_H
#define ASM_FUNCS_H

/* Cuenta cuantos bytes de 'buf' (longitud 'len') son iguales a 'objetivo'. */
long asm_contar(const char *buf, long len, char objetivo);

/* Devuelve el caracter del mapa lineal en la posicion (x, y). */
char asm_celda(const char *mapa, int ancho, int x, int y);

/* Calcula el indice lineal: y * ancho + x. */
int asm_indice(int ancho, int x, int y);

/* Limita 'valor' al rango [min, max]. */
int asm_clamp(int valor, int min, int max);

#endif /* ASM_FUNCS_H */
