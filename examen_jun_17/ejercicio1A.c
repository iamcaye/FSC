/* INCLUDES */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


/* ESTADOS */
#define SS 0
#define CA 1

/* EVENTOS */
#define ev_ack 0
#define ev_dupack 1

int espera_evento(){
	char buffer[4];
	int evento = -1;

	int fd = open("/tmp/congestionTCP", O_RDONLY);
	if(fd < 0){
		perror("Fallo en open espera_evento");
		exit(1);
	}

	
	int l = read(fd, buffer, 4);
	if(l < 0 || l != 4){
		perror("Fallo en lectura de fifo");
		close(fd);
		exit(1);
	}
	
	buffer[3] = '\0';
	
	printf("%s\n",buffer);
	
	if(strcmp(buffer, "DCK") == 0){
		printf("Evento recibido: DCK\n");
		evento = ev_dupack;
		
	}
	if(strcmp("ACK", buffer) == 0){
		printf("Evento recibido: ACK\n");
		evento = ev_ack;
		
	}
	
	return evento;
}

int main(int argc, char *argv[]){

	int evento, fin = 0;
	int estado = SS;
	double cwnd = 1.0, umbral = 4.0;
	int dupACKs = 0;
	
	while(!fin){
		evento = espera_evento();
		
		switch(evento){
			case ev_ack:
				if(estado == SS){
					cwnd = cwnd + 1.0;
					dupACKs = 0;
					
					if(cwnd >= umbral){
						printf("SS -> CA\n");
						estado = CA;
					}
				}else{
					cwnd = cwnd + 1.0/cwnd;
					dupACKs = 0;
				}
				break;
				
			case ev_dupack:
				if(estado == SS){
					dupACKs++;
					
					if(dupACKs >= 3){
						cwnd = 1.0;
						dupACKs = 0;
					}
				}else{
					dupACKs++;
					
					if(dupACKs >= 3){
						cwnd = 1.0;
						dupACKs = 0;
						umbral = cwnd/2.0;
						printf("CA -> SS\n");
						estado = SS;
					}
				}
				break;
			
			default:
				printf("Evento no esperado\n");
				fin = 1;
		}
	}
	


	return 0;
}