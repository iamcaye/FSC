/*------------------------------------------------------------------------
* Programa: chat con select
*
* Nota: Usando multiplexacion con select()
*------------------------------------------------------------------------
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#define TAM_BUFFER 1024

int main(){
	char buffer[TAM_BUFFER];
	fd_set cjto_descriptores, cjto_modificado ;

	/*1. Abre la fifo "/tmp/fsc_chat" para leer*/	
	int fd_fifo = open("/tmp/fsc_chat", O_RDONLY);
	if(fd_fifo < 0){
		perror("Abriendo fifo");
		exit(1);
	}
	/*2.  Se inicializa cjto_descriptores a cero*/
	FD_ZERO(&cjto_descriptores);
	/*3. Incluimos en el select el descriptor de teclado */
	FD_SET(0, &cjto_descriptores) ;
	/*4. Incluimos en el select el descriptor de la fifo */
	FD_SET(fd_fifo, &cjto_descriptores) ;
	
	while(1){		
		/*5. puesto que select modifica el fd_set que se pasa por referencia, se le pasa una copia para no perder los
		descriptores originales*/
		memcpy(&cjto_modificado,&cjto_descriptores,sizeof(fd_set));
		
		/*6. llama a select, que bloquea hasta que haya datos*/
		int result = select(fd_fifo+1, &cjto_modificado, NULL, NULL, NULL) ;
		
		if(result < 0){
			perror("select");
			exit(1);
		}else if(result > 0){
			/*7. Si hay datos por teclado */
			if( FD_ISSET(0, &cjto_modificado) ) {
					/* Lee bytes del teclado */
					int mensaje = read(0,buffer,TAM_BUFFER);
					printf("ENVIADO: %s\n",buffer); /*no manda nada a ningun sitio, es una simulacion*/

			}
			/*8. Si hay datos por la fifo */
			if(FD_ISSET(fd_fifo, &cjto_modificado)) { 
					int leidos = read(fd_fifo, buffer, TAM_BUFFER);
					if(leidos == -1){
						perror("Error en fifo");
						break; //salida del bucle while (fallo en pipe)
					}else if(leidos == 0){
						break;
					}else{
						buffer[leidos] = '\0';
						printf("RECIBIDOS (%d): %s\n",leidos,buffer);
					}
			}
		}else if(result == 0){
			exit(-1);
		}
	}
	
	/*9. En caso de salir del while, se cierran los descriptores abiertos por el programa*/
	close(fd_fifo);
	return 0;
}