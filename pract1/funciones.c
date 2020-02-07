#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t writen(int fd, void *memoria, size_t n);
ssize_t readn(int fd, void *memoria, size_t n);

int main(int argc, char *argv[]){
	int fd = creat("mifichero.txt", 0640);

	int pruebaw, pruebar;
	char array1[1024];
	char array2[1024];
	
	read(1, array1, 1023);
	
	pruebaw = writen(fd, array1,strlen(array1));
	
	pruebar = readn(fd, array2, strlen(array1));	
	
	write(0, array2, strlen(array2));
	
	close(fd);

	return;
}

ssize_t writen(int fd, void *memoria, size_t n){
	if(fd != -1){
		int escritos = write(fd,memoria,n);

		if(escritos != n){	
			while(escritos < n){
				char *p = (char *)memoria;
				escritos += write(fd, p + escritos, n - escritos);
			}
		}else{
			return 0;
		}
		
	}else{
		return -1;
	}

}

//NO FUNCIONA READN
/*
ssize_t readn(int fd, void *memoria, size_t n){
	if(fd != -1){
		int leido = read(fd, memoria,n);
		write(0,leido,sizeof(int));
		
		if(leido != n){
			while(leido < n){
				char *p = (char *)memoria;
				leido += read(fd, p + leido, n - leido); 
				write(0,&leido, sizeof(int));
			}
		}else{
			return 0;	
		}
	}else{
		return -1;
	}		
}


*/