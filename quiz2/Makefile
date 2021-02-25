#
# makefile for quiz2 csc424 semester 212
# author: bjr
# last-update:
#	16 feb 2021
#

F= class-ip
A= 10.0.0.1
B= 172.16.0.1
C= 192.168.0.1 
D= 255.255.255.255
B0= 172.16.1.1 
B1= 172.17.0.1
C0= 192.168.0.2
C1= 192.168.1.1



all: $F
	make test
	make clean

$F: $F.c
	cc -o $@ $<

test: $F
	./$F $A $B
	./$F $C $D
	./$F $B ${B0}
	./$F $B ${B1}
	./$F $C ${C0}
	./$F $C ${C1}

clean:
	-rm $F

