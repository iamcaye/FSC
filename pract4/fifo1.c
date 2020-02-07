//FIFO DE LECTURA POR PANTALLA

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>    // para atributos de fichero tipo O_RD...
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]){
	mkfifo("/tmp/fifo_p4",0666);
	int fd;
	if((fd = open("/tmp/fifo_p4",O_WRONLY | O_APPEND)) < 0){
		perror("error open fifo_p4");
		exit(-1);
	}
	char mensj[100]={""};
	
	int r;
	
		if((r = read(0, mensj, 99)) < 0){
			perror("fallo en leer x ptlla");
			exit(-1);
		}
		mensj[r] = '\0';
		write(fd, mensj, r);
		
		close(fd);
	

	return 0;
}