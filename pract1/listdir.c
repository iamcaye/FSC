#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
//Hacen falta más includes

int estadof(char* fichero) 
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
	//Del ejercicio anterior




int listdir(char *nombre_directorio){

	//1. Declarar un puntero a estructura de directorio llamado ‘dir’
	 int* dir;

	//2. Abrir el directorio que han pasado como argumento y comprobar errores
	&dir = open(nombre_directorio, O_RDONLY);
	if (&dir < 0){
		printf("error al intentar abrir directorio\n");
		return -1;
	}

	//3. Declarar un puntero a estructura dirent llamada ‘entry’
	struct dirent *entry;
	
	printf("Contenidos del directorio:\n");
	//4. Mientras haya entradas que leer:
	
	while ((entry = read(dir, )) != ??) {
		printf("Nombre de fichero: %s\n", ??);
		estadof((char *)entry);
		printf("\n\n");
	}

	//5. Cerrar directorio y comprobar errores
	if (close(dir)) {
		printf("problema al cerrar directorio\n");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[]){

	if (argc < 2){
		printf("Necesito al menos un argumento: el nombre del fichero\n");
		return -1;
	}
	
	return listdir(argv[1]);
}
