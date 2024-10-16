#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	
	// INICIALIZACIONES
	// Abrimos el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando socket");
	
	// Hacemos el bind al puerto
	memset(&serv_adr, 0, sizeof(serv_adr)); // inicializa a cero serv_adr
	serv_adr.sin_family = AF_INET;
	
	// Asocia el socket a cualquiera de las IP de la maquina.
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Establecemos el puerto de escucha
	serv_adr.sin_port = htons(9001);
	
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	// Bucle infinito
	for (;;)
	{
		printf("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		
		int terminar = 0;
		// Entramos en un bucle para atender todas las peticiones de este cliente
		// hasta que se desconecte
		while (terminar == 0)
		{
			// Ahora recibimos la peticiￃﾳn
			ret = read(sock_conn, peticion, sizeof(peticion));
			printf("Recibido\n");
			
			// Aￃﾱadimos la marca de fin de string
			peticion[ret] = '\0';
			
			printf("Peticion: %s\n", peticion);
			
			// Vamos a ver que quieren
			char *p = strtok(peticion, "/");
			int codigo = atoi(p); // Obtenemos el codigo de la peticion
			char nombre[20] = ""; // Inicializamos el nombre como cadena vacia
			
			if (codigo != 0)
			{
				p = strtok(NULL, "/"); // Extraemos el nombre
				if (p != NULL) {
					strncpy(nombre, p, sizeof(nombre) - 1); // Copiamos el nombre
					nombre[sizeof(nombre) - 1] = '\0'; // Aseguramos que sea nulo terminado
				}
				printf("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo == 0) // Peticiￃﾳn de desconexion
				terminar = 1;
			else if (codigo == 1) // Piden la longitud del nombre
				sprintf(respuesta, "%d", strlen(nombre));
			else if (codigo == 2) // Quieren saber si el nombre es bonito
				if ((nombre[0] == 'M') || (nombre[0] == 'S'))
					strcpy(respuesta, "SI");
				else
					strcpy(respuesta, "NO");
				else if (codigo == 3) // Quiere saber si es alto
				{
					p = strtok(NULL, "/");
					float altura = atof(p);
					if (altura > 1.70)
						sprintf(respuesta, "%s: eres alto", nombre);
					else
						sprintf(respuesta, "%s: eres bajo", nombre);
				}
				else if (codigo == 4) // Convertir nombre a mayusculas
				{
					for (int j = 0; nombre[j] != '\0'; j++) {
						if (nombre[j] >= 'a' && nombre[j] <= 'z') {
							nombre[j] = nombre[j] - ('a' - 'A'); 
						}
					}
					sprintf(respuesta, "%s", nombre);
				}
				else if (codigo == 5) // Comprobar si es paliﾭndromo
				{
					int longitud = strlen(nombre);
					int es_palindromo = 1; 
					
					for (int i = 0; i < longitud / 2; i++) {
						if (nombre[i] != nombre[longitud - i - 1]) {
							es_palindromo = 0;
						}
					}
					
					if (es_palindromo)
						strcpy(respuesta, "SI");
					else
						strcpy(respuesta, "NO");
				}
				
				if (codigo != 0)
				{
					printf("Respuesta: %s\n", respuesta);
					// Enviamos respuesta
					write(sock_conn, respuesta, strlen(respuesta));
				}
		}
		// Se acaba el servicio para este cliente
		close(sock_conn);
	}
}