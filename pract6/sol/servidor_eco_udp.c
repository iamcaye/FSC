/*---------------------------------------------------------------
 * FICHERO:     servidor_eco_udp.c
 * DESCRIPCION: servidor de eco sobre UDP
 * SINTAXIS:    servidor_eco_udp
 *---------------------------------------------------------------
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> 

#define PUERTO 4950 /* Puerto de conexión */
#define TAM_BUFFER 100  /* Tamaño de buffer */

int main(int argc, char ** argv) {
  int sd; /* Descriptor de socket */
  int num_bytes; /* Bytes enviados o recibidos */
  struct sockaddr_in cli_addr, serv_addr; /* Direcciones cliente y servidor */
  socklen_t cli_len; /* Longitud de la dirección del cliente */
  char buffer[TAM_BUFFER]; /* Buffer */
  
  /* Creación del socket UDP */
  sd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("socket");
    exit(1);
  }
  
  /* Inicializar la estructura con la dirección del servidor */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PUERTO);
  serv_addr.sin_addr.s_addr = INADDR_ANY; /* Escucha por cualquier interfaz */
  
  /* Vincula el socket al puerto asignado */
  if (bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    exit(1);
  }
  
  /* Recibimos mensajes de clientes en un bucle */
  while(1) {
    /* Recibe datos del cliente, obteniendo también su IP y puerto */
    cli_len = sizeof(cli_addr);
    num_bytes = recvfrom(sd, buffer, TAM_BUFFER, 0, (struct sockaddr *) &cli_addr, &cli_len);
	    
    if (num_bytes < 0) {
      perror("recvfrom");
      exit(1);
    }
    
    /* Responder con un eco al cliente */
    num_bytes = sendto(sd, buffer, num_bytes, 0, (struct sockaddr *) &cli_addr, cli_len);
        
    if (num_bytes < 0) {
      perror("sendto");
      exit(1);
    }
  }
  
  /* Cerramos el socket */
  close(sd);
  
  return 0;
}
