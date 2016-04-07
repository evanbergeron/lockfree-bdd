# call with make D='-DDEBUG -g' for debug mode
FLAGS=-Wall -Wextra -std=c99 -pedantic -Werror
# D=-DDEBUG -g
GCC=gcc ${D} ${FLAGS}

default: nqueens

hashtable : include/contracts.h include/xalloc.h lib/xalloc.c include/hashtable.h lib/hashtable.c test/hashtable-test.c
	$(GCC) lib/xalloc.c lib/hashtable.c test/hashtable-test.c -o bin/hashtable

robdd : include/contracts.h include/xalloc.h lib/xalloc.c include/hashtable.h lib/hashtable.c include/robdd.h src/robdd.c test/robdd-test.c
	$(GCC) lib/xalloc.c lib/hashtable.c src/robdd.c test/robdd-test.c -o bin/robdd

nqueens : include/contracts.h include/xalloc.h lib/xalloc.c include/hashtable.h lib/hashtable.c include/robdd.h src/robdd.c test/nqueens.c
	$(GCC) lib/xalloc.c lib/hashtable.c src/robdd.c test/nqueens.c -o bin/nqueens

clean :
	${RM} -rf bin/a.out a.out.dSYM
	${RM} -rf bin/hashtable hashtable.dSYM
	${RM} -rf bin/robdd robdd.dSYM
	${RM} -rf bin/nqueens nqueens.dSYM
