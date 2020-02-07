/* INCLUDES */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>

/* ESTADOS */
#define LISTO 0
#define RET_TX 1
#define ESP_CTS 2
#define ESP_TX_FIN 3
#define RET_RTS_OFF 4
#define ESP_CTS_OFF 5

/* EVENTOS */
#define ev_manda_msj 0
#define ev_timeout 1
#define ev_cts 2
#define ev_tx_fin 3
#define ev_cts_off 4

int fd_pipe[2];

int espera_evento(){
	uint8_t evento;
	if(read(fd_pipe[0], &evento, 1) < 0){
		perror("Fallo en recibir evento\n");
		exit(1);
	}
	
	printf("%d\n", evento);
	
	return evento;
}

void timeout(int r){
	printf("#timeout\n");
	uint8_t evento = ev_timeout;
	write(fd_pipe[1], &evento, 1);
	
	signal(SIGALRM, timeout);
}

void manda_msj(int r){
	printf("#Manda Mensaje\n");
	uint8_t evento = ev_manda_msj;
	write(fd_pipe[1], &evento, 1);
	
	signal(SIGINT, manda_msj);
}

void cts(int r){
	printf("#CTS\n");
	uint8_t evento = ev_cts;
	write(fd_pipe[1], &evento, 1);
	
	signal(SIGTERM, cts);
}

void tx_fin(int r){
	printf("#TX_FINISHED\n");
	uint8_t evento = ev_tx_fin;
	write(fd_pipe[1], &evento, 1);
	
	signal(SIGUSR1, tx_fin);
}

void cts_off(int r){
	printf("#CTS OFF\n");
	uint8_t evento = ev_cts_off;
	write(fd_pipe[1], &evento, 1);
	
	signal(SIGUSR2, cts_off);
}


int main(int argc, char *argv[]){

	int estado, evento, fin = 0;
	struct timeval tv;
	struct itimerval timer;
	
	pipe(fd_pipe);
	
	estado = LISTO;
	printf("<> Listo\n");
	
	signal(SIGALRM, timeout);
	signal(SIGINT, manda_msj);
	signal(SIGTERM, cts);
	signal(SIGUSR1, tx_fin);
	signal(SIGUSR2, cts_off);
	
	while(!fin){
		evento = espera_evento();
	
		switch(evento){
			case ev_manda_msj:
				if(estado == LISTO){
					estado = RET_TX;
					
					tv.tv_sec = 1;
					tv.tv_usec = 5000;
					timer.it_value = tv;
					timer.it_interval = tv;
					
					setitimer(ITIMER_REAL, &timer, 0);
					printf("<> Retraso TX\n");
				}else{
					printf("ev_manda_msj no esperado\n");
					fin=1;
				}
				break;
				
			case ev_timeout:
				switch(estado){
					case RET_TX:
						setitimer(ITIMER_REAL, 0, 0);
						printf("RTS activa\n");
						estado = ESP_CTS;
						printf("<> Espera CTS\n");
						
						tv.tv_sec = 3;
						tv.tv_usec = 3000;
						timer.it_value = tv;
						timer.it_interval = tv;
						
						setitimer(ITIMER_REAL, &timer, 0);
						break;
						
					case ESP_CTS:
						printf("Error: CTS no activa\n");
						fin = 1;
						break;
						
					case RET_RTS_OFF:
						setitimer(ITIMER_REAL, 0, 0);
						printf("RTS desactivada\n");
						estado = ESP_CTS_OFF;
						printf("<> Espera CTS off\n");
						
						tv.tv_sec = 4;
						tv.tv_usec = 20;
						timer.it_value = tv;
						timer.it_interval = tv;
						
						setitimer(ITIMER_REAL, &timer, 0);
						break;
						
					case ESP_CTS_OFF:
						printf("Error: CTS no desactivada\n");
						fin = 1;
						break;						
				}
				break;
				
			case ev_cts:
				if(estado == ESP_CTS){
					setitimer(ITIMER_REAL, 0, 0);
					printf("CTS activa");
				
					estado = ESP_TX_FIN;
					printf("<> Espera TX_FINISH\n");
				}
				break;
					
				
			case ev_tx_fin:
				if(estado == ESP_TX_FIN){
					tv.tv_sec = 1;
					tv.tv_usec = 50;
					timer.it_value = tv;
					timer.it_interval = tv;
				
					setitimer(ITIMER_REAL, &timer, 0);
				
					estado = RET_RTS_OFF;
					printf("<> Retraso RTS off");
				}
				break;
				
			case ev_cts_off:
				if(estado == ESP_CTS_OFF){
					setitimer(ITIMER_REAL, 0, 0);
					printf("CTS desactivada\n");
			
					estado = LISTO;
					printf("<> Listo\n");
				}
				break;				
				
				
		}
		
		printf("Vuelta\n");

	}


	return 0;
}

