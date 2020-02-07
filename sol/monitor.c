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
#include <sys/sysinfo.h>
#include <signal.h>
#include <sys/time.h>



#define PUERTO 4950
#define MAXBUFFER 512

//descriptor de socket 
int sd;
struct sysinfo si;
uint16_t datoBigEndian;
	

ssize_t read_n(int fd, void * mensaje, size_t longitud_mensaje);
ssize_t write_n(int fd, void * mensaje, size_t longitud_mensaje);
int trimming(char * cad);


void timeout(int signo) {
	sysinfo(&si);
	short dato = si.procs;
	printf("enviando procs = %d\n", dato);

	datoBigEndian = htons(si.procs);
		
	if  (write_n(sd, &datoBigEndian, sizeof(datoBigEndian)) != sizeof(datoBigEndian)) {
		perror("write longitud");
		close(sd);
		exit(1);
	}
	signal(SIGALRM, timeout);
}

void terminar(int signo) {
	if (close(sd) < 0) {
		perror("close");
		exit(-1);
	}
	exit(0);
}


int main(int argc, char * argv[]) {
	//0.- manejadores de señal
	signal(SIGALRM, timeout);
	signal(SIGINT, terminar);
	
	
	//1.- Procesamos los argumentos
	if (argc < 3) {
		printf("Uso: %s <IP> <fichero>\n",argv[0]);
		exit(1);
	}
	
	//2.- Declaración de variables
	
	//dirección del servidor
	struct sockaddr_in serv_addr;
	//longitudes
	uint8_t longitud;
	
	//3.- Montamos la dirección del servidor
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PUERTO);
	uint32_t ip_servidor;
	if ((ip_servidor = inet_addr(argv[1])) < 0) {
		printf("Error al convertir la IP %s\n", argv[1]);
		exit(1);
	}
	memcpy(&serv_addr.sin_addr, &ip_servidor, sizeof(ip_servidor));
	
	//4.- Creamos el socket
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(1);
	}	
	printf("socket\n");
	
	//5.- Abrimos la conexión con el servidor
	if (connect(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
		perror("connect");
		exit(1);
	}
	printf("connect\n");
	
	
	//6.- Enviamos el nombre de fichero
	//calculamos la longitud
	longitud = strlen(argv[2]);
		 
		 

	//la envianos (como es uint8_t no hay que pasar a big endian)
	if (write_n(sd, &longitud, sizeof(longitud)) != sizeof(longitud)) {
	 perror("write longitud");
	 exit(1);
	}

	//enviamos la cadena
	if  (write_n(sd, argv[2], longitud) != longitud) {
	 perror("write longitud");
	 exit(1);
	}
	
	struct itimerval t;
	struct timeval t0;
	t0.tv_sec = 2;
	t0.tv_usec = 0;
	struct timeval interval;
	interval.tv_sec = 2;
	interval.tv_usec = 0;
	
	t.it_value = t0;
	t.it_interval = interval;
	
	if (setitimer(ITIMER_REAL, &t, NULL) < 0 ) {
		perror("setitimer");
		exit(-1);
	}
	
	//7.- Enviamos todos los datos
	while(1) {
		pause();
	}

	
	return 0;
}

/**
 * Funciones auxiliares
 */
ssize_t read_n(int fd, void * mensaje, size_t longitud_mensaje) {
  ssize_t a_leer = longitud_mensaje;
  ssize_t total_leido = 0;
  ssize_t leido;
  
  do {
    errno = 0;
    leido = read(fd, mensaje + total_leido, a_leer);
    if (leido >= 0) {
      total_leido += leido;
      a_leer -= leido;
    }
  } while((
      (leido > 0) && (total_leido < longitud_mensaje)) ||
      (errno == EINTR));
  
  if (total_leido > 0) {
    return total_leido;
  } else {
    /* Para permitir que devuelva un posible error en la llamada a read() */
    return leido;
  }
}

ssize_t write_n(int fd, void * mensaje, size_t longitud_mensaje) {
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

int trimming(char * cad) {
	  int longitud = 0;
	  //calculamos la longitud
	 while (cad[longitud] != '\0') {
		 longitud++;
	 }
	 
	 if ((longitud > 0) && (cad[longitud-1] == '\n')) {
		cad[longitud -1] = '\0';
		longitud--;
	}
	
	return longitud;
}
