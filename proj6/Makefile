#
# test makefile for csc424.192 proj4
# author: bjr
# date: 13 may 2017
# last update: 12 may 2020
#

COPTS= -g -w

PASSFILE= mypasswords.txt
SERVER= localhost
#SERVER= 52.91.116.86
SHELL= /bin/bash

PORT= 1812

RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m

build:
	make mradius

mradius: mradius.c mradius.h utils.o mradius-client.o mradius-server.o
	cc ${COPTS} -o $@ $< utils.o mradius-client.o mradius-server.o -lcrypto

mradius-client.o: mradius-client.c mradius.h
	cc ${COPTS} -c -o $@ $< 
	
mradius-server.o: mradius-server.c mradius.h
	cc ${COPTS} -c -o $@ $< 

utils.o: utils.c mradius.h
	cc ${COPTS} -c -o $@ $< 

run-server-nr:
	./mradius -vLR ${PASSFILE}
	./mradius -vLR ${PASSFILE}
	
run-server:
	./mradius -v ${PASSFILE}
	
run-client-nr:
	./mradius -vR -h ${SERVER} pikachu LaaOVTXLgtk0
	sleep 1
	./mradius -vR -h ${SERVER} pikachu wrong
	sleep 1
	./mradius -vR -h ${SERVER} nonexistant none

run-client-v:
	./mradius -v -h ${SERVER} pikachu LaaOVTXLgtk0
	sleep 1
	./mradius -v -h ${SERVER} pikachu wrong
	sleep 1
	./mradius -v -h ${SERVER} nonexistant none

run-client-x:
	./mradius -h ${SERVER} pikachu LaaOVTXLgtk0
	sleep 1
	./mradius -h ${SERVER} pikachu wrong
	sleep 1
	./mradius -h ${SERVER} wonderwoman refs385tike
	sleep 1
	./mradius -h ${SERVER} wonderwoman wrong
	sleep 1
	./mradius -h ${SERVER} nonexistant none
	sleep 1
	./mradius -h ${SERVER} nameonly

run-client:
	( make run-client-x ) | grep "YES\|NO" &> basic-test.out
	diff basic-test.ref basic-test.out
	@echo -e "***\n*** ${GREEN}passed the basic test${NC}\n***"

making-basic-ref:
	@echo make run-server 
	( make run-client-x ) | grep "YES\|NO" &> basic-test.ref

# this is used to install the open ssl libraries; only needed once after an install
# use this if for instance, cc cannot find openssl/md5.h 
install-openssl:
	sudo apt-get install libssl-dev

tcpdump:
	@echo "generally run in own window"
	sudo tcpdump -i lo -lX port ${PORT} or portrange 10000-65535

	
clean:
	-rm mradius utils.o mradius-client.o mradius-server.o
	-rm basic-test.out



