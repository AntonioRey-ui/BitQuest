/* main.c - Punto de entrada de BitQuest
 * Explorador de matrices: mapa 60x60, ventana visible 20x20, monedas,
 * llave, puerta y salida, con funciones obligatorias en NASM.
 */
#include <stdio.h>
#include "game.h"
#include "render.h"
#include "input.h"

static void pausa_continuar(void)
{
    printf("  Presiona cualquier tecla para continuar...\n");
    input_leer();
}

int main(void)
{
    Juego juego;
    int nivel = 0;
    int corriendo = 1;
    int total_monedas = 0;      /* monedas recogidas en todo el juego */
    int total_posibles = 0;     /* monedas que existian en todos los niveles */
    int total_pasos = 0;        /* pasos acumulados de todos los niveles */
    int niveles_completados = 0;

    render_init();
    input_init();

    if (!juego_cargar_nivel(&juego, nivel)) {
        input_fin();
        return 1;
    }

    while (corriendo) {
        render_juego(&juego);

        if (juego_ganado(&juego)) {
            /* Acumula los totales del nivel recien terminado */
            total_monedas  += juego.monedas;
            total_posibles += juego.monedas_total;
            total_pasos    += juego.pasos;
            niveles_completados++;

            render_mensaje_final(&juego);
            nivel++;
            if (nivel >= MAX_NIVELES) {
                pausa_continuar();
                render_resumen_total(total_monedas, total_posibles,
                                     total_pasos, niveles_completados);
                input_fin();        /* deja el resumen visible en pantalla */
                return 0;
            }
            pausa_continuar();
            if (!juego_cargar_nivel(&juego, nivel))
                break;
            continue;
        }

        switch (input_leer()) {
        case TECLA_ARRIBA: juego_mover(&juego,  0, -1); break;
        case TECLA_ABAJO:  juego_mover(&juego,  0,  1); break;
        case TECLA_IZQ:    juego_mover(&juego, -1,  0); break;
        case TECLA_DER:    juego_mover(&juego,  1,  0); break;
        case TECLA_SALIR:  corriendo = 0; break;
        default: break;
        }
    }

    input_fin();
    render_limpiar();
    printf("Saliste de BitQuest.\n");
    return 0;
}
