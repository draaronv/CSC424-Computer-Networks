#
# makefile for netbounce example
# Jan 12,2016
# bjr
#

CC = gcc
MACHINE = localhost
PORT = 3333

all:
	make netbounce

netbounce: netbounce-server netbounce-client

netbounce-server: netbounce-server.c 
	${CC} -o $@ $<
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<
	
test:
	@echo In one window, make run-server
	@echo Then, in another window, make run-client
	
run-server: netbounce-server
	@echo Ready to bounce!
	./netbounce-server -lv -p ${PORT}

run-client: netbounce-client
	./netbounce-client -h ${MACHINE} -p ${PORT} -v "The amazing net bounce!"

clean:
	-rm netbounce-client netbounce-server

