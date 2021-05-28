table.exe: testArbre.o p_arbre.o
	gcc -o table.exe testArbre.o p_arbre.o

testArbre.o: testArbre.c
	gcc -c testArbre.c

p_arbre.o: p_arbre.c p_arbre.h
	gcc -c p_arbre.c
clean:
	rm -f *.o
	rm -f *.exe
