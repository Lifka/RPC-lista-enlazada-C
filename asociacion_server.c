/*
		Javier Izquierdo Vera
		javierizquierdovera@gmail.com
		UGR
 */

#include "asociacion.h"

 typedef enum estado Estado;
 typedef struct tupla Tupla;
 typedef struct lista Lista;

#define RED  "\x1B[31m"
#define GRE  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define RESET "\033[0m"


Lista* lista_aso = 0;



void debug(){
	Lista* lista_actual = lista_aso;
	while(lista_actual != 0){
		printf("%d:\n", lista_actual->id);

		Tupla* tupla_actual = lista_actual->lista_tupla;

		while(tupla_actual != 0){
			printf("\t[%s] => %s\n", tupla_actual->clave, tupla_actual->valor);
			tupla_actual = tupla_actual->siguiente;
		}

		lista_actual = lista_actual->siguiente;
		printf("**********************\n\n");

	}
}



estado* ponerasociacion_1_svc(int id, char *clave, char *valor,  struct svc_req *rqstp){
static Estado result = OK;


	/****/printf("\n****************\n");
	/****/printf(" %s- Añadiendo (%d,%s,%s)...%s\n",YEL,id,clave,valor,RESET);

	Lista* lista_actual;
	int encontrado = 0;
	int inicializar = 0;
	int continuar;

	// La primera id
	if (lista_aso == 0){
		lista_aso = (Lista *) malloc (sizeof (Lista));
		/****/printf("%s****\nSe ha generado una nueva lista de id's\n****%s\n",BLU,RESET);

		// Nos situamos en la nueva
		lista_actual = lista_aso;

		// Necesario inicializar
		inicializar = 1;

		// Ya hemos encontrado donde insertar
		encontrado = 1;

	} else { // Existen id's ¿Alguna es igual? --> Hay que alcanzar la pos 

		// Al pirncipio de la lista que ya existía...
		lista_actual = lista_aso;

		do{

			continuar = 0;

			if(lista_actual->id == id){
				/****/printf(" %s-%s id %d conocida\n",BLU,RESET,id);
				encontrado = 1;
			} else if (lista_actual->siguiente != 0){
				lista_actual = lista_actual->siguiente;
				continuar = 1;
			} 


		} while(continuar == 1 && encontrado == 0);
	}


	// Si está aquí es porque:
	//	--- Encontrado donde insertar  --> encontrado == 1
	//  --- Posición adecuada para insertar  --> encontrado == 0

	// Hay que insertar
	// lista_actual apunta al final
	if (encontrado == 0){
		/****/printf("%s(!)%s id %d NO conocida\n",RED,RESET,id);
		/****/printf(" %s-%s Insertando al final de la lista...\n",BLU,RESET);

		lista_actual->siguiente = (Lista *) malloc (sizeof (Lista));

		// Nos situamos en la nueva
		lista_actual = lista_actual->siguiente;

		// Necesario inicializar
		inicializar = 1;
	}

	if (inicializar == 1){
		result = NO_ENCONTRADO;
		/****/printf(" %s-%s Creando id y tupla...\n",BLU,RESET);
		lista_actual->id = id;
		lista_actual->lista_tupla = (Tupla *) malloc (sizeof (Tupla));
		lista_actual->siguiente = 0;
		/**/
		lista_actual->lista_tupla->clave = (char *) malloc (sizeof(clave));
		strcpy(lista_actual->lista_tupla->clave, clave);

		lista_actual->lista_tupla->valor = (char *) malloc (sizeof(valor));
		strcpy(lista_actual->lista_tupla->valor, valor);

		lista_actual->lista_tupla->siguiente = 0;
		/****/printf("%s[*]%s Creada id y tupla\n\n",GRE,RESET);

	} else {

		// Estamos exactamente en la casilla en la que hay que insertar
		// si inicializar  == 0, es que la id ya existía y por lo tanto tenemos que 
		//    ver si la tupla ya está dentro antes de insertar


		Tupla* tupla_actual;
		encontrado = 0;
		tupla_actual = lista_actual->lista_tupla;

		// Buscamos si la clave ya existe
		do{
			continuar = 0;

			if(strcmp (tupla_actual->clave,clave) == 0){
				/****/printf(" %s-%s Clave \"%s\" conocida\n",BLU,RESET,clave);
				encontrado = 1;
			}

			if (tupla_actual->siguiente != 0){
				tupla_actual = tupla_actual->siguiente;
				continuar = 1;
			}
		}while(continuar == 1 && encontrado == 0);

		// Si estamos aquí:
		//	--- La clave no existía, y estamos al final
		//	--- La clave existe, y estamos en la posición para actualizar

		// La clave no existe
		if (encontrado == 0){

			result = NO_ENCONTRADO;

			/****/printf("%s(!)%s clave \"%s\" NO conocida\n",RED,RESET,clave);
			/****/printf(" %s-%s Insertando...\n",BLU,RESET);

			tupla_actual->siguiente = (Tupla *) malloc (sizeof (Tupla));

			tupla_actual = tupla_actual->siguiente;


			tupla_actual->clave = (char *) malloc (sizeof(clave));
			strcpy(tupla_actual->clave, clave);

			tupla_actual->valor = (char *) malloc (sizeof(valor));
			strcpy(tupla_actual->valor, valor);


			tupla_actual->siguiente = 0;

			/****/printf("%s[*]%s Tupla creada\n",GRE,RESET,clave);


		} else {
			/****/printf(" %s-%s Actualizando \"%s\"...\n",BLU,RESET,clave);

			result = SUSTITUCION;

			free(tupla_actual->valor);
			tupla_actual->valor = (char *) malloc (sizeof(valor));
			strcpy(tupla_actual->valor, valor);

			/****/printf("%s[*]%s Valor actualizado\n",GRE,RESET,clave);
		}


	}

	/****/printf("****************\n\n");

	/***/debug();

	return &result;
}



valor_estado* obtenerasociacion_1_svc(int id, char *clave,  struct svc_req *rqstp){
	static valor_estado result;


	/****/printf("\n****************\n");
	/****/printf(" %s- Obteniendo (%d,*%s*)...%s\n",YEL,id,clave,RESET);
	Lista* lista_actual = lista_aso;
	int encontrada_id = 0;
	int encontrada_clave = 0;
	int continuar = 0;

	while(lista_actual != 0 && encontrada_id == 0){

		if (lista_actual->id == id){
			encontrada_id = 1;

			Tupla* tupla_actual = lista_actual->lista_tupla;

			do{

				if(strcmp (tupla_actual->clave,clave) == 0){
					encontrada_clave = 1;
					result.Estado = OK;
					result.valor = (char *) malloc (sizeof(tupla_actual->valor));
					strcpy(result.valor, tupla_actual->valor);
					/****/printf(" %s-%s Coincidencia encontrada [%s] => %s\n",BLU,RESET,clave,tupla_actual->valor);


				}

				if (tupla_actual->siguiente != 0){
					tupla_actual = tupla_actual->siguiente;
					continuar = 1;
				} else
					continuar = 0;

			} while(encontrada_clave == 0 && continuar == 1);
		}

		lista_actual = lista_actual->siguiente;
	}

	if(encontrada_clave == 0){
		result.Estado = NO_ENCONTRADO;
		result.valor = (char *) malloc (0);
			/****/printf(" %s-%s No se han encontrado coincidencias para \"%s\"\n",BLU,RESET,clave);
	}

	return &result;
}

estado* borrarasociacion_1_svc(int id, char *clave,  struct svc_req *rqstp){
	static estado result;


	/****/printf("\n****************\n");
	/****/printf(" %s- Borrando (%d,*%s*)...%s\n",YEL,id,clave,RESET);


	Lista* lista_actual = lista_aso;
	Lista* lista_anterior = lista_aso;
	Tupla* tupla_actual;
	Tupla* tupla_anterior;

	int encontrada_id = 0;
	int encontrada_clave = 0;
	int continuar = 1;

	// Primer caso
	if (lista_actual != 0){

		if(lista_actual->id == id){
			tupla_actual = lista_actual->lista_tupla;
			tupla_anterior =  lista_actual->lista_tupla;

			// Primer caso
			if(strcmp (tupla_actual->clave,clave) == 0){
				encontrada_id = 1;
				continuar = 0;
				encontrada_clave = 1;
				/****/printf(" %s-%s Coincidencia encontrada [%s] => %s\n",BLU,RESET,clave,tupla_actual->valor);
				
				// Borramos, pero hay más tuplas en esa id
				if (tupla_actual->siguiente != 0){
					lista_actual->lista_tupla = tupla_actual->siguiente;
					free(tupla_actual);

				} else { // La id se queda vacía, hay que borrarla

					if (lista_actual->siguiente != 0){ // Hay más id's después
						lista_aso = lista_actual->siguiente;
						free(lista_actual);
					} else if(lista_actual == lista_aso){ //No hay más ids, ni antes ni después
						free(lista_aso);
						lista_aso = 0;
					} else { // No hay más ids después
						lista_anterior->siguiente = 0;
						free(lista_actual);
					}
				}

			}

			// add
			// getall
			// remove 
			// getall ---> Error


			if(encontrada_id == 0 && tupla_actual->siguiente != 0)
				tupla_actual = tupla_actual->siguiente;

			// Resto de los casos
			while(encontrada_id == 0 && continuar == 1) {

				if(strcmp (tupla_actual->clave,clave) == 0){
					encontrada_id = 1;
					/****/printf(" %s-%s Coincidencia encontrada [%s] => %s\n",BLU,RESET,clave,tupla_actual->valor);
					
					// Borramos, pero hay más tuplas después
					if (tupla_actual->siguiente != 0){
						tupla_anterior->siguiente = tupla_actual->siguiente;
						free(tupla_actual);
					} else { // Borramos, pero hay más tuplas antes
						tupla_anterior->siguiente = 0;
						free(tupla_actual);
					}
				}

				if (tupla_actual->siguiente != 0 && encontrada_id == 0){
					tupla_anterior = tupla_anterior->siguiente;
					tupla_actual = tupla_actual->siguiente;
				} else {
					continuar = 0;
				}

			} 

		}

	}


	if (lista_actual != 0 && encontrada_id == 0)
		lista_actual = lista_actual->siguiente;

	// Resto
	while(lista_actual != 0 && encontrada_clave == 0){


		if (lista_actual->id == id){

			tupla_actual = lista_actual->lista_tupla;
			tupla_anterior =  lista_actual->lista_tupla;

			// Primer caso
			if(strcmp (tupla_actual->clave,clave) == 0){
				encontrada_clave = 1;
				/****/printf(" %s-%s Coincidencia encontrada [%s] => %s\n",BLU,RESET,clave,tupla_actual->valor);
				
				// Borramos, pero hay más tuplas en esa id
				if (tupla_actual->siguiente != 0){
					lista_actual->lista_tupla = tupla_actual->siguiente;
					free(tupla_actual);

				} else { // La id se queda vacía, hay que borrarla

					if (lista_actual->siguiente != 0){ // Hay más id's después
						lista_anterior->siguiente = lista_actual->siguiente;
						free(lista_actual);
					} else if(lista_actual == lista_aso){ //No hay más ids, ni antes ni después
						lista_aso = 0;
						free(lista_actual);
					} else { // No hay más ids después
						lista_anterior->siguiente = 0;
						free(lista_actual);
					}
				}

			}

			if(encontrada_clave == 0 && tupla_actual->siguiente != 0)
				tupla_actual = tupla_actual->siguiente;

			// Resto de los casos
			while(encontrada_clave == 0 && continuar == 1) {

				if(strcmp (tupla_actual->clave,clave) == 0){
					encontrada_clave = 1;
					/****/printf(" %s-%s Coincidencia encontrada [%s] => %s\n",BLU,RESET,clave,tupla_actual->valor);
					
					// Borramos, pero hay más tuplas después
					if (tupla_actual->siguiente != 0){
						tupla_anterior->siguiente = tupla_actual->siguiente;
						free(tupla_actual);
					} else { // Borramos, pero hay más tuplas antes
						tupla_anterior->siguiente = 0;
						free(tupla_actual);
					}
				}

				if (tupla_actual->siguiente != 0 && encontrada_clave == 0){
					tupla_anterior = tupla_anterior->siguiente;
					tupla_actual = tupla_actual->siguiente;
				} else {
					continuar = 0;
				}

			} 
		}


		lista_actual = lista_actual->siguiente;
		lista_anterior = lista_anterior->siguiente;
	}

	if(encontrada_clave == 0 && encontrada_id == 0){
		result = NO_ENCONTRADO;
			/****/printf(" %s-%s No se han encontrado coincidencias para \"%s\"\n",BLU,RESET,clave);
	} else {
		result = OK;
		/****/printf("%s[*]%s Valor borrado\n",GRE,RESET);
	}

	debug();

	return &result;
}

enumerados* enumerar_1_svc(int id,  struct svc_req *rqstp){
	static enumerados result;

	int encontrado = 0;
	int continuar = 0;

	/****/printf("\n****************\n");
	/****/printf(" %s- Obteniendo (%d,*,*)...%s\n",YEL,id,RESET);


	Lista* lista_actual = lista_aso;

	result.tupla.tupla_val = 0;
	result.tupla.tupla_len = 0;
	
	while(lista_actual != 0 && encontrado == 0){


		if (lista_actual->id == id){


			encontrado = 1;

			result.tupla.tupla_val = (tupla_unica*) malloc (sizeof(tupla_unica));
			result.tupla.tupla_len = 1;


			// Inicializamos, vamos a guardar tuplas

			Tupla* tupla_actual_leyendo = lista_actual->lista_tupla;

			do{

				/****/printf(" %s(!)%s Clave \"%s\" encontrada\n",GRE,RESET,tupla_actual_leyendo->clave);	

				result.tupla.tupla_val[result.tupla.tupla_len-1].clave = (char *) malloc (sizeof(tupla_actual_leyendo->clave));
				strcpy(result.tupla.tupla_val[result.tupla.tupla_len-1].clave, tupla_actual_leyendo->clave);
				
				result.tupla.tupla_val[result.tupla.tupla_len-1].valor = (char *) malloc (sizeof(tupla_actual_leyendo->valor));
				strcpy(result.tupla.tupla_val[result.tupla.tupla_len-1].valor, tupla_actual_leyendo->valor);
			

				if (tupla_actual_leyendo->siguiente != 0){

					continuar = 1;

					result.tupla.tupla_len++;
					
					result.tupla.tupla_val = (tupla_unica*) realloc (result.tupla.tupla_val, (result.tupla.tupla_len)*sizeof(tupla_unica) ); 

					tupla_actual_leyendo = tupla_actual_leyendo->siguiente;

				} else {
					continuar = 0;
			
				}

			} while(continuar == 1);
		}
				
		lista_actual = lista_actual->siguiente;
	}

	// Estado
	if(encontrado == 0){
		result.Estado = NO_ENCONTRADO;
			/****/printf(" %s-%s No se han encontrado coincidencias\n",BLU,RESET);
	} else {
		result.Estado = OK;
			/****/printf(" %s-%s Se han obtenido resultados para %d\n",BLU,RESET,id);
	}

	debug();

	return &result;
}