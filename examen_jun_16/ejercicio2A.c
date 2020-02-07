#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TAM 512

int max(int n1, int n2){
	if(n1 > n2){
		return n1;
	}else{
		return n2;
	}
}

int main(int argc, char *argv[]){
	
	int fd1, fd2, fich, leidos, escrito;
	fd_set conj_desc, conj_copia;
	char buffer[TAM];
	
	if(fd1 = open("logs_fifo1", O_RDONLY) < 0){
		perror("openfd1");
		exit(1);
	}
	
	if(fd2 = open("logs_fifo2", O_RDONLY) < 0){
		perror("openfd2");
		exit(1);
	}
	fich = creat("/home/alumno/Escritorio/FSC/examen_jun_16/logs", 0666);
	

	
	FD_ZERO(&conj_desc);
	FD_SET(fd1, &conj_desc);
	FD_SET(fd2, &conj_desc);
	
	while(1){
		memcpy(&conj_copia, &conj_desc, sizeof(fd_set));
		
		printf("alo?\n");
		int r = select(max(fd1,fd2)+1, &conj_copia, NULL, NULL, NULL);
		if(r > 0){
			if(FD_ISSET(fd1, &conj_copia)){
				leidos = read(fd1, buffer, TAM);
				if(leidos < 0){
					perror("read fd1");
					exit(1);
				}
				if(write(fich, "Cliente1: ", 11) != 11){
					perror("Fallo1");
					exit(1);
				}
				escrito = write(fich, buffer, leidos);
				if(escrito != leidos){
					perror("write fich 1");
					exit(1);
				}
				
				printf("%s",buffer);
				
				write(fich, "\n", 1);
				
				
			}
			
			if(FD_ISSET(fd2, &conj_copia)){
				leidos = read(fd2, buffer, TAM);
				if(leidos < 0){
					perror("read fd2");
					exit(1);
				}
				if(write(fich, "Cliente2: ", 11) != 11){
					perror("Fallo2");
					exit(1);
				}
				escrito = write(fich, buffer, leidos);
				if(escrito != leidos){
					perror("write fich 2");
					exit(1);
				}	
				write(fich, "\n", 1);			
			}
			
		}else if(r < 0){
			perror("select");
			exit(1);
			
		}else{
			exit(1);
		}
	}
	
	 
	
	
	
	return 0;
}