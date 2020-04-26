default:
	gcc main.c -o ./bin/main

debug:
	gcc main.c -DDEBUG -o ./bin/main

clean:
	rm -f ./bin/main