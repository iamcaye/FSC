
#include <stdint.h>
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
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#define TAM 500
#define PUERTO 4950

int sd;
struct sysinfo si;
uint16_t datoBigE;

ssize_t readn(int fd, void *buffer, ssize_t longitud);
ssize_t writen(int fd, void * buffer, ssize_t longitud);

void terminar(int r){
	close(sd);
	exit(1);
}

void timeout(int r){
	sysinfo(&si);
	
	int dato = si.procs;
	printf("enviando procs = %d\n", dato);

	datoBigE = htons(si.procs);
		
	if  (writen(sd, &datoBigE, sizeof(datoBigE)) != sizeof(datoBigE)) {
		perror("write longitud");
		close(sd);
		exit(1);
	}
	signal(SIGALRM, timeout);
}

int main(int argc, char * argv[]){
	/* SEÑALES */
	signal(SIGINT, terminar);
	signal(SIGALRM, timeout);


	/* Comprobamos los argumentos de entrada (IP + NFICH) */
	if(argc < 3){
		printf("Uso: %s <IP> <FICHERO>\n", argv[0]);
		exit(1);
	}
	
	int sd;
	struct sockaddr_in serv_addr;
	uint8_t longitud;
	char buffer[TAM];
	
	/* Direccion del Servidor */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_port = htons(PUERTO);
	serv_addr.sin_family = AF_INET;
	
	uint32_t ip_servidor = inet(argv[1]);
	
	if(ip_servidor < 0){
		perror("Fallo en asignacion de ip");
		exit(1);
	}
		
	memcpy(&serv_addr.sin_addr, &ip_servidor, sizeof(ip_servidor));
	
	/* Creamos el socket */
	sd = socket(PF_INET, SOCK_STREAM, 0);
	
	if(sd < 0){
		perror("Fallo en socket");
		exit(1);
	}
	
	//Conectamos con el servidor
	if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Fallo en conexion al servidor");
		close(sd);
		exit(1);
	}
	
	/* Enviamos el nombre del fichero */
	longitud = strlen(argv[2]);
	
	if(writen(sd, &longitud, sizeof(longitud)) < 0){
		close(sd);
		exit(1);
	}
	
	if(writen(sd, argv[2], longitud) < 0){
		close(sd);
		exit(1);
	}
	
	/* Establecemos el temporizador */
	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec = 5000;
	
	struct itimerval timer;
	timer.it_value = t;
	timer.it_interval = t;
	
	setitimer(ITIMER_REAL, &timer, 0);
	
	/* Y LA MAGIA */
	while(1){
		pause();
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