#
# makefile author: bjr
# last-update:
#	11 feb 2021: (bjr) updated for simple-datagram
#

HOST= localhost
MESSAGE= "helloworld!"
MESSAGE2="\x07\x00\x01\x48\x6a\x20\x30\x31\x32"
PORT = 3333

RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m

all:
	make simple-datagram

simple-datagram: simple-datagram.c simple-data-c.o simple-data-s.o simple-data.h
	cc -o $@ $< simple-data-c.o simple-data-s.o

simple-data-s.o: simple-data-s.c simple-data.h
	cc -c $< 

simple-data-c.o: simple-data-c.c simple-data.h
	cc -c $< 

run-server: simple-datagram
	./simple-datagram -p ${PORT} > testserver.txt
	
run-client: simple-datagram
	echo ${MESSAGE} | ./simple-datagram -p ${PORT} ${HOST}

test: simple-datagram
	echo ${MESSAGE} > test.ref
	./simple-datagram -p ${PORT} > test.out &
	echo  ${MESSAGE} | ./simple-datagram -p ${PORT} ${HOST}
	diff test.out test.ref
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test2:simple-datagram
	./simple-datagram -p 12345 > test.out &
	echo ${MESSAGE} | ./simple-datagram -p 12345 127.0.0.1
	diff test.out test.ref
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test3:simple-datagram
	echo ${MESSAGE2} > test.ref
	./simple-datagram -p ${PORT} > test.out &
	echo ${MESSAGE2} | ./simple-datagram -p ${PORT} ${HOST}
	diff test.out test.ref
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

testverbose: simple-datagram
	./simple-datagram -v -p ${PORT} &
	echo ${MESSAGE} | ./simple-datagram -v -p ${PORT} ${HOST}

test4:simple-datagram tcpdump
	echo ${MESSAGE2} > test.ref
	./simple-datagram -p ${PORT} > test.out &
	echo ${MESSAGE2} | ./simple-datagram -p ${PORT} ${HOST}
	diff test.out test.ref
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

tcpdump:
	sudo tcpdump -i lo -X udp > tcpdump.out & 

clean:
	-rm simple-data-c.o simple-data-s.o simple-datagram test.out test.ref tcpdump.out


