/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _ASOCIACION_H_RPCGEN
#define _ASOCIACION_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct tupla {
	char *clave;
	char *valor;
	struct tupla *siguiente;
};
typedef struct tupla tupla;

struct lista {
	int id;
	struct tupla *lista_tupla;
	struct lista *siguiente;
};
typedef struct lista lista;

enum estado {
	OK = 0,
	SUSTITUCION = 1,
	NO_ENCONTRADO = 2,
};
typedef enum estado estado;

struct valor_estado {
	char *valor;
	estado Estado;
};
typedef struct valor_estado valor_estado;

struct tupla_unica {
	char *clave;
	char *valor;
};
typedef struct tupla_unica tupla_unica;

struct enumerados {
	struct {
		u_int tupla_len;
		tupla_unica *tupla_val;
	} tupla;
	estado Estado;
};
typedef struct enumerados enumerados;

struct ponerasociacion_1_argument {
	int id;
	char *clave;
	char *valor;
};
typedef struct ponerasociacion_1_argument ponerasociacion_1_argument;

struct obtenerasociacion_1_argument {
	int id;
	char *clave;
};
typedef struct obtenerasociacion_1_argument obtenerasociacion_1_argument;

struct borrarasociacion_1_argument {
	int id;
	char *clave;
};
typedef struct borrarasociacion_1_argument borrarasociacion_1_argument;

#define ASOCIACION 0x20000157
#define ASOCIACION_1 1

#if defined(__STDC__) || defined(__cplusplus)
#define ponerAsociacion 1
extern  estado * ponerasociacion_1(int , char *, char *, CLIENT *);
extern  estado * ponerasociacion_1_svc(int , char *, char *, struct svc_req *);
#define obtenerAsociacion 2
extern  valor_estado * obtenerasociacion_1(int , char *, CLIENT *);
extern  valor_estado * obtenerasociacion_1_svc(int , char *, struct svc_req *);
#define borrarAsociacion 3
extern  estado * borrarasociacion_1(int , char *, CLIENT *);
extern  estado * borrarasociacion_1_svc(int , char *, struct svc_req *);
#define enumerar 4
extern  enumerados * enumerar_1(int , CLIENT *);
extern  enumerados * enumerar_1_svc(int , struct svc_req *);
extern int asociacion_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ponerAsociacion 1
extern  estado * ponerasociacion_1();
extern  estado * ponerasociacion_1_svc();
#define obtenerAsociacion 2
extern  valor_estado * obtenerasociacion_1();
extern  valor_estado * obtenerasociacion_1_svc();
#define borrarAsociacion 3
extern  estado * borrarasociacion_1();
extern  estado * borrarasociacion_1_svc();
#define enumerar 4
extern  enumerados * enumerar_1();
extern  enumerados * enumerar_1_svc();
extern int asociacion_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_tupla (XDR *, tupla*);
extern  bool_t xdr_lista (XDR *, lista*);
extern  bool_t xdr_estado (XDR *, estado*);
extern  bool_t xdr_valor_estado (XDR *, valor_estado*);
extern  bool_t xdr_tupla_unica (XDR *, tupla_unica*);
extern  bool_t xdr_enumerados (XDR *, enumerados*);
extern  bool_t xdr_ponerasociacion_1_argument (XDR *, ponerasociacion_1_argument*);
extern  bool_t xdr_obtenerasociacion_1_argument (XDR *, obtenerasociacion_1_argument*);
extern  bool_t xdr_borrarasociacion_1_argument (XDR *, borrarasociacion_1_argument*);

#else /* K&R C */
extern bool_t xdr_tupla ();
extern bool_t xdr_lista ();
extern bool_t xdr_estado ();
extern bool_t xdr_valor_estado ();
extern bool_t xdr_tupla_unica ();
extern bool_t xdr_enumerados ();
extern bool_t xdr_ponerasociacion_1_argument ();
extern bool_t xdr_obtenerasociacion_1_argument ();
extern bool_t xdr_borrarasociacion_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_ASOCIACION_H_RPCGEN */