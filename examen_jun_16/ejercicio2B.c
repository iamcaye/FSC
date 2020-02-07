#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>


#define TAM 512

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Uso: %s <FIFO>", argv[0]);
		exit(1);
	}
	
	char buffer[TAM];
	int escrito;
	int fd = open(argv[1], O_WRONLY);
	
	if(fd < 0){
		perror("open");
		exit(1);
	}
	
	int leido = read(0, buffer, TAM);
	buffer[leido-1] = '\0';
	
	while(strcmp(buffer,"fin") != 0){
		escrito = write(fd , buffer, leido);
		
		leido = read(0, buffer,TAM);
		buffer[leido] = '\0';
	}



	return 0;
}