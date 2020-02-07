//Ejercicio para leer un fichero de texto y mostrarlo por pantalla
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

//Faltan mas includes

int main(int argc, char *argv[]){

	//1. Declara un descriptor de fichero denominado fd_origen
	int fd_origen;
	char bloque[512];

	if (argc < ) {
		printf("Uso del programa: %s <fichero>\n", argv[0]);
		return 1;
	}
	//2. Abre fichero en modo lectura utilizando open()
	if ((fd_origen = open(argv[1], O_RDONLY)) < 0) {
		perror("ha fallado la apertura del fichero\n");
		return 1;
	}
	
	int bytesRead;
	//3. Lee el fichero en bloques. Si el fichero es de texto, hay que convertir cada bloque leido
	// en una cadena, insertando '\0' en la siguiente posicion libre para imprimir el bloque por pantalla
	while ((bytesRead = read(fd_origen, bloque, 511)) > 0){
		//4. Convierte el array bloque en cadena
		bloque[bytesRead] = '\0';
		
		printf("%s",bloque);
	}
	//5. Comprueba si hubo error en la llamada al sistema read()
	if(bytesRead == -1){
		perror("error en read\n");
		return 1;
	}
	//6. Cierra el fichero con close() y comprueba su valor de retorno
	if (close(fd_origen) == -1){
		perror("error cerrando fichero");
		return 1;
	}

	return 0;
}

