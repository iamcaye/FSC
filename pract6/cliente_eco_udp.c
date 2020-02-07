/*--------------------------------------------------------------------------
 * FICHERO:     cliente_eco_udp.c
 * DESCRIPCION: cliente que hace uso del servidor de eco
 * SINTAXIS:    cliente_eco [direccionIP]
 *              Si no se especifica la IP, se usa 127.0.0.1
 *--------------------------------------------------------------------------
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> 
#include <stdint.h>

#define PUERTO 4950 /* Puerto de conexión */
#define TAM_BUFFER 100 /* Tamaño de buffer */

const char IP_SERVIDOR[] = "127.0.0.1"; /* IP del servidor por defecto */

int main(int argc, char ** argv) {
  int sd; /* Descriptor de socket */
  int num_bytes, leidos;
  struct sockaddr_in serv_addr; /* Dirección del servidor */
  uint32_t ip_servidor; /* IP del servidor */
  socklen_t serv_len; /* Longitud de la dirección del servidor */
  char buffer[TAM_BUFFER]; /* Array de bytes */
  char ip_serv[20]; /* Dirección IP del servidor */

  /* Obtenemos la dirección IP del servidor */
  if (argc <= 1) {
    /* No se ha indicado como argumento. Se usa la IP por defecto. */
    ip_servidor = inet_addr(IP_SERVIDOR);
  } else {
    ip_servidor = inet_addr(argv[1]);
  }
  
  /* Comprobamos que la IP del servidor es válida */
  if (ip_servidor == -1) {
    perror("Fallo en asignar la ip cliente");
    exit(1);
  }
  
  /* Creamos el socket UDP */
  sd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("Fallo en la creacion de socket");
    exit(1);
  }
  
  /* Inicializar la estructura con la dirección del servidor */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PUERTO);
  memcpy(&serv_addr.sin_addr, &ip_servidor, 4);
  
  printf("Bienvenido al cliente de eco UDP\n\n");
  while (1) {
    /* Leemos una cadena por teclado */
    printf("Introduzca una cadena cualquiera:\n");
	
	leidos = read(0, buffer, TAM_BUFFER); /*lee bytes de teclado*/
    
    /* Envía la cadena al servidor */
    num_bytes = sendto(sd, buffer, leidos, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
      
    if (num_bytes < 0) {
      /* Error enviando el datagrama */
      perror("Fallo en sendto al servidor");
      close(sd);
      exit(1);
    }
    
    /* Espera la respuesta (eco) del servidor */
    serv_len = sizeof(serv_addr);
    num_bytes = recvfrom(sd, buffer, TAM_BUFFER, 0, (struct sockaddr *)&serv_addr, &serv_len);
      
    if (num_bytes < 0) {
      /* Error al recibir cadena */
      perror("recvfrom maaaal");
      close(sd);
      exit(1);
    }
    
    buffer[num_bytes] = '\0'; /* Se añade '\0' al final para que sea una cadena */
    printf("Recibido eco: %s\n\n", buffer);
  }
  
  /* Cerramos el socket */
  close(sd);
  
  return 0;
}
