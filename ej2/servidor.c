#include string.h
#include unistd.h
#include stdlib.h
#include systypes.h
#include syssocket.h
#include netinetin.h
#include stdio.h

int main(int argc, char argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0))  0) {
		printf(Error creando socketn);
		return 1;
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9005);
	
	if (bind(sock_listen, (struct sockaddr ) &serv_adr, sizeof(serv_adr))  0) {
		printf(Error al bindn);
		return 1;
	}
	
	if (listen(sock_listen, 3)  0) {
		printf(Error en el Listenn);
		return 1;
	}
	
	for (;;) {
		printf(Escuchandon);
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf(He recibido conexionn);
		
		int terminar = 0;
		while (terminar == 0) {
			ret = read(sock_conn, peticion, sizeof(peticion));
			printf(Recibidon);
			
			peticion[ret] = '0';
			printf(Peticion %sn, peticion);
			
			char p = strtok(peticion, );
			int codigo = atoi(p);
			float temperatura = 0.0;
			
			if (codigo != 0) {
				p = strtok(NULL, );
				if (p != NULL) {
					temperatura = atof(p);
				}
				printf(Codigo %d, Temperatura %.2fn, codigo, temperatura);
			}
			
			if (codigo == 0) 
				terminar = 1;
			else if (codigo == 1) {
				float celsius_a_fahrenheit(float celsius) {
					return (celsius  9  5) + 32;
				}
				float resultado = celsius_a_fahrenheit(temperatura);
				sprintf(respuesta, %.2f, resultado);
			}
			else if (codigo == 2) {
				float fahrenheit_a_celsius(float fahrenheit) {
					return (fahrenheit - 32)  5  9;
				}
				float resultado = fahrenheit_a_celsius(temperatura);
				sprintf(respuesta, %.2f, resultado);
			}
			
			if (codigo != 0) {
				printf(Respuesta %sn, respuesta);
				write(sock_conn, respuesta, strlen(respuesta));
			}
		}
		close(sock_conn);
	}
}