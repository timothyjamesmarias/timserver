timserver: 
	gcc -ggdb -Wall -std=gnu99 -Wextra -pedantic timserver.c -o timserver

clean:
	rm *.o timserver
