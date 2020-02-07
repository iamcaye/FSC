#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
//Hacen falta más includes (mirar manual)

int copiaFichero(char *nombre_origen, char *nombre_destino)
{
	char bloque[512]; //memoria auxiliar para mover los datos de un fichero a otro (puede tener otro tamaño)
	
	//1. Declara los descriptores de ficheros fd_destino y fd_origen
		int fd_destino, fd_origen;
	//2. Abre ambos ficheros y comprobar si ha habido algún error
		//2.a Abre el fichero nombre_origen en modo lectura y asignalo a fd_origen
		fd_origen = open(nombre_origen,O_RDONLY);
		//2.b Crea el fichero nombre_destino con permisos rw-rw-rw- y asignalo a fd_destino
		fd_destino = creat(nombre_destino, 0666);
	if (fd_origen < 0){
		perror("ha fallado la apertura del fichero origen");
		return -1;
	}

	if (fd_destino < 0){
		perror("ha fallado la creacion del fichero destino");
		return -1;
	}
	
	//3. Mientras haya algo que leer…
	int bytesRead, bytesWritten;
	while ((bytesRead = read(fd_origen, bloque, 512))>0){
		//4. Escribe lo que hayas leido y comprueba si ha habido algun error
		// (TEN MUCHO CUIDADO CON LOS BYTES QUE ESCRIBES EN EL FICHERO!!!)
		bytesWritten = write(fd_destino, bloque, 512);
		if (bytesWritten < 0){
			perror("error en write\n");
			return -1;
		}
	}
	if(bytesRead == -1){
		perror("error en read\n");
		return -1;
	}

	
	//5. Cierra los ficheros
	close(fd_destino);
	close(fd_origen);	
	
	return 0;
}

int main(int argc, char* argv[]){
	if (argc < 3) {
		printf("Modo de uso: %s <fichero_origen> <fichero_destino>\n",argv[0] );
		return -1;
	}

	return copiaFichero(argv[1], argv[2]);
}
