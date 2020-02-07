//Ejercicio para introducir datos en ficheros y luego recuperarlos
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

struct Estructura{
	int a;
	float b;
	char c;
};

int main(int argc, char *argv[]){

	//1. Declara un descriptor de fichero denominado fd
	int fd;
	char bloque[512];

	//2. Crea y abre el fichero utilizando creat() al menos con permisos de lectura y escritura para el propietario
	if ((fd = creat("temporal.bin", 0640)) < 0) {
		perror("ha fallado la creacion del fichero\n");
		return 1;
	}

	//3. Escribe el valor de la variable entera x en el fichero
	int x = 7;
	if(write(fd, &x, sizeof(int))!= sizeof(int)){
		perror("error en write del valor de x");
		return 1;
	}	
		
	//4. Escribe el valor de un array de enteros en el fichero
	int array_enteros[4] = {0x00,0x01,0x02,0x03};
	if(write(fd,array_enteros, 4*sizeof(int))!= sizeof(int)*4 ){
		perror("error en write del array");
		return 1;
	}
	
	//5. Escribe el valor de una estructura en el fichero
	struct Estructura est;
	est.a = 1; est.b = 2.0; est.c = '3';
	if(write(fd, &est, sizeof(est))!= sizeof(est)){
		perror("error en write de la estructura");
		return 1;
	}
	
	//6. Cierra el fichero con close() y comprueba su valor de retorno
	if (close(fd) == -1){
		perror("error cerrando fichero");
		return 1;
	}
	
	//7. Abre ahora el fichero otra vez para lectura, esta vez con open()
	/*NOTA INTERESANTE: si el fichero anterior se hubiera creado utilizando open y con estos flags: 
			O_CREAT| O_RDONLY| O_WRONLY| O_TRUNC
	  no haria falta cerrarlo y volverlo a abrir, tan solo rebobinar al principio para empezar a leer:
			lseek(fd, 0, SEEK_SET); //pone a cero el desplazamiento de lectura/escritura del fichero
	 */
	if ((fd = open("temporal.bin", 0640)) < 0) {
		perror("ha fallado la creacion del fichero\n");
		return 1;
	}	
	  
	//La informacion en el fichero es la siguiente: <entero,array de 4 enteros,estructura>
	//Asi que hay que leer en ese mismo orden!!
	  
	//8. Lee el valor del entero en la variable y. Imprimelo por pantalla  
	int y;
	if(read(fd,&y,sizeof(int)) != sizeof(int)){
		perror("error leyendo el entero");
		return 1;
	}	
	printf("Valor entero: %d\n",y);
	
	//9. Lee el valor del array de 4 enteros en la variable arr. Imprimelo por pantalla  
	int arr[4];
	if(read(fd,arr,sizeof(int)*4) != (sizeof(int)*4)){
		perror("error leyendo el array de enteros");
		return 1;
	}
	int indice;
	for(indice=0;indice < 4; indice++)
		printf("Valor casilla %d del array: %x\n",indice,arr[indice]);

	//10. Lee el valor de una estructura de tipo Estructura en la variable z. Imprimela por pantalla  
	struct Estructura z;
	if(read(fd,&z,sizeof(est)) != sizeof(est)){
		perror("error leyendo la estructura");
		return 1;
	}	
	printf("Valores-> z.a: %d, z.b: %f, z.c: %c\n",z.a,z.b,z.c);	
		
	//11. Cierra el fichero con close() y comprueba su valor de retorno
	if (close(fd) == -1){
		perror("error cerrando el fichero");
		return 1;
	}	
		
	/*NOTA INTERESANTE: el archivo "temporal.bin" permanece en el sistema. Abrelo con un editor y comprueba
	que sigue conteniendo la informacion, pero no es texto legible*/	
		
		
	return 0;
}

