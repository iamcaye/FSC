#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

int adivina(int num){

	int adiv;
	
	if((num < 0)||(num > 10)){
		printf("El valor introducido no está en el rango indicado...\n");
		return -1;
	}
	
	printf("Adivina el numero %d\n", num);
	
	scanf("%d",&adiv);
	
	while(num != adiv){
		if(num < adiv){
			printf("El numero es menor que %d \n", adiv);
		}else{
			printf("El numero es mayor que %d\n", adiv);
		}
		printf("Vuelve a intentarlo: ");
		scanf("%d",&adiv);
		//No me sale con read(0,....
	}
	
	printf("Has acertado!!\n");
	return 0;
	

}

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Necesito dos argumentos minimo...\n");
		return -1;
	}
	adivina(atoi(argv[1]));
	return  0;
}
