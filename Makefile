# Makefile - Compila BitQuest en macOS / Linux (64 bits)
#   make          -> compila ./bitquest
#   make run      -> compila y ejecuta
#   make clean    -> borra binarios y objetos
#
# NASM genera codigo x86-64. En Macs con Apple Silicon (arm64) el binario
# se compila para x86_64 y se ejecuta mediante Rosetta 2.

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

CC      := clang
CFLAGS  := -O2 -Wall -Isrc

ifeq ($(UNAME_S),Darwin)
    NASM_FMT := macho64
    NASM_PREFIX := --prefix _
    ARCH := -arch x86_64
else
    NASM_FMT := elf64
    NASM_PREFIX :=
    ARCH :=
endif

CFLAGS += $(ARCH)

OBJ := main.o game.o render.o input.o funciones.o

all: bitquest

bitquest: $(OBJ)
	$(CC) $(ARCH) $(OBJ) -o bitquest

main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@
game.o: src/game.c
	$(CC) $(CFLAGS) -c $< -o $@
render.o: src/render.c
	$(CC) $(CFLAGS) -c $< -o $@
input.o: src/input.c
	$(CC) $(CFLAGS) -c $< -o $@

funciones.o: asm/funciones.asm
	nasm -f $(NASM_FMT) $(NASM_PREFIX) $< -o $@

run: bitquest
	./bitquest

clean:
	rm -f $(OBJ) bitquest

.PHONY: all run clean
