# BitQuest — Explorador de Matrices (C + NASM)

Videojuego de consola que recorre un mapa tipo matriz de **60×60** a través de
una **ventana visible de 20×20**. El jugador recoge **monedas**, busca una
**llave** para abrir una **puerta** y llega a la **salida** para avanzar de
nivel. Varias funciones del juego están implementadas en **ensamblador NASM de
64 bits**.

## Cómo jugar

| Tecla            | Acción            |
|------------------|-------------------|
| `W` / `↑`        | Mover arriba      |
| `S` / `↓`        | Mover abajo       |
| `A` / `←`        | Mover izquierda   |
| `D` / `→`        | Mover derecha     |
| `Q`              | Salir             |

Símbolos en pantalla: `@` jugador · `#` pared · `o` moneda · `K` llave ·
`D` puerta · `E` salida.

## Estructura del proyecto

```
ProyectoEnsamblador/
├── src/            Código C (.c y .h)
│   ├── main.c      Bucle principal del juego
│   ├── game.c/.h   Carga de mapas, movimiento y reglas
│   ├── render.c/.h Dibujado de la ventana 20×20 y el HUD
│   ├── input.c/.h  Lectura de teclado (Windows y Unix)
│   └── asm_funcs.h Prototipos de las funciones en ensamblador
├── asm/
│   └── funciones.asm  Funciones obligatorias en NASM (win64 y macho64/elf64)
├── maps/           Mapas de los niveles (level1.txt … level3.txt)
├── build.bat       Compilación en Windows
├── Makefile        Compilación en macOS / Linux
└── README.md
```

## Funciones implementadas en NASM (`asm/funciones.asm`)

| Función       | Descripción                                              |
|---------------|----------------------------------------------------------|
| `asm_contar`  | Cuenta las monedas (bytes objetivo) en el mapa.          |
| `asm_celda`   | Devuelve el caracter del mapa en una coordenada `(x,y)`. |
| `asm_indice`  | Calcula el índice lineal `y*ancho + x`.                  |
| `asm_clamp`   | Limita la cámara a los bordes del mapa (ventana visible).|

El mismo archivo `.asm` se ensambla en Windows (`win64`) y en macOS/Linux
(`macho64`/`elf64`); detecta la convención de llamada automáticamente con la
macro `__OUTPUT_FORMAT__` de NASM.

## Compilación y ejecución

### Windows

Requisitos: **NASM** y **GCC de 64 bits** (MinGW-w64 / MSYS2) en el `PATH`.

```bat
build.bat            REM compila -> bitquest.exe
build.bat run        REM compila y ejecuta
build.bat clean      REM borra los archivos generados
```

### macOS / Linux

Requisitos: **NASM** y **clang** (o gcc). En macOS: `brew install nasm`.

```sh
make            # compila -> ./bitquest
make run        # compila y ejecuta
make clean      # limpia
```

> En Macs con Apple Silicon el binario se compila para `x86_64` (NASM genera
> código x86-64) y se ejecuta mediante Rosetta 2.
