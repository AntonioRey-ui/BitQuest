/* game.c - Logica del juego: carga de mapas, movimiento y reglas */
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "asm_funcs.h"

/* Construye la ruta del archivo de mapa para un nivel dado */
static void ruta_mapa(int nivel, char *buf, size_t n)
{
    snprintf(buf, n, "maps/level%d.txt", nivel + 1);
}

/* Carga el nivel indicado desde maps/levelN.txt.
 * Devuelve 1 si todo salio bien, 0 si hubo error. */
int juego_cargar_nivel(Juego *j, int nivel)
{
    char ruta[64];
    FILE *f;
    int fila, col, c;

    if (nivel < 0 || nivel >= MAX_NIVELES)
        return 0;

    ruta_mapa(nivel, ruta, sizeof(ruta));
    f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "No se pudo abrir el mapa: %s\n", ruta);
        return 0;
    }

    /* Inicializa el estado del nivel */
    memset(j->mapa, T_PISO, sizeof(j->mapa));
    j->nivel         = nivel;
    j->monedas       = 0;
    j->tiene_llave   = 0;
    j->terminado     = 0;
    j->pasos         = 0;
    j->px = j->py    = 1;

    /* Lee el archivo caracter por caracter, llenando la matriz */
    fila = 0;
    col  = 0;
    while ((c = fgetc(f)) != EOF && fila < MAPA_ALTO) {
        if (c == '\n') {            /* fin de fila */
            fila++;
            col = 0;
            continue;
        }
        if (c == '\r')              /* ignora retorno de carro de Windows */
            continue;
        if (col < MAPA_ANCHO) {
            if (c == T_JUGADOR) {   /* posicion inicial del jugador */
                j->px = col;
                j->py = fila;
                j->mapa[asm_indice(MAPA_ANCHO, col, fila)] = T_PISO;
            } else {
                j->mapa[asm_indice(MAPA_ANCHO, col, fila)] = (char)c;
            }
            col++;
        }
    }
    fclose(f);

    /* Cuenta las monedas del nivel usando la funcion en ensamblador */
    j->monedas_total = (int)asm_contar(j->mapa,
                                       (long)(MAPA_ALTO * MAPA_ANCHO),
                                       T_MONEDA);
    return 1;
}

/* Intenta mover al jugador en (dx, dy). Aplica las reglas del juego. */
void juego_mover(Juego *j, int dx, int dy)
{
    int nx = j->px + dx;
    int ny = j->py + dy;
    char destino;

    if (j->terminado)
        return;

    /* Limites del mapa */
    if (nx < 0 || nx >= MAPA_ANCHO || ny < 0 || ny >= MAPA_ALTO)
        return;

    /* Caracter de la casilla destino (funcion en ensamblador) */
    destino = asm_celda(j->mapa, MAPA_ANCHO, nx, ny);

    switch (destino) {
    case T_PARED:
        return;                     /* no se puede atravesar */
    case T_PUERTA:
        if (!j->tiene_llave)        /* puerta cerrada sin llave */
            return;
        break;                      /* con llave: se abre y pasa */
    case T_MONEDA:
        j->monedas++;
        break;
    case T_LLAVE:
        j->tiene_llave = 1;
        break;
    case T_SALIDA:
        j->terminado = 1;           /* nivel completado */
        break;
    default:
        break;                      /* piso normal */
    }

    /* Despeja la casilla recogida y actualiza posicion */
    j->mapa[asm_indice(MAPA_ANCHO, nx, ny)] = T_PISO;
    j->px = nx;
    j->py = ny;
    j->pasos++;
}

int juego_ganado(const Juego *j)
{
    return j->terminado;
}
