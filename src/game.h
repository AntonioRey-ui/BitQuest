/* game.h - Estructuras y constantes principales de BitQuest */
#ifndef GAME_H
#define GAME_H

#define MAPA_ANCHO   60      /* columnas del mapa completo */
#define MAPA_ALTO    60      /* filas del mapa completo    */
#define VENTANA      20      /* lado de la ventana visible (20x20) */
#define MAX_NIVELES  3       /* numero de mapas/niveles    */

/* Caracteres usados en los archivos de mapa (maps/levelN.txt) */
#define T_PARED   '#'
#define T_PISO    '.'
#define T_MONEDA  'o'
#define T_LLAVE   'K'
#define T_PUERTA  'D'
#define T_SALIDA  'E'
#define T_JUGADOR '@'

/* Estado completo de una partida */
typedef struct {
    char mapa[MAPA_ALTO * MAPA_ANCHO]; /* mapa lineal (fila por fila) */
    int  px, py;            /* posicion del jugador */
    int  monedas;           /* monedas recogidas */
    int  monedas_total;     /* monedas que tenia el nivel */
    int  tiene_llave;       /* 1 si ya recogio la llave */
    int  nivel;             /* nivel actual (0..MAX_NIVELES-1) */
    int  terminado;         /* 1 si llego a la salida */
    int  pasos;             /* contador de movimientos */
} Juego;

/* API del juego (game.c) */
int  juego_cargar_nivel(Juego *j, int nivel);
void juego_mover(Juego *j, int dx, int dy);
int  juego_ganado(const Juego *j);

#endif /* GAME_H */
