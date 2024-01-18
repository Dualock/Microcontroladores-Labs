CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = state_machine.c          # Nombre del archivo fuente
HEADERS = state_machine.h   # Nombre del archivo de encabezado
OBJ = $(SRC:.c=.o)

all: myprogram

myprogram: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) myprogram
