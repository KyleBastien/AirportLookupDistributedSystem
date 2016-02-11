/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "places.h"

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

	while (list != NULL) {
	  printf("%s\n", list->name);
	  printf("%f\n", list->distance);
	  list = list->next;
	}
	

	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
  /*char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	placesprog_1 (host);
  */
  char *host;
  struct location * input = malloc(sizeof *input);


  if (argc < 3) {
	printf ("usage: %s server_host\n", argv[0]);
	exit (1);
  }
  input->host = argv[1];
  input->city = argv[2];
  input->state = argv[3];
  host = argv[1];

  placesprog_1 (host, input);
  
exit (0);
}
