
default:
	gcc -g -Wall -Werror -fsanitize=address part1.c -o part1

debug:
	gcc -g -Wall -Werror -fsanitize=address part1.c -DDEBUG -o part1

demo.o: demo.c part1.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined demo.c

part1.o: part1.c part1.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined part1.c


# separate version of part1.o with debugging text enabled
dpart1.o: part1.c part1.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -DDEBUG -o dpart1.o part1.c

ddemo: demo.o dpart1.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o demo demo.o dpart1.o

clean:
	rm -f *.o demo ddemo
