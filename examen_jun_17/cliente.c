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

/*ssize_t writen(int fd, void * buffer, ssize_t tam);
ssize_t readn(int fd, void * buffer, ssize_t tam);*/

int main(int argc, char * argv[]){

	int sd;
	struct sockaddr_in serv_addr;
	socklen_t serv_len;
	uint16_t longitud;
	char buffer[TAM];
	
	/* Montamos la direccion de servidor */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PUERTO);
	uint32_t ip_servidor;
	ip_servidor = inet_addr("127.0.0.1");
	if(ip_servidor < 0){
		perror("Error al convertir la ip");
		exit(1);
	}
	
	memcpy(&serv_addr.sin_addr, &ip_servidor, sizeof(ip_servidor));
	
	/* Creamos el socket */
	sd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sd < 0){
		perror("Fallo en crear socket");
		exit(1);
	}
	printf("socket\n");
	
	/* Conectamos con el servidor */
	if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Fallo abriendo conexion con el servidor");
		close(sd);
		exit(1);
			
	}
	printf("connect\n");
	
	char mensaje[TAM];
	
	/*int escrito = read(0, mensaje, TAM);
	mensaje[escrito-1] = '\0'
	//printf("%s\n", mensaje);
	longitud = strlen(mensaje);
	
	if(writen(sd, &longitud, sizeof(longitud)) != sizeof(longitud)){
		perror("Fallo en envio de longitud");
		close(sd);
		exit(1);
	}
	printf("1\n");
	if(writen(sd, mensaje, longitud)  != longitud){
		perror("Fallo en envio de cadena");
		close(sd);
		exit(1);
	}
	printf("2\n");*/
	
	
	int escrito = read(0, mensaje, TAM);
	mensaje[escrito-1] = '\0';
	
	while(strcmp("fin", mensaje) != 0){
			//printf("%s\n", mensaje);
			
			if(writen(sd, &longitud, sizeof(longitud)) != sizeof(longitud)){
				perror("Fallo en envio de longitud");
				close(sd);
				exit(1);
			}
			printf("1\n");
			if(writen(sd, mensaje, &longitud)  != longitud){
				perror("Fallo en envio de cadena");
				close(sd);
				exit(1);
			}
			printf("2\n");
			int escrito = read(0, mensaje, TAM);
			mensaje[escrito-1] = '\0';
	}

	
	return 0;
}


ssize_t writen(int fd, void * mensaje, size_t longitud_mensaje) {
  ssize_t a_escribir = longitud_mensaje;
  ssize_t total_escrito = 0;
  ssize_t escrito;
  
  do {
    errno = 0;
    escrito = write(fd, mensaje + total_escrito, a_escribir);
    if (escrito >= 0) {
      total_escrito += escrito ;
      a_escribir -= escrito ;
    }
  } while(
        ((escrito > 0) && (total_escrito < longitud_mensaje)) ||
        (errno == EINTR));
  
  if (total_escrito > 0) {
    return total_escrito;
  } else {
    /* Para permitir que devuelva un posible error de la llamada a write */
    return escrito;
  }
}

