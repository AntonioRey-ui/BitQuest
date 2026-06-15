/* render.h - Dibujado de la ventana visible y la interfaz */
#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_init(void);              /* prepara la consola (VT en Windows) */
void render_limpiar(void);           /* limpia la pantalla */
void render_juego(const Juego *j);   /* dibuja ventana 20x20 + HUD */
void render_mensaje_final(const Juego *j); /* pantalla de fin de nivel */

/* Resumen del juego completo (totales de todos los niveles) */
void render_resumen_total(int monedas, int monedas_total, int pasos, int niveles);

#endif /* RENDER_H */
