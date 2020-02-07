#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

#define TAM 500
#define PUERTO 4950


ssize_t readn(int fd, void *buffer, ssize_t longitud);
ssize_t writen(int fd, void * buffer, ssize_t longitud);

int main(int argc, char *argv[]){
	/* Declaracion de vbles */
	int sd, n_sd;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	uint8_t longitud;

	/* Direccion de servidor */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PUERTO);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	/* Socket */
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Fallo en crear socket");
		exit(1);
	}
	
	/* Asociamos socket con el puerto */
	if(bind(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Fallo en asociar socket");
		exit(1);
	}
	
	/* Ponemos el socket en modo pasivo */
	if(listen(sd,5) < 0){
		perror("Fallo en listen server");
		exit(1);
	}
	
	uint16_t longBigE, dataBigE;
	int data, leidos;
	
	while(1){
		printf("Servidor esperando cliente...");
		socklen_t addr_len = sizeof(cli_addr);
		
		if((n_sd = accept(sd, (struct sockaddr *)&cli_addr, &addr_len)) < 0){
			perror("Fallo en accept de servidor");
			close(n_sd);
			close(sd);
			exit(1);
		}
		
		if(readn(n_sd, &longitud, sizeof(uint8_t)) < 0){
			perror("Fallo en read longitud");
			close(sd);
			close(n_sd);
			exit(1);
		}
		
		if(readn(n_sd, buffer, longitud) != longitud){
			perror("Fallo en envio del nfich");
			close(sd);
			close(n_sd);
			exit(1);
		}
	
		buffer[longitud] = '\0';
		
		int fd = open(buffer, O_WRONLY || O_CREAT || O_APPEND);
		if(fd < 0){
			perror("Fallo open archivo");
			close(sd);
			close(n_sd);
			exit(1);
		}
		
		int leidos = readn(n_sd, &dataBigE, sizeof(uint16_t));
		
		if(leidos < 0){
			perror("Fallo readn longBigE");
			close(n_sd);
			close(sd);
			close(fd);
		}
		longBigE = sizeof(dataBigE);
		
		while(leidos > 0 && leidos == longBigE){
		
			data = ntohs(dataBigE);
			
			int escrito = writen(fd, &data, longBigE);
			if(escrito < 0){
				perror("Fallo en writen fichero");
				close(sd);
				close(n_sd);
				close(fd);
			}
			
			escrito = writen(fd, "\n", 1);
			if(escrito < 0){
				perror("Fallo en barraN");
				close(sd);
				close(n_sd);
				close(fd);				
			}
			
			leidos = readn(n_sd, &dataBigE, sizeof(uint16_t));
			longBigE  = sizeof(dataBigE);
		}
		
		if(leidos < 0){
			perror("Fallo en readn");
			close(sd);
			close(n_sd);
			close(fd);
		}else{
			close(n_sd);
			close(fd);
		}
	}
	return 0;
}

ssize_t readn(int fd, void * buffer, ssize_t tam){
	int leidos = read(fd, buffer, tam);
	int total = 0;
	int porleer = tam;
	
	while(leidos > 0 && total < tam){
		total += leidos;
		porleer -= leidos;
		read(fd, buffer, porleer);
	}
	
	if(total > 0){
		return total;
	}else{
		return leidos;
	}
}

ssize_t writen(int fd, void * buffer, ssize_t tam){
	int escrito = writen(fd, buffer, tam);
	int total = 0;
	int a_escribir = tam;
	
	while(escrito > 0 && total < tam){
		total += escrito;
		a_escribir -= escrito;
		escrito = write(fd, buffer, a_escribir);
	}
	if(total > 0){
		return total;
	}else{
		return escrito;
	}

}