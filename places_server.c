/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "places.h"
#include "airports.h"
#include "trie.h"

struct coordinates *coord; 

void
airportsprog_1(struct location * argp)
{
  CLIENT *clnt;
  readairports_ret  *result_1;
  struct coordinates  get_aiports_1_arg;

#ifndef DEBUG
  clnt = clnt_create (argp->host, AIRPORTSPROG, AIRPORTS_VERS, "udp");
  if (clnt == NULL) {
	clnt_pcreateerror (argp->host);
	exit (1);
  }
#endif  /* DEBUG */

  //TODO: pass lat/long in get_airports_1 function.
  //      for now, there is a placeholder called coord (struct defined above)
  // result_1 = get_aiports_1(coord, clnt);
  if (result_1 == (readairports_ret *) NULL) {
	clnt_perror (clnt, "call failed");
  }
#ifndef DEBUG
  clnt_destroy (clnt);
#endif   /* DEBUG */
}

// TODO: This function gets the location (lat, long) from the datastructure
readplaces_ret *
get_places_1_svc(struct location *argp, struct svc_req *rqstp)
{
  static readplaces_ret  result;

  /*
   * insert server code here
   */
  
  //TODO: Call airportsprog_1 from here to pass lat/long.
  airportsprog_1(argp);
  return &result;
}
