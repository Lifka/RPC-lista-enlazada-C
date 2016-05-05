/*
		Javier Izquierdo Vera
		javierizquierdovera@gmail.com
		UGR
 */

#include "asociacion.h"

#define PonerAsociacion 0
#define ObtenerAsociacion 1
#define BorrarAsociacion 2
#define Enumerar 3
#define RED  "\x1B[31m"
#define GRE  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define RESET "\033[0m"


typedef enum estado Estado;
CLIENT *clnt;

 estado PonerAsociacionServer(int id, char* clave, char* valor){
		estado* resultado;
		resultado = ponerasociacion_1(id, clave, valor, clnt);

		if (resultado == (estado *) NULL) {
			clnt_perror (clnt, "PonerAsociacion call failed");
			printf ("%s(!)%s No es posible conectar con el servidor\n",RED,RESET);
			exit (1);
		}

		return *resultado;
 }

 valor_estado ObtenerAsociacionServer(int id, char* clave){
 	
		valor_estado* resultado = obtenerasociacion_1(id, clave, clnt);


		if (resultado == (valor_estado *) NULL) {
			clnt_perror (clnt, "ObtenerAsociacion call failed");
			printf ("%s(!)%s No es posible conectar con el servidor\n",RED,RESET);
			exit (1);
		}
		
		return *resultado;
 }

 estado BorrarAsociacionServer(int id, char* clave){
 		estado* resultado;
 		resultado = borrarasociacion_1(id, clave, clnt);

		if (resultado == (estado *) NULL) {
			clnt_perror (clnt, "BorrarAsociacion call failed");
			printf ("%s(!)%s No es posible conectar con el servidor\n",RED,RESET);
			exit (1);
		}

		return *resultado;
 }

enumerados EnumerarServer(int id){
		enumerados* resultado;
		resultado = enumerar_1(id, clnt);

		if (resultado == (enumerados *) NULL) {
			clnt_perror (clnt, "Enumerar call failed");
			printf ("%s(!)%s No es posible conectar con el servidor\n",RED,RESET);
			exit (1);
		}

		return *resultado;
}


void conectar(char *host){


		#ifndef	DEBUG
			clnt = clnt_create (host, ASOCIACION, ASOCIACION_1, "udp");
			if (clnt == NULL) {
				clnt_pcreateerror (host);
				printf ("%s(!)%s No es posible conectar con el servidor \"%s\"\n",RED,RESET,host);
				exit (1);
			}
		#endif	/* DEBUG */

}

void desconectar(){

		#ifndef	DEBUG
			clnt_destroy (clnt);
		#endif	 /* DEBUG */

}


int main (int argc, char *argv[]){
	char* host;
	int orden_server;

	int id;
	char* clave, *valor;



	if (argc == 6 && strcmp(argv[2],"add") == 0){
		orden_server = PonerAsociacion;
		id = atoi(argv[3]);
		clave = argv[4];
		valor = argv[5];
	} else if(argc == 5 && strcmp(argv[2],"get") == 0){
		orden_server = ObtenerAsociacion;
		id = atoi(argv[3]);
		clave = argv[4];
	} else if(argc == 5 && strcmp(argv[2],"remove") == 0){
		orden_server = BorrarAsociacion;
		id = atoi(argv[3]);
		clave = argv[4];
	} else if(argc == 4 && strcmp(argv[2],"getAll") == 0){
		orden_server = Enumerar;
		id = atoi(argv[3]);
	} else {
		printf ("usage: %s server_host add id[>0] clave valor\n", argv[0]);
		printf ("usage: %s server_host get id[>0] clave\n", argv[0]);
		printf ("usage: %s server_host remove id[>0] clave\n", argv[0]);
		printf ("usage: %s server_host getAll id[>0]\n", argv[0]);
		exit (1);
	}



	if(id < 0) {
		printf ("%s(!)%s id debe ser mayor que 0\n",RED,RESET);
		exit (1);
	}


	host = argv[1];
	conectar(host);

	if (orden_server == PonerAsociacion){
		printf("Añadiendo (%d,%s,%s)...\n",id,clave,valor);
		estado resultado = PonerAsociacionServer(id, clave, valor);

		if (resultado == OK){
			printf("%s[*]%s OK\n",GRE,RESET);

		} else if(resultado == NO_ENCONTRADO){
			printf("%s[*]%s NO_ENCONTRADO -> El valor no existía, pero se ha creado\n",GRE,RESET);


		} else if(resultado == SUSTITUCION){
			printf("%s[*]%s SUSTITUCIÓN -> El valor ya existía y se ha actualizado\n",GRE,RESET);

		} else {
			printf("%s(!)%s Se ha producido un error desconocido\n",RED,RESET);

		}

	} else if(orden_server == ObtenerAsociacion){
		printf("Obteniendo (%d,%s,*)...\n",id,clave);
		valor_estado resultado = ObtenerAsociacionServer(id, clave);

		if (resultado.Estado == OK){
			printf("%s[*]%s OK -> [%s] => %s\n",GRE,RESET,clave,resultado.valor);

		} else if(resultado.Estado == NO_ENCONTRADO){
			printf("%s[*]%s NO_ENCONTRADO -> No se han encontrado coincidencias\n",GRE,RESET);

		} else {
			printf("%s(!)%s Se ha producido un error desconocido\n",RED,RESET);

		}

	} else if(orden_server == BorrarAsociacion){
		printf("Borrando (%d,%s,*)...\n",id,clave);
		estado resultado = BorrarAsociacionServer(id, clave);

		if (resultado == OK){
			printf("%s[*]%s OK -> Se ha eliminado correctamente\n",GRE,RESET);

		} else if(resultado == NO_ENCONTRADO){
			printf("%s[*]%s NO_ENCONTRADO -> No se han encontrado coincidencias\n",RED,RESET);

		} else {
			printf("%s(!)%s Se ha producido un error desconocido\n",RED,RESET);

		}

	} else if(orden_server == Enumerar){
		printf("Obteniendo (%d,*,*)...\n",id);
		enumerados resultado = EnumerarServer(id);

		if (resultado.Estado == OK){
			printf("%s[*]%s OK -> Se han encontrado coincidencias\n",GRE,RESET);
		
			int i;
			for(i = 0; i < resultado.tupla.tupla_len; i++)
				printf("\t[%s] => %s\n",resultado.tupla.tupla_val[i].clave,resultado.tupla.tupla_val[i].valor);


		} else if(resultado.Estado == NO_ENCONTRADO){
			printf("%s[*]%s NO_ENCONTRADO -> No se han encontrado coincidencias\n",RED,RESET);

		} else {
			printf("%s(!)%s Se ha producido un error desconocido\n",RED,RESET);

		}

	}

	desconectar();

exit (0);
}
