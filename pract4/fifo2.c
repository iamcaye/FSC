//LEE X EL FIFO Y ESCRIBE POR PTLLA

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>    // para atributos de fichero tipo O_RD...
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]){

	int fd;
	char mensj[100] = {""};
	
	if((fd = open("/tmp/fifo_p4",O_RDONLY)) < 0){
		perror("error en open de leer x fifo");
		exit(-1);
	}
	
	int r;
	
	if( (r = read(fd,mensj,100)) < 0){
		perror("error en read de fifo2");
		exit(-1);
	}
	
	write(1,mensj,r);
	
	close(fd);

	return 0;
}