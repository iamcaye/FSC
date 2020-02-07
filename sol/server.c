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

#define PUERTO 4950
#define MAXBUFFER 512

ssize_t read_n(int fd, void * mensaje, size_t longitud_mensaje);
ssize_t write_n(int fd, void * mensaje, size_t longitud_mensaje);


int main(int argc, char * argv[]) {
	//1.- Declaramos variables
	int sd, n_sd; //descriptores de socket
	char buffer[MAXBUFFER];
	uint8_t longitud;
	//direcciones de socket
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	
	//2.- Creamos la dirección del servidor
	//2.1.- Inicializamos
	memset(&serv_addr, 0, sizeof(serv_addr));
	//2.2.- Rellenamos campos
	 //2.2.1.- Familia de direcciones
	 serv_addr.sin_family = AF_INET;
	 //2.2.2.- Puerto
	 serv_addr.sin_port = htons(PUERTO);
	 //2.2.3.- Dirección IP
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 
	//4.- Creamos el socket
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(1);
	}
	printf("socket\n");
	
	
	//5.- Asociamos el socket al puerto: bind()
	if (bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
		perror("bind");
		exit(1);
	}
	printf("bind\n");
	
	//6.- Ponemos el socket en escucha (modo pasivo): listen()
	if (listen(sd, 5) < 0)  {
		perror("listen");
		exit(1);
	}
	printf("listen\n");
	
	//7.- Iteramos
	uint16_t longBigEndian;
	uint16_t dataBigEndian;
	int   data;
	int leidos;
	while (1) {
		printf("Servidor  esperando a cliente...\n");
		socklen_t addr_len = sizeof(cli_addr);
		if ( (n_sd = accept(sd, (struct sockaddr *) &cli_addr, &addr_len )) < 0) {
			perror("accept");
			close(n_sd);
			close(sd);
			exit(1);
		} //*/
		printf("accept\n");
		
		//7.1.- Recibimos la longitud del nombre
		if (read_n(n_sd, &longitud, sizeof(longitud)) != sizeof(longitud)) {
			perror("read_n longBigEndian");
			close(n_sd);
			close(sd);
		}
		
		//leemos la cadena
		if (read_n(n_sd, buffer, longitud) != longitud) {
			perror("read_n buffer");
			close(sd);
			close(n_sd);
			exit(1);
		}
		
		buffer[longitud] = '\0';
		printf("fichero = %s\n", buffer);
		
		//7.2.- abrimos el fichero
		int fd;
		if ((fd = open(buffer, O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0) {
			perror("open");
			close(sd);
			close(n_sd);
		}
		
		printf("leyendo datos...\n");
		//7.2.- Recibimos los números
		leidos = read_n(n_sd, &dataBigEndian, sizeof(dataBigEndian));
		while (leidos > 0) {
			if (leidos != sizeof(dataBigEndian)) {
				perror("read dataBigEndian");
				close(sd);
				close(n_sd);
				close(fd);
			}
			
			//convertimos a formato de host
			data = ntohs(dataBigEndian);
			
			printf("dato = %d\n", data);
			
			//escribirmos los datos en disco
			if (write_n(fd, &data, sizeof(data)) != sizeof(data)) {
				perror("write data");
				close(sd);
				close(n_sd);
				close(fd);
			}
			
			if (write_n(fd, "\n", 1) != 1) {
				perror("write newline");
				close(sd);
				close(n_sd);
				close(fd);
			}
			
			//vuelta a leer
			leidos = read_n(n_sd, &dataBigEndian, sizeof(dataBigEndian));
		}
		
		//comprobación de errores
		if (leidos < 0 ) {
			perror("read dataBigEndian");
			close(sd);
			close(n_sd);
			close(fd);
		}
		else { //fin de conexión 
			//cierre de socket
			if (close(n_sd) < 0) {
				perror("close n_sd");
				close(sd);
				close(fd);
			}
			//cierre de fichero
			if (close(fd) < 0) {
				perror("close fd");
				close(sd);
				close(n_sd);
			}
		} // else
	} //while
	
	return 0;
} //main
			
			
		
	
	
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

