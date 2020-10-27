#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>


int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	char nombre[20];
	float altura;
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9400);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	// bucle infinito
	for (;;)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//Bucle atención al cliente
		int terminar = 0;
		while(terminar != 1){			
			
			// Ahora recibimos la petici?n
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			printf ("Peticion: %s\n",peticion);
			char *p = strtok( peticion, "/");
			int codigo = atoi (p);
			if(codigo != 0){
				p = strtok( NULL, "/");
				strcpy (nombre, p);
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			if(codigo == 0)
				terminar = 1;
			else if (codigo == 1)
				sprintf (respuesta,"%d",strlen (nombre));
			else if (codigo == 2){
				if((nombre[0] == 'M') || (nombre[0] == 'S'))
				    strcpy (respuesta,"SI");
				else
					strcpy (respuesta,"NO");
            }
			else if (codigo == 3){
				p = strtok( NULL, "/");
				altura =  atof (nombre);
				if (altura > 1.70)
					strcpy (respuesta, "alta");
				else
					strcpy (respuesta, "baja");
			}
        
            else if (codigo == 4){
                int error = 0;
				//pasamos todo el nombre pasado por el comando a minúsculas para poder comparar todas las letras correctamente.
				int i=0;
				for (int indice = 0; nombre[indice] != '\0'; ++indice) {
		            nombre[indice] = tolower(nombre[indice]);
					i++;
				}
                //miramos que la primera letra sea igual que la última y así seguidamente hasta llegar a la del medio
                for(int aux = 0; nombre[aux] != '\0' ; aux++)
                    if(nombre[aux] != nombre[i-1-aux])
						error = 1;
					
                if(error == 1)
                    strcpy (respuesta, "NO");
                else 
                    strcpy (respuesta, "SI");
            }  
            else
            {
                for (int i = 0; nombre[i] != '\0'; i++)
		            nombre[i] = toupper(nombre[i]);
                strcpy (respuesta, nombre);
            }
			
			if (codigo != 0)
			{
				printf ("Respuesta: %s\n", respuesta);
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}
		
	}
	close(sock_conn); 
}
