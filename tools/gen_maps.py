#!/usr/bin/env python3
"""Genera los mapas 60x60 de BitQuest en maps/levelN.txt.

Leyenda: '#' pared, '.' piso, 'o' moneda, 'K' llave, 'D' puerta,
         'E' salida, '@' inicio del jugador.

Cada nivel garantiza que la llave, la puerta, la salida y todas las
monedas sean alcanzables: se construye sobre un piso abierto y los
muros interiores se colocan dejando siempre pasillos libres.
"""
import os
import random

W = H = 60


def base_grid():
    """Mapa con borde de pared y el interior de piso."""
    g = [[('#' if x in (0, W - 1) or y in (0, H - 1) else '.')
          for x in range(W)] for y in range(H)]
    return g


def add_walls(g, rng, density):
    """Coloca bloques de pared dejando los pasillos pares libres,
    asi el interior queda siempre conectado por filas/columnas pares."""
    for y in range(2, H - 2):
        for x in range(2, W - 2):
            if x % 2 == 1 and y % 2 == 1 and rng.random() < density:
                g[y][x] = '#'


def libres(g):
    return [(x, y) for y in range(1, H - 1) for x in range(1, W - 1)
            if g[y][x] == '.']


def construir(nivel, semilla, monedas, density):
    rng = random.Random(semilla)
    g = base_grid()
    add_walls(g, rng, density)

    celdas = libres(g)
    rng.shuffle(celdas)

    # Jugador en la esquina superior izquierda libre
    px, py = celdas.pop()
    g[py][px] = '@'

    # Salida lejos del jugador (esquina opuesta entre las celdas libres)
    celdas.sort(key=lambda c: (c[0] - px) ** 2 + (c[1] - py) ** 2)
    sx, sy = celdas.pop()           # la mas lejana
    g[sy][sx] = 'E'

    # Llave y puerta en celdas libres restantes
    rng.shuffle(celdas)
    kx, ky = celdas.pop(); g[ky][kx] = 'K'
    dx, dy = celdas.pop(); g[dy][dx] = 'D'

    # Monedas
    for _ in range(monedas):
        if not celdas:
            break
        cx, cy = celdas.pop()
        g[cy][cx] = 'o'

    return '\n'.join(''.join(row) for row in g) + '\n'


def main():
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    out = os.path.join(here, 'maps')
    os.makedirs(out, exist_ok=True)
    niveles = [
        # (semilla, monedas, densidad de muros)
        (1, 15, 0.25),
        (2, 25, 0.40),
        (3, 35, 0.55),
    ]
    for i, (s, m, d) in enumerate(niveles, start=1):
        txt = construir(i, s, m, d)
        path = os.path.join(out, f'level{i}.txt')
        with open(path, 'w') as f:
            f.write(txt)
        print(f'Generado {path}')


if __name__ == '__main__':
    main()
