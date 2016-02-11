/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "airports.h"
#include "kdtree.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>


#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
airportsprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		struct coordinates get_airports_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case get_airports:
		_xdr_argument = (xdrproc_t) xdr_coordinates;
		_xdr_result = (xdrproc_t) xdr_readairports_ret;
		local = (char *(*)(char *, struct svc_req *)) get_airports_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

//===========================ADDED CODE=============================

static const char filename[] = "airport-locations.txt";
struct kdtree *kd;

/*// struct to store place (line in text file)
struct airport {
  char code[255];
  char name[255];
  char state[2];
  float latitude;
  float longitude;
};

typedef struct airport Airport;
*/

char *trim(char *s)
{
  while(isspace(*s)) s++;
  s[strlen(s) - 1] = '\0';
  return s;
}


// This function parses the line that is reas and stores data in the "airportnode" struct
Airport *  parseLine(char * line){

  Airport *a = malloc(sizeof *a);
  int length = strlen(line);
  char temp[255];
  
  //code
  strncpy(a->code, line+1, 3);  // code

  //state
  strncpy(a->name, line+19, length-19+3);
  strncpy(a->name, trim(a->name), strlen(a->name)); // trimming spaces

  //latitiude
  strncpy(temp, line+6, 5);
  a->latitude = atof(temp);

  //longitude
  strncpy(temp, line+12, 7);
  a->longitude = atof(temp);

  // Uncomment code below if you want to see the data being printed out

  /*
    printf("%s\n",a->code);
    printf("%s\n",a->name);
	printf("%.6f",a->latitude);
	printf("%.6f\n",a->longitude);
  */
  return a;

};

// Function to read file airport-locations.txt
void readFile() {
  FILE *file = fopen(filename, "r");
  char line[255]; //temp storage for line
  if (file != NULL) {
	fgets(line, sizeof line, file); // disregard first line
	kd_create(2);
	while (fgets(line, sizeof line, file) != NULL) { //read line
	  // parse line and get data in struct
	  if (strchr(line, ',')){
		Airport *a = parseLine(line);
		float coords[] = {a->latitude, a->longitude};
		kd_insertf(kd, coords, a);
		
		//printf("Added: %s, %s:, lat:%f lon:%f\n", a->code, a->name, a->latitude, a->longitude);
		//tree = kd_create(2);
		//assert(0==kd_insert3(tree, a->latitude, a->longitude, 0.0, a));

	  }
	  // TODO: Use Airport struct store in KDTREE

	}
	fclose(file);
  }

  /* struct place {
	float latitude;
	float longitude;
	float blank;
  };

  struct kdres *result;
  
  Airport *a = malloc(sizeof *a);
  a->latitude = 47.626353;
  a->longitude = -122.333144;
  
  double radius = 0.1;
  result = kd_nearest_range(tree, a,radius);

  Airport *b = malloc(sizeof *b);
  double position[3];
  b = (Airport *)kd_res_item(result,position);
  

  printf("%s\n",b->code);
  printf("%s\n",b->name);
  printf("%.6f",b->latitude);
  printf("%.6f\n",b->longitude);
  */
  
}


int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (AIRPORTSPROG, AIRPORTS_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, AIRPORTSPROG, AIRPORTS_VERS, airportsprog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (AIRPORTSPROG, AIRPORTS_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, AIRPORTSPROG, AIRPORTS_VERS, airportsprog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (AIRPORTSPROG, AIRPORTS_VERS, tcp).");
		exit(1);
	}
	kd = kd_create(2);
	readFile();
	printf("Created KDTree\n");
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
