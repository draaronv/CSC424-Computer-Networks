#Author: Aaron Valdes
#Title:Proj 0 makefile
# last update:01 February 2021: makefile created

C=gcc
H=hello

$H:$H.c
	$C $H.c -o $H.out

test:$H
	./$H.out

clean:
	rm *.out

all: test clean

