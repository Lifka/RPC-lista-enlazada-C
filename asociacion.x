struct tupla{
	string clave<>;
	string valor<>;
	struct tupla* siguiente;
};

struct lista{
	int id;
	struct tupla* lista_tupla;
	struct lista* siguiente;
};

enum estado{	
	OK = 0,
	SUSTITUCION = 1,
	NO_ENCONTRADO = 2
};


struct valor_estado{
	string valor<>;
	estado Estado;
};

struct tupla_unica{
	string clave<>;
	string valor<>;
};

struct enumerados{
	tupla_unica tupla<>;
	estado Estado;
};

program ASOCIACION {
	version ASOCIACION_1 {
		estado ponerAsociacion(int id, string clave<>, string valor<>) = 1;
		valor_estado obtenerAsociacion(int id, string clave<>) = 2;
		estado borrarAsociacion(int id, string clave<>) = 3;
		enumerados enumerar(int id) = 4;
	} = 1;
} = 0x20000157;