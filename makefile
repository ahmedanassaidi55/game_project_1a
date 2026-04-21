prog: main.o enemy.o
	gcc main.o enemy.o -o prog -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm -g
 
main.o: main.c enemy.h
	gcc -c main.c -g
 
enemy.o: enemy.c enemy.h
	gcc -c enemy.c -g
 
clean:
	rm -f *.o prog
 
