# Makefile for ush
#
# Vincent W. Freeh
#
# $Id: Makefile,v 1.1.1.1 2004/10/20 16:03:56 vin Exp $
# 

CC=gcc
CFLAGS=-g
SRC=main.c parse.c parse.h
OBJ=main.o parse.o

ush:	$(OBJ)
	$(CC) -o $@ $(OBJ)

tar:
	tar czvf ush.tar.gz $(SRC) Makefile README

clean:
	\rm $(OBJ) ush
