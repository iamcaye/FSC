/* INCLUDES */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

/* CONSTANTES */
#define PUERTO 4950
#define TAM 512

ssize_t readn(int fd, void * buffer, ssize_t tam);
ssize_t writen(int fd, void * buffer, ssize_t tam);

int main(int argc, char argv[]){
	if(argc != 7){
		printf("Uso: %s <nombre1> <IP1> <nombre2> <IP2> <nombre3> <IP3>\n", argv[0]);
		exit(1);
	}

	int sd, n_sd, leidos;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	uint16_t longitud;
	char buffer[TAM];
	
	/* Declaramos la direccion del server */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PUERTO);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	/* Creamos el socket */
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Fallo en crear socket servidor");
		exit(1);
	}
	printf("socket\n");
	
	/* Vinculamos el socket con el servidor */
	if(bind(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Fallo en bind servidpr");
		close(sd);
		exit(1);
	}
	printf("bind\n");
	
	/* Ponemos el servidor en espera */
	if(listen(sd,5) < 0){
		perror("Fallo en listen servidor");
		close(sd);
		exit(1);
	}
	printf("listen\n");
	
	socklen_t cli_len = sizeof(cli_addr);
	
	while(1){
		printf("blalflda\n");
		n_sd = accept(sd, (struct sockaddr *)&cli_addr, &cli_len);
		if(n_sd < 0){
			perror("Fallo accept servidor");
		}
		printf("accept\n");
		
		if(leidos = readn(n_sd, &longitud, sizeof(longitud)) != sizeof(longitud)){
			perror("Fallo en recepcion de longitud de mensaje");
			close(n_sd);
			close(sd);
			exit(1);
		}
		
		printf("%d\n", longitud);
		
		if(readn(n_sd, buffer, (ssize_t)&longitud) != longitud){
			perror("Fallo en lectura del dominio");
			close(n_sd);
			close(sd);
			exit(1);
		}
		
		printf("%s", buffer);
		
		while(leidos > 0){
			unsigned i = 1;
			while(i < 6 && (strcmp(buffer, &argv[i]) != 0) ){
				i += 2;
			}
		
			if(i < 6){
				longitud = sizeof(argv[i+1]);
				if(writen(n_sd, &longitud, sizeof(longitud)) < 0){
					perror("Fallo en envio de long al cliente");
					close(n_sd);
					close(sd);
					exit(1);
				}
			
				if(writen(n_sd, &argv[i+1], longitud) != longitud){
					perror("Fallo en el envio de la ip del dominio");
					close(sd);
					close(n_sd);
					exit(1);
				}
			}else{
				longitud = strlen("0.0.0.0");
				if(writen(n_sd, &longitud, sizeof(longitud)) < 0){
					perror("Fallo en envio de long al cliente");
					close(n_sd);
					close(sd);
					exit(1);
				}
			
				if(writen(n_sd, "0.0.0.0", longitud) != longitud){
					perror("Fallo en el envio de la ip del dominio");
					close(sd);
					close(n_sd);
					exit(1);
				}
			
			}
		}
		if(leidos < 0){
			perror("Fallo en read del servidor");
			close(sd);
			close(n_sd);
			exit(1);
		}else{
		
			close(n_sd);
		}
		
		
	}
	
	return 0;
	
}

ssize_t writen(int fd, void * buffer, ssize_t tam){
	int porleer = tam;
	int leido = 0;
	int total = 0;
	
	leido = read(fd, buffer, tam);
	
	while(leido > 0 && total < tam){
		total += leido;
		porleer -= leido;
		
		leido = read(fd, buffer, porleer);
	}
	
	if(total > 0){
		return total;
	}else{
		return leido;
	}
}

ssize_t readn(int fd, void * buffer, ssize_t tam){
	int aescribir = tam;
	int escrito = 0;
	int total = 0;
	
	escrito = write(fd, buffer, tam);
	
	while(escrito > 0 && total < tam){
		total += escrito;
		aescribir -= escrito;
		
		escrito = write(fd, buffer, aescribir);
	}
	
	if(total > 0){
		return total;
	}else{
		return escrito;
	}
}