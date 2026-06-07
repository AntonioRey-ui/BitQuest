/* render.c - Dibuja la ventana visible de 20x20 y la informacion del HUD */
#include <stdio.h>
#include "render.h"
#include "asm_funcs.h"

#ifdef _WIN32
#include <windows.h>
#endif

void render_init(void)
{
#ifdef _WIN32
    /* Habilita las secuencias de escape ANSI en la consola de Windows 10+ */
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(h, &modo))
        SetConsoleMode(h, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void render_limpiar(void)
{
    /* Borra la pantalla y coloca el cursor arriba a la izquierda */
    fputs("\033[2J\033[H", stdout);
}

/* Caracter visible para cada tipo de casilla */
static char glifo(char celda)
{
    switch (celda) {
    case T_PARED:  return '#';
    case T_MONEDA: return 'o';
    case T_LLAVE:  return 'K';
    case T_PUERTA: return 'D';
    case T_SALIDA: return 'E';
    default:       return ' ';   /* piso -> espacio para mejor contraste */
    }
}

void render_juego(const Juego *j)
{
    int cam_x, cam_y, fila, col;

    render_limpiar();

    /* La camara centra al jugador, pero se mantiene dentro del mapa.
     * El recorte a los bordes lo hace la funcion en ensamblador asm_clamp. */
    cam_x = asm_clamp(j->px - VENTANA / 2, 0, MAPA_ANCHO - VENTANA);
    cam_y = asm_clamp(j->py - VENTANA / 2, 0, MAPA_ALTO  - VENTANA);

    printf("=== BitQuest ===  Nivel %d/%d\n", j->nivel + 1, MAX_NIVELES);

    /* Dibuja la ventana 20x20 */
    for (fila = 0; fila < VENTANA; fila++) {
        for (col = 0; col < VENTANA; col++) {
            int mx = cam_x + col;
            int my = cam_y + fila;
            if (mx == j->px && my == j->py) {
                putchar('@');                   /* jugador */
            } else {
                char celda = asm_celda(j->mapa, MAPA_ANCHO, mx, my);
                putchar(glifo(celda));
            }
        }
        putchar('\n');
    }

    /* HUD: estado de la partida */
    printf("Monedas: %d/%d   Llave: %s   Pasos: %d\n",
           j->monedas, j->monedas_total,
           j->tiene_llave ? "SI" : "NO", j->pasos);
    printf("Mover: W/A/S/D o flechas   Salir: Q\n");
    if (!j->tiene_llave && j->monedas_total > 0)
        printf("Busca la llave (K) para abrir la puerta (D) y llegar a la salida (E).\n");
    else
        printf("Usa la llave en la puerta (D) y alcanza la salida (E).\n");

    fflush(stdout);
}

void render_mensaje_final(const Juego *j)
{
    render_limpiar();
    printf("\n  *** NIVEL %d COMPLETADO ***\n\n", j->nivel + 1);
    printf("  Monedas recogidas: %d/%d\n", j->monedas, j->monedas_total);
    printf("  Pasos realizados : %d\n\n", j->pasos);
    fflush(stdout);
}
