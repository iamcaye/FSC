#include<stdio.h>
#include <unistd.h>
#include <errno.h>

int main(){}

ssize_t readn(int fd, void * memoria, size_t n){
	size_t totalLeidos = 0; int l;
	char *p = (char *)memoria;
	
	do{
		l = read(0,p+totalLeidos, n-totalLeidos);
		if(l > 0){
			totalLeidos += l;
		}
	}while(((l > 0)&&(totalLeidos < n))||(errno == EINTR));
	
	return !(totalLeidos < n);	
}