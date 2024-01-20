CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c state_machine.c        # Nombre del archivo fuente
#HEADERS = state_machine.h  # Nombres de los archivos de encabezado
OBJ = $(SRC:.c=.o)

all: exe

exe: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) myprogram
