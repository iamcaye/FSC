#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<string.h>

int main(int argc, char *argv[]){
	
	if(argc < 2){
		printf("Debe introducir dos argumentos minimo...\n");
		exit(1);
	}
	pid_t p = fork();
	
	if(p < 0){
		printf("Error al crear clon...\n");
		exit(1);
		
	}else if(p > 0){
		wait(0);
	}else{
		char cmd[100]={' '};
		int i;
		for(i = 1; i < argc ; i++){
			strcat(cmd,argv[i]);
			strcat(cmd," ");
		} 
	
		int r = execl("/bin/sh","sh","-c", cmd, NULL);
			
		if(r < 0){
		 printf("No funciona");
		 exit(1);
		}else{exit(1);}
	}

	
	return 0;

}
