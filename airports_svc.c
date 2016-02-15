/*
  Name: Hesham Alsaeedi, Kyle McNutt, Matt Smith, Craig Colomb
  Date: February 15, 2016
  Project Name: Group Project (airports_svc.c)

  Program Description: This program stores the airport information from airport-locations.txt into a KD
  Tree data structure. It stores the airport code, latitude, longitude, city, and state name.
*/


#include "placesairports.h"
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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function trims a whitespace off a line read in from a file.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

char *trim(char *str)
{
  char *endl;

  // Trim leading space
  while(isspace(*str))
	str++;

  // Trim trailing space
  endl = str + strlen(str) - 1;
  while(endl > str && isspace(*endl)) endl--;

  // Null it out
  *(endl+1) = 0;

  return str;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function parses the line that is read and stored in the
//:: "airportnode" struct.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Airport *  parseLine(char * line)
{
  Airport *a = malloc(sizeof *a);
  int length = strlen(line);
  char temp[255];

  //code
  strncpy(a->code, line+1, 3);

  //latitiude
  strncpy(temp, line+6, 5);
  a->latitude = atof(temp);

  //longitude
  strncpy(temp, line+12, 7);
  a->longitude = atof(temp);

  //city
  strncpy(temp, trim(line), length);
  strncpy(temp, trim(temp + 19), length);
  temp[strlen(temp) - 3] = '\0';
  strcpy(a->name, temp);
  
  //state
  strncpy(temp, line+length-3, 3);
  strncpy(a->state, temp, 3);
  

  return a;
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: Function to read the file airport-locations.txt and add the info
//:: to the KD-Tree.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void readFile() {
  FILE *file = fopen(filename, "r");
  char line[255]; //temp storage for line
  if (file != NULL) {
	fgets(line, sizeof line, file); // disregard first line
	kd_create(2);
	while (fgets(line, sizeof line, file) != NULL) { //read line
	  // parse line and get data in struct
	  if (strchr(line, ',')) {
		Airport *a = parseLine(line);
		float coords[] = {a->latitude, a->longitude};
		kd_insertf(kd, coords, a);
	  }
	}
	fclose(file);
  }
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
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
