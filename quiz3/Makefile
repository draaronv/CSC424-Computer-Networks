#
# quiz3, csc424-212
# bjr
# 16 march 2021
# last-update:
#
#

F= sign-me-up
G= short-pack

RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m

all:
	make run-sign-me-up
	make test-short-pack

$F: $F.c
	cc -o $@ $<

$G: $G.c
	cc -o $@ $<
	
run-sign-me-up: $F
	@echo 
	./$F
	@echo "*** ${RED} SHOUlD BE 255 NOT -1  ${NC}***"
	@echo


test-short-pack: short-pack
	./short-pack 3 4 > short-pack.out
	./short-pack 70 90 >> short-pack.out
	./short-pack 455 544 >> short-pack.out
	diff short-pack.ref short-pack.out
	@echo "*** ${GREEN}PASSED the test ${NC}***"
	@echo
clean:
	-rm $F $G short-pack.out

