#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
//Harán falta más includes

int estadof(char * fichero) 
{
	
	struct stat datos_fichero;

	//1. Recupera la informacion del fichero llamando a stat y comprueba si hay error
	int resultado = stat(fichero, &datos_fichero);
	
	if(resultado < 0){
		return -1;
	}

	printf("Tipo de fichero:\n");
	//2. Imprime el tipo de fichero (regular o directorio)
	
	if(S_ISDIR(datos_fichero.st_mode)){
		printf("Es un directorio\n");
	}
	if (S_ISREG(datos_fichero.st_mode)) {
		printf("Es un fichero regular\n");
	}

	//3. Imprime el tamaño en bytes del fichero
	printf("Tamaño:  %lld bytes\n", (long long) datos_fichero.st_size); //el comodin %lld es un long long int
	return 0;

}

int main(int argc, char* argv[]){

	if (argc < 2){
		printf("Necesito al menos un argumento: el nombre del fichero\n");
		return 1;
	}

	return (estadof(argv[1]));
}
