default:
	gcc main.c -o ./bin/main

debug:
	gcc main.c -DDEBUG -o ./bin/main

clean:
	sudo dmesg --clear
	rm -f ./bin/main