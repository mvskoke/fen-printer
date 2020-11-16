CFLAGS = -Wall -Wextra -g
CC = gcc

fen-printer: fen-printer.c
	${CC} fen-printer.c ${CFLAGS} -o fen-printer
