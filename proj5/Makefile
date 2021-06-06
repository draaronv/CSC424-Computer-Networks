#
# Name: Aaron Valdes
# Date: 04/09/2021
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

SHELL= /bin/bash

COPTS=

# adjust these macro values accordingly.
# you may have to adjust PORT= to claim an available port
PORT= 12344
FILE_SPOEM=poem.txt
FILE_S=file-on-server.txt
FILE_S0=0bytes.bin
FILE_S1=1bytes.bin
FILE_S1_0=1bytezero.bin
FILE_S16=16bytezero.bin
FILE_S511=511bytes.bin
FILE_S512=512bytes.bin
FILE_S512_0=512byteszero.bin
FILE_S513=513bytes.bin
FILE_S513_0=513byteszero.bin
FILE_S1535=1535bytes.bin
FILE_S1536=1536bytes.bin
FILE_S1537=1537bytes.bin
FILE_C = file-on-client.txt
HOST = localhost
REMOTE_HOST = localhost

all: build clean

build:
	make ttftp

ttftp-server.o: ttftp-server.c ttftp.h
	cc -g ${COPTS} -c -o $@ $<

ttftp-client.o: ttftp-client.c ttftp.h
	cc -g ${COPTS} -c -o $@ $<

ttftp: ttftp.c ttftp-server.o ttftp-client.o ttftp.h
	cc -g ${COPTS} -o $@ $< ttftp-client.o ttftp-server.o 

run-server: ttftp
	./ttftp -L ${PORT}
	
run-client: ttftp
	./ttftp -h ${REMOTE_HOST} -f ${POEM} ${PORT} > ${FILE_C}
	diff ${POEM} ${FILE_C}

testAll: ttftp
	make testDate
	make testPoem
	make test0
	make test1
	make test2
	make test3
	make test4
	make test5
	make test6
	make test7
	make test8
	make test9
	make test10
	make test11

testDate: ttftp
	echo `date` >> ${FILE_S}
	./ttftp -L ${PORT} &
	./ttftp -h ${HOST} -f ${FILE_S} ${PORT} > ${FILE_C}
	diff ${FILE_S} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

testPoem: ttftp
	@echo Testing Poem
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_SPOEM} ${PORT} > ${FILE_C}
	@diff ${FILE_SPOEM} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test0: ttftp	
	@echo Testing 0 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S0} ${PORT} > ${FILE_C}
	@diff ${FILE_S0} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test1: ttftp
	@echo Testing 1 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S1} ${PORT} > ${FILE_C}
	@diff ${FILE_S1} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test2: ttftp
	@echo Testing 1 bytes with zeros
	@./ttftp -L ${PORT} & 
	@./ttftp -h ${HOST} -f ${FILE_S1_0} ${PORT} > ${FILE_C}
	@diff ${FILE_S1_0} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test3: ttftp
	@echo Testing 16 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S16} ${PORT} > ${FILE_C}
	@diff ${FILE_S16} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test4: ttftp
	@echo Testing 511 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S511} ${PORT} > ${FILE_C}
	@diff ${FILE_S511} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test5: ttftp
	@echo Testing 512 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S512} ${PORT} > ${FILE_C}
	@diff ${FILE_S512} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test6: ttftp
	@echo Testing 512 bytes with zeros
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S512_0} ${PORT} > ${FILE_C}
	@diff ${FILE_S512_0} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test7: ttftp
	@echo Testing 513 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S513} ${PORT} > ${FILE_C}
	@diff ${FILE_S513} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test8: ttftp
	@echo Testing 513 bytes with zeros
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S513_0} ${PORT} > ${FILE_C}
	@diff ${FILE_S513_0} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test9: ttftp
	@echo Testing 1535 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S1535} ${PORT} > ${FILE_C}
	@diff ${FILE_S1535} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test10: ttftp
	@echo Testing 1536 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S1536} ${PORT} > ${FILE_C}
	@diff ${FILE_S1536} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

test11: ttftp
	@echo Testing 1537 bytes
	@./ttftp -L ${PORT} &
	@./ttftp -h ${HOST} -f ${FILE_S1537} ${PORT} > ${FILE_C}
	@diff ${FILE_S1537} ${FILE_C}
	@echo "***\n*** ${GREEN}PASSED${NC}\n***"

tcpdump:
	sudo tcpdump -i lo -X  udp

clean:
	-rm ttftp ttftp-server.o ttftp-client.o ${FILE_S} ${FILE_C}

