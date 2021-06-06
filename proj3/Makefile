# author:Aaron Valdes
# makefile for packet passaround
# created: 3 feb 2019
# updated: 15 feb 2021
# author: bjr
#

PORT= 33031


# set these to the IP address for test
MC=172.31.10.23
MS=172.31.1.68
M1=172.31.10.23
M2=172.31.1.68
M3=172.31.12.219

LOC=localhost
LOC1=127.0.0.1
RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m

all:
	make passaround

passaround: passaround.c passaround.h 
	cc -g -o $@ $< 

#Local Test
test-local3: passaround test-ref-locals
	./passaround -n 3 -m localhost:localhost:localhost ${PORT} > test-local3.out
	diff test-local3.ref test-local3.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-local2: passaround test-ref-locals
	./passaround -n 2 -m localhost:localhost:localhost ${PORT} > test-local2.out
	diff test-local2.ref test-local2.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-local1: passaround test-ref-locals
	./passaround -n 1 -m localhost:localhost:localhost ${PORT} > test-local1.out	
	diff test-local1.ref test-local1.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-local4:passaround test-ref-locals
	./passaround -n 3 -m 127.0.0.1:127.0.0.1:127.0.0.1 ${PORT} > test-local4.out	
	diff test-local4.ref test-local4.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-local5:passaround test-ref-locals
	./passaround -n 5 -m 127.0.0.1:127.0.0.1:127.0.0.1:127.0.0.1:127.0.0.1 ${PORT} > test-local5.out	
	diff test-local5.ref test-local5.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-local6:passaround test-ref-locals
	./passaround -n 1 -m localhost ${PORT} > test-local6.out	
	diff test-local6.ref test-local6.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

#Testing the basic test
test:


test-s: passaround test-ref-basic
	@echo Run this on ${MS}
	@echo Then run make test-c on ${MC}
	./passaround -n 2 ${PORT} > test-s.out
	-diff test-s.ref test-s.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-c: passaround test-ref-basic
	@echo First run make test-s on ${MS}
	@echo The run this on ${MC}
	./passaround -n 3 -m ${MS}:${MC}:${MS}:${MC} ${PORT} > test-c.out
	-diff test-c.ref test-c.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"



#Testing 4 computers
test-s2: passaround test-ref-medium
	./passaround -n 2 ${PORT} > test-s2.out
	-diff test-s2.ref test-s2.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-s3: passaround test-ref-medium
	./passaround -n 2 ${PORT} > test-s3.out
	-diff test-s3.ref test-s3.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-c1: passaround test-ref-basic
	./passaround -n 2 -m ${M2}:${M3}:${M1}:${M2}:${M3} ${PORT} > test-c1.out
	-diff test-c1.ref test-c1.out
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test-ref-basic:
	@echo "S: ${MS}:${PORT} |${MC}:${MS}:${MC}|" > test-c.ref
	@echo "R: ${MS}:${PORT} |${MS}:${MC}|" >> test-c.ref
	@echo "S: ${MS}:${PORT} |${MC}|" >> test-c.ref
	@echo "R: ${MS}:${PORT} ||" >> test-c.ref
	@echo "R: ${MC}:${PORT} |${MC}:${MS}:${MC}|" > test-s.ref
	@echo "S: ${MC}:${PORT} |${MS}:${MC}|" >> test-s.ref
	@echo "R: ${MC}:${PORT} |${MC}|" >> test-s.ref
	@echo "S: ${MC}:${PORT} ||" >> test-s.ref

test-ref-medium:
	@echo "S: ${M2}:${PORT} |${M3}:${M1}:${M2}:${M3}|" > test-c1.ref
	@echo "R: ${M3}:${PORT} |${M2}:${M3}|" >> test-c1.ref
	@echo "S: ${M2}:${PORT} |${M3}|" >> test-c1.ref
	@echo "R: ${M1}:${PORT} |${M3}:${M1}:${M2}:${M3}|" > test-s2.ref
	@echo "S: ${M3}:${PORT} |${M1}:${M2}:${M3}|" >> test-s2.ref
	@echo "R: ${M1}:${PORT} |${M3}|" >> test-s2.ref
	@echo "S: ${M1}:${PORT} |${M3}|" >> test-s2.ref
	@echo "R: ${M2}:${PORT} |${M1}:${M2}:${M3}|" > test-s3.ref
	@echo "S: ${M1}:${PORT} |${M2}:${M3}|" >> test-s3.ref
	@echo "R: ${M2}:${PORT} ||" >> test-s3.ref

test-ref-locals:
	@echo "S: ${LOC1}:${PORT} |${LOC}:${LOC}|" > test-local1.ref
	@echo "S: ${LOC1}:${PORT} |${LOC}:${LOC}|" > test-local2.ref
	@echo "R: ${LOC1}:${PORT} |${LOC}:${LOC}|" >> test-local2.ref
	@echo "S: ${LOC1}:${PORT} |${LOC}|" >> test-local2.ref
	@echo "S: ${LOC1}:${PORT} |${LOC}:${LOC}|" > test-local3.ref
	@echo "R: ${LOC1}:${PORT} |${LOC}:${LOC}|" >> test-local3.ref
	@echo "S: ${LOC1}:${PORT} |${LOC}|" >> test-local3.ref
	@echo "R: ${LOC1}:${PORT} |${LOC}|" >> test-local3.ref
	@echo "S: ${LOC1}:${PORT} ||" >> test-local3.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}:${LOC1}|" > test-local4.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}:${LOC1}|" >> test-local4.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}|" >> test-local4.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}|" >> test-local4.ref
	@echo "S: ${LOC1}:${PORT} ||" >> test-local4.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}:${LOC1}:${LOC1}:${LOC1}|" > test-local5.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}:${LOC1}:${LOC1}:${LOC1}|" >> test-local5.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}:${LOC1}:${LOC1}|" >> test-local5.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}:${LOC1}:${LOC1}|" >> test-local5.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}:${LOC1}|" >> test-local5.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}:${LOC1}|" >> test-local5.ref
	@echo "S: ${LOC1}:${PORT} |${LOC1}|" >> test-local5.ref
	@echo "R: ${LOC1}:${PORT} |${LOC1}|" >> test-local5.ref
	@echo "S: ${LOC1}:${PORT} ||" >> test-local5.ref
	@echo "S: ${LOC1}:${PORT} ||" > test-local6.ref

clean:
	rm test-local1.out test-local2.out test-local3.out test-local4.out test-local5.out test-local6.out test-local1.ref test-local2.ref test-local3.ref test-local4.ref test-local5.ref test-local6.ref test-s.out test-c.out test-c.ref test-s.ref passaround test-local.ref test-local.out test-c1.ref test-c1.out test-s2.out test-s2.ref test-s3.ref test-s3.out
	
	
	
