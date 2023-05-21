timserver: 
	gcc -ggdb -Wall -std=gnu99 -Wextra -pedantic -pthread timserver.c helpers.c -o timserver

clean:
	rm timserver
