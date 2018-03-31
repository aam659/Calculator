MOBJS = integer.o real.o string.o da.o cda.o stack.o queue.o bst.o scanner.o process.o matilda.o -lm
OOPTS = -Wall -Wextra -std=c99 -c -g
EOPTS = -Wall -Wextra -std=c99 -g
VOPTS = -v
IOPTS = -i input
IPOPTS = -i -p items
IPBOPTS = -i -p -b testfile
IPBVOPTS = -i -p -b -v declare
PBIMOPTS = -p -b -i mytestfile
BIPSOPTS = -b -i -p sample
BIPEOPTS = -b -i -p exp
CRAZY = -b -i -p crazy
PI = -b -i -p -v division
MODULUS = -b -i -p -v modulus
CRAZIER = -b -i -p -v crazier
ZERO = -b -i -p -v zero
GARBAGE = -b -i -p -v garbage
MOREGARBAGE = -b -i -p -v moregarbage
NEGATIVES = -b -p -i -v negatives

matilda : $(MOBJS)
	gcc $(EOPTS) -o matilda $(MOBJS)

da.o : da.c da.h
	gcc $(OOPTS) da.c

cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c

stack.o : stack.c stack.h da.h
	gcc $(OOPTS) stack.c

queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

process.o : process.c process.h integer.h real.h string.h da.h cda.h stack.h queue.h bst.h scanner.h
	gcc $(OOPTS) process.c

matilda.o : matilda.c integer.h real.h string.h da.h cda.h stack.h queue.h bst.h scanner.h process.h
	gcc $(OOPTS) matilda.c

test :
	@echo running: matilda -v
	./matilda $(VOPTS)
	@echo
	@echo running: matilda -i input
	./matilda $(IOPTS)
	@echo
	@echo running: matilda -i -p items
	./matilda $(IPOPTS)
	@echo
	@echo running: matilda -i -p -b testfile
	./matilda $(IPBOPTS)
	@echo
	@echo running: matilda -i -p -b -v declare
	./matilda $(IPBVOPTS)
	@echo
	@echo running: matilda -p -b -i mytestfile
	./matilda $(PBIMOPTS)
	@echo
	@echo running: matilda -b -i -p sample
	./matilda $(BIPSOPTS)
	@echo
	@echo running: matilda -b -i -p exp
	./matilda $(BIPEOPTS)
	@echo
	@echo running: matilda -b -i -p crazy
	./matilda $(CRAZY)
	@echo
	@echo running: matilda -b -i -p -v division
	./matilda $(PI)
	@echo
	@echo running: matilda -b -i -p -v modulus
	./matilda $(MODULUS)
	@echo
	@echo running: matilda -b -i -p -v crazier
	./matilda $(CRAZIER)
	@echo
	@echo running: matilda -b -i -p -v zero
	./matilda $(ZERO)
	@echo
	@echo running: matilda -b -i -p -v garbage
	./matilda $(GARBAGE)
	@echo
	@echo running: matilda -b -i -p -v moregarbage
	./matilda $(MOREGARBAGE)
	@echo
	@echo running: matilda -b -p -i -v negatives
	./matilda $(NEGATIVES)
	@echo

clean   :
	rm -f *.o matilda


