ww:
	gcc -g -std=c99 -Wall -Werror -fsanitize=address,undefined ww.c -o ww

debug:
	gcc -g -std=c99 -Wall -Werror -fsanitize=address,undefined ww.c -DDEBUG -o ww

ww.o: ww.c
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined ww.c

clean:
	rm -f *.o ww
