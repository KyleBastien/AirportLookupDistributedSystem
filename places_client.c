/*
  Name: Hesham Alsaeedi, Kyle McNutt, Matt Smith, Craig Colomb
  Date: February 15, 2016
  Project Name: Group Project (places_client.c)

  Program Description: This program is the client program for the RPC. It takes in user input of
  the server name, a city, and a state a path. The server name is used to initilize the client RPC by making
  a call to the client stub which marshals the given city and state into a network message.
  If the city and state are found, the client will print each airport code, name, and ditance of the 5 closes
  neighbors.
*/


#include "placesairports.h"
#include "math.h"
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: 
//:: This function makes a call out to places in order to get back 
//:: a list of the five nearest airports and their information. It 
//:: prints the airport code, name, state, and distance from our 
//:: input location. 
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: 

void
placesprog_1(char *host, struct location * input)
{
	CLIENT *clnt;
	readplaces_ret  *result_1;
	struct location  get_places_1_arg;
	placeslist list;

#ifndef	DEBUG
	clnt = clnt_create (host, PLACESPROG, PLACES_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = get_places_1(input, clnt);
	if (result_1 == (readplaces_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	list = result_1->readplaces_ret_u.list;

	if (list) {
		printf("%s, %s: %f, %f\n", list->name, list->state, list->latitude, list->longitude);
		list = list->next;
	} else {
		printf("Sorry, we couldn't locate that city, please try again.\n");
	}

	while (list != NULL) {
	  printf("code=%s, name=%s, state=%s, distance:%.2f miles\n", list->code, list->name, list->state, roundf(list->distance*100)/100);
	  list = list->next;
	}	
	xdr_free((xdrproc_t)xdr_readplaces_ret, (char*)result_1);	

	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
  char *host;
  struct location * input = malloc(sizeof *input);

  if (argc < 4) {
	printf ("Please input 3 arguments (host, city, state)\n");
	exit (1);
  }
  input->host = argv[1];
  input->city = argv[2];
  input->state = argv[3];
  host = argv[1];

  placesprog_1 (host, input);
  
  exit (0);
}

