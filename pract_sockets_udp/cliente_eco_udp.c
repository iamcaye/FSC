#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> 
#include <stdint.h>

#define PUERTO 4950 /* PUERTO DE CONEXION */
#define TAMA 100 /* Tamaño de buffer */

const char IP_SERVIDOR[] = "127.0.0.1" /* IP por defecto */

int main(int argc, char *argv[]){

	int sd; /* Descriptor de SOCKET */
	int leidos, num_bytes;
	
	struct sockaddr_in serv_addr; /* Direccion del Servidor */
	uint32_t ip_servidor; /* IP del servidor */
	socklen_t serv_len; /* Longitud de la direccion del servidor */
	
	char buffer[TAMA];	/* Array de bytes */
	char ip_serv[20];	/* Direccion ip de servidor */
	
	/* OBTENCION DE LA IP SERVIDOR */
	if(argc <= 1){
		/* No se ha introducido ninguna ip por argumento, asi que usamos la de por defecto */
		ip_servidor = inet_addr(IP_SERVIDOR):
		
	}else{
		/* Usamos la ip que se introduce por argumento */
		ip_servidor = inet_addr(argv[1]);
	
	}
	
	/* CHEQUEO DE ERRORES EN LA IP */
	if(ip_servidor < 0){
		perror("Fallo en la asignacion de ip_servidor");
		exit(-1);
	}
	
	/* Creamos el socket UDP */
	sd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sd < 0){
		perror("Fallo en la creacion del socket udp");
		exit(-1);
	}
	
	/* Inicializar la estructura con la direccion del servidor */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET

	return 0;
}