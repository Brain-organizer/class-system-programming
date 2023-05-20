.PHONY: clean

RM = rm -f

rls: rls.o
	gcc -o rls rls.o
rls.o: rls.c
	gcc -g -c rls.c
rlsd: rlsd.o
	gcc -o rlsd rlsd.o
rlsd.o: rlsd.c
	gcc -g -c rlsd.c

clean:
	$(RM) *.o