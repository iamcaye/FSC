/* INCLUDES */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>

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

int espera_evento(){
	//return 1;
}

void timeout(int r){
	signal(SIGALRM, timeout);
}

int main(int argc, char *argv[]){

	int estado, evento, fin = 0;
	struct timeval tv;
	struct itimerval timer;
	
	estado = LISTO;
	printf("<> Listo\n");
	
	signal(SIGALRM, timeout);
	
	while(!fin){
		evento = espera_evento();
	
		switch(evento){
			case ev_manda_msj:
				if(estado == LISTO){
					estado = RET_TX;
					printf("<> Retraso TX\n");
					
					tv.tv_sec = 1;
					tv.tv_usec = 50000;
					timer.it_value = tv;
					
					setitimer(ITIMER_REAL, &timer, 0);
				}else{
					printf("ev_manda_msj no esperado\n");
					fin=1;
				}
				break;
				
			case ev_timeout:
				switch(estado){
					case RET_TX:
						printf("RTS activa\n");
						estado = ESP_CTS;
						printf("<> Espera CTS\n");
						
						tv.tv_sec = 3;
						tv.tv_usec = 30000;
						timer.it_value = tv;
						
						setitimer(ITIMER_REAL, &timer, 0);
						break;
						
					case ESP_CTS:
						printf("Error: CTS no activa\n");
						fin = 1;
						break;
						
					case RET_RTS_OFF:
						printf("RTS desactivada\n");
						estado = ESP_CTS_OFF;
						printf("<> Espera CTS off\n");
						
						tv.tv_sec = 4;
						tv.tv_usec = 20000;
						timer.it_value = tv;
						
						setitimer(ITIMER_REAL, &timer, 0);
						break;
						
					case ESP_CTS_OFF:
						printf("Error: CTS no desactivada\n");
						fin = 1;
						break;						
				}
				break;
				
			case ev_cts:
				setitimer(ITIMER_REAL, 0, 0);
				printf("CTS activa");
				
				estado = ESP_TX_FIN;
				printf("<> Espera TX_FINISH\n");
				break;
				
			case ev_tx_fin:
				tv.tv_sec = 1;
				tv.tv_usec = 50000;
				timer.it_value = tv;
				
				setitimer(ITIMER_REAL, &timer, 0);
				
				estado = RET_RTS_OFF;
				printf("<> Retraso RTS off");
				break;
				
			case ev_cts_off:
				setitimer(ITIMER_REAL, 0, 0);
				printf("CTS desactivada\n");
				
				estado = LISTO;
				printf("<> Listo\n");
				break;				
				
				
		}

	}


	return 0;
}

