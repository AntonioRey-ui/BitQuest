/* input.c - Lectura de teclado inmediata (sin Enter), Windows y Unix */
#include "input.h"

#ifdef _WIN32
/* ------------------------- Version Windows ------------------------- */
#include <conio.h>

void input_init(void) { }
void input_fin(void)  { }

int input_leer(void)
{
    int c = _getch();
    if (c == 0 || c == 0xE0) {       /* prefijo de teclas especiales */
        switch (_getch()) {
        case 72: return TECLA_ARRIBA;
        case 80: return TECLA_ABAJO;
        case 75: return TECLA_IZQ;
        case 77: return TECLA_DER;
        default: return TECLA_OTRA;
        }
    }
    switch (c) {
    case 'w': case 'W': return TECLA_ARRIBA;
    case 's': case 'S': return TECLA_ABAJO;
    case 'a': case 'A': return TECLA_IZQ;
    case 'd': case 'D': return TECLA_DER;
    case 'q': case 'Q': return TECLA_SALIR;
    default:            return TECLA_OTRA;
    }
}

#else
/* -------------------------- Version Unix --------------------------- */
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

static struct termios g_orig;   /* configuracion original de la terminal */

void input_init(void)
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &g_orig);
    raw = g_orig;
    raw.c_lflag &= ~(ICANON | ECHO);   /* sin buffer de linea ni eco */
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void input_fin(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &g_orig);
}

int input_leer(void)
{
    int c = getchar();
    if (c == 27) {                     /* posible secuencia de flecha: ESC [ X */
        if (getchar() == '[') {
            switch (getchar()) {
            case 'A': return TECLA_ARRIBA;
            case 'B': return TECLA_ABAJO;
            case 'D': return TECLA_IZQ;
            case 'C': return TECLA_DER;
            default:  return TECLA_OTRA;
            }
        }
        return TECLA_OTRA;
    }
    switch (c) {
    case 'w': case 'W': return TECLA_ARRIBA;
    case 's': case 'S': return TECLA_ABAJO;
    case 'a': case 'A': return TECLA_IZQ;
    case 'd': case 'D': return TECLA_DER;
    case 'q': case 'Q': return TECLA_SALIR;
    default:            return TECLA_OTRA;
    }
}

#endif
