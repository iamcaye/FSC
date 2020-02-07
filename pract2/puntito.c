//1. Includes
#include<stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>

//2. Hace falta una función aquí...
void manejador(int s){
	write(1,".\n", 2);
	signal(SIGALRM, manejador);
};


int main(){

	//3. Ignoramos interrupción (Ctrl-C)
	signal(SIGINT, SIG_IGN);
  
	//4. Declaramos temporizador y valor de tiempo
	struct itimerval timer;

	//5. Establecemos el tiempo (de CPU)
	timer.it_value.tv_sec = 3;
	timer.it_value.tv_usec = 300000;
	timer.it_interval.tv_sec = 3;
	timer.it_interval.tv_usec = 300000;

	//6. Establecer manejador 
	signal(SIGALRM, manejador);

	//7. Establecer temporizador
 	setitimer (ITIMER_REAL, &timer, NULL);
  
  while(1){
  	pause();
  };

  return 0;
}
