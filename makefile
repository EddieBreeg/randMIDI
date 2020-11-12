output: src/main.c src/utils.c include/utils.h
	gcc -Wall -I include src/main.c src/utils.c -o randMIDI