/* INCLUDES */

/* EVENTOS */
#define DLE 0X10
#define STX 0X02
#define ETX 0X03
#define EOT 0X04

ssize_t write_n(int fd, char *buffer, ssize_t tam){
	ssize_t escrito = 0;
	ssize_t longitud = tam;
	ssize_t total_escrito = 0;
	bool error = false;
	
	do{
		escrito = write(fd, buffer, longitud);
		if(escrito < 0){
			perror("Fallo en write_n");
			exit(-1);
			
		}else{
			total_escrito += escrito;
			longitud -= escrito;
		}
	
	}while( !error && escrito > 0 && total_escrito < tam);
	
	if(total_escrito > 0){
		return total_escrito;
	}else{
		return total_escrito;
		//Control de errores
	}
}

void emisor(int fd_emisor, char *nfichero){
	//Abrimos fichero que contiene la info
	int r = open(nfichero, O_RDONLY);
	if(r < 0){
		perror("Fallo en open emisor");
		exit(-1);
	}
	
	char buffer[256];
	int leidos, i;
	
	char c_dle = DLE; char c_stx = STX; char c_etx = ETX; char c_eot = EOT;
	int fin = 0;
	while(!fin){	
		int leidos = read(r, buffer, 256);
		if(leidos < 0){
			perror("Fallo en read de fichero pa emitir");
			exit(-1);
		}
	
		//MANDAMOS DLE Y STX PA ESTAR EN ESPERA DATOS
		write_n(fd_emisor, &c_dle, 1);
		write_n(fd_emisor, &c_stx, 1);
	
		for(i = 0; i < leidos; i++){
			write_n(fd_emisor, buffer[i], 1);
		
			if(buffer[i] == DLE){
				write_n(fd_emisor, &c_dle, 1);
			}
		}
	
		write_n(fd_emisor, &c_dle,1);
	
		if(leidos < 0){
			write_n(fd_emisor, c_etx, 1);
		
		}else{
			write_n(fd_emisor, c_eot, 1);
		}
	}
	
	close(r);

}

/* ESTADOS */
#define ESPERA_TRAMA 0
#define ESPERA_STX 1
#define ESPERA_DTE 2
#define MODO_DLE 3

void receptor(int fd_receptor, char *nfichero){
	int r = creat(nfichero, 0666);
	if(r < 0){
		perror("Fallo en open receptor");
		exit(-1);
	}
	
	int fin = 0;
	char buffer[256];
	
	while(!fin){
		char ch;
		int i = 0;
		
		read(fd_receptor, &ch, 1);
		
		switch(ch){
			case DLE:
				if
		
		}
		
	}
}