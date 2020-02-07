#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Hacen falta mas includes

int main(int argc, char* argv[])
{

	char comando[40] = "ls ";

	if (argc < 2) {
		printf("Necesito al menos un argumento...\n");
		return -1;
	}

	if (!strcmp(argv[1],"-l")){
		strcat(comando, argv[1]); //constuye el comando incluyendo el argumento "-l"
		//Hacer la llamada correspondiente a la funcion system() para que ejecute el comando que esta en la cadena
		system(comando);
	} else {
		printf("ERROR: La opcion introducida no es -l\n");
		return -1;
	}

	return 0;
}
