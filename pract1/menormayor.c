#include <stdio.h>

int main(int argc, char *argv[]){

	int array[50];
	int x, menor, mayor;

	printf("%s", "Introduzca una serie de numeros\n");
	
	scanf("%d",&x);
	menor = x;
	mayor = x;
	
	
	while(x != 0){
		if(x < menor){
			menor = x;
		}
		
		if(x > mayor){
			mayor = x;
		}
		
		scanf("%d",&x);
		
		printf("%d\n", x);
	}
	
	printf("El mayor es: %d\n", mayor);
	printf("EL menor es: %d\n", menor);

	return;
}