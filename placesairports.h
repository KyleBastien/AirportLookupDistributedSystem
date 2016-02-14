/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PLACES_H_RPCGEN
#define _PLACES_H_RPCGEN

//#ifndef _AIRPORTS_H_RPCGEN
//#define _AIRPORTS_H_RPCGEN

#include <rpc/rpc.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct placesnode *placeslist;

  struct placesnode {
	char *code;
	char *name;
	char *state;
	float latitude;
	float longitude;
	float distance;
	placeslist next;
  };
  typedef struct placesnode placesnode;

  struct location {
	char *city;
	char *state;
	char *host;
  };
  typedef struct location location;

  struct readplaces_ret {
	int err;
	union {
	  placeslist list;
	} readplaces_ret_u;
  };
  typedef struct readplaces_ret readplaces_ret;

  typedef struct airportsnode *airportslist;
  struct kdtree *kd;

  struct airportsnode {
	char *code;
	char *name;
	char *state;
	float distance;
	airportslist next;
  };
  
  typedef struct airportsnode airportsnode;

  // struct to store place (line in text file)
  struct airport {
	char code[255];
	char name[255];
	char state[2];
	float latitude;
	float longitude;
	float dist;
  };

  typedef struct airport Airport;

  struct coordinates {
	float lattitude;
	float longitude;
  };
  typedef struct coordinates coordinates;

  struct readairports_ret {
	int err;
	union {
	  airportslist list;
	} readairports_ret_u;
  };
  typedef struct readairports_ret readairports_ret;

  
#define PLACESPROG 0x3777777D
#define PLACES_VERS 1

#define AIRPORTSPROG 0x37777783
#define AIRPORTS_VERS 1
  
#if defined(__STDC__) || defined(__cplusplus)
#define get_places 1
  extern  readplaces_ret * get_places_1(struct location *, CLIENT *);
  extern  readplaces_ret * get_places_1_svc(struct location *, struct svc_req *);
  extern int placesprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define get_places 1
  extern  readplaces_ret * get_places_1();
  extern  readplaces_ret * get_places_1_svc();
  extern int placesprog_1_freeresult ();
#endif /* K&R C */
  
#if defined(__STDC__) || defined(__cplusplus)
#define get_airports 1
  extern  readairports_ret * get_airports_1(struct coordinates *, CLIENT *);
  extern  readairports_ret * get_airports_1_svc(struct coordinates *, struct svc_req *);
  extern int airportsprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);
#else /* K&R C */
#define get_airports 1
  extern  readairports_ret * get_airports_1();
  extern  readairports_ret * get_airports_1_svc();
  extern int airportsprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
  extern  bool_t xdr_placeslist (XDR *, placeslist*);
  extern  bool_t xdr_placesnode (XDR *, placesnode*);
  extern  bool_t xdr_location (XDR *, location*);
  extern  bool_t xdr_readplaces_ret (XDR *, readplaces_ret*);
  extern  bool_t xdr_airportslist (XDR *, airportslist*);
  extern  bool_t xdr_airportsnode (XDR *, airportsnode*);
  extern  bool_t xdr_coordinates (XDR *, coordinates*);
  extern  bool_t xdr_readairports_ret (XDR *, readairports_ret*);
   
#else /* K&R C */
  extern bool_t xdr_placeslist ();
  extern bool_t xdr_placesnode ();
  extern bool_t xdr_location ();
  extern bool_t xdr_readplaces_ret ();
  extern bool_t xdr_airportslist ();
  extern bool_t xdr_airportsnode ();
  extern bool_t xdr_coordinates ();
  extern bool_t xdr_readairports_ret ();
#endif /* K&R C */
  
#ifdef __cplusplus
}
#endif

#endif /* !_PLACES_H_RPCGEN */