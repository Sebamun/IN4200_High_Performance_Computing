CC = gcc
CFLAGS = -O2

LDFLAGS = -L/Users/sebastianamundsen/Desktop/IN4200/IN4200_High_Performane_Computing/Oblig_2/serial_code/simple-jpeg -lsimplejpeg

PROJ = serial_main

all: simple-jpeg $(PROJ)

serial_main: serial_main.c functions.c
	$(CC) $(CFLAGS) -o $@ serial_main.c functions.c $(LDFLAGS)

simple-jpeg:
	$(MAKE) -C ../../simple-jpeg
