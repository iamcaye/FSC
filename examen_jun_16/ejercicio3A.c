#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

#define TAM 512
#define PUERTO 4950

int global = 0;
int cont;

ssize_t writen(int fd, void * mensaje, size_t longitud_mensaje);
ssize_t readn(int fd, void * mensaje, size_t longitud_mensaje);

void manejador(int r){
	global = 1;
	wait(0);
	signal(SIGCHLD, manejador);
}


int main(int argc, char *argv[]){

	int sd, n_sd;
	struct sockaddr_in cli_addr, serv_addr;
	uint16_t longitud;
	socklen_t cli_len, serv_len;
	int leidos, r, escrito, fin = 0;
	pid_t p;
	char buffer[TAM];
	

	/* DECLARAMOS LA DIRECCION DE SERVIDOR */
	memset(&serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PUERTO);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	/* SOCKET */
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("socket");
		exit(1);
	}
	
	/* BIND */
	serv_len = sizeof(serv_addr);
	if(bind(sd, (struct sockaddr *)&serv_addr, serv_len) < 0){
		perror("bind");
		close(sd);
		exit(1);
	}
	
	/* Ponemos el servidor en espera a clientes */
	if(listen(sd, 10) < 0){
		perror("listenn");
		close(sd);
		exit(1);
	}
	
	cli_len = sizeof(cli_addr);
	
	while(!fin){
		global = 0;
		if((n_sd = accept(sd, (struct sockaddr *)&cli_addr, &cli_len)) < 0 && global == 0){
			perror("accept");
			close(sd);
			exit(1);		
		}else if(n_sd > 0){		
			if((leidos = readn(n_sd, &longitud, sizeof(longitud))) < 0){
				perror("readn longitud");
				close(sd);
				close(n_sd);
				exit(1);
			}
		
			if((leidos = readn(n_sd, buffer, longitud)) != longitud){
				perror("readn buffer");
				close(sd);
				close(n_sd);
				exit(1);
			}
			
			if(cont < 10 ){
				p = fork();
				cont++;
			
				if(p < 0){
					perror("fork");
					close(sd);
					close(n_sd);
					exit(1);
				
				}else if(p == 0){
					r = system(buffer);
					longitud = sizeof(r);
					
					if(escrito = (n_sd, &longitud, sizeof(longitud)) != sizeof(longitud)){
						perror("writen long");
						close(n_sd);
						close(sd);
						exit(1);
					}
					
					if((escrito = writen(n_sd, &r, sizeof(r))) != longitud){
						perror("writen r");
						close(sd);
						close(n_sd);
						exit(1);
					}
					
					cont--;
					exit(0);
				}
			}
			
		}
		
		close(n_sd);
		
		
		
		
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
  
  
ssize_t readn(int fd, void * mensaje, size_t longitud_mensaje) {
  ssize_t a_leer = longitud_mensaje;
  ssize_t total_leido = 0;
  ssize_t leido;
  
  do {
    errno = 0;
    leido = read(fd, mensaje + total_leido, a_leer);
    if (leido >= 0) {
      total_leido += leido ;
      a_leer -= leido ;
    }
  } while(
        ((leido > 0) && (total_leido < longitud_mensaje)) ||
        (errno == EINTR));
  
  if (total_leido > 0) {
    return total_leido;
  } else {
    /* Para permitir que devuelva un posible error de la llamada a write */
    return leido;
  }
 }