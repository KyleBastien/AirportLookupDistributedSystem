/*
  Name: Hesham Alsaeedi, Kyle McNutt, Matt Smith, Craig Colomb
  Date: February 15, 2016
  Project Name: Group Project (places_server.c)

  Program Description: This program does a unique search of the users input of city and state. If found,
  it contacts the airports_server to retrieve the 5 nearest neighbors to that city's latitude and longitude.
*/

#include "placesairports.h"
#include "trie.h"

airportslist airportsprog_1(coordinates * coord, char *host)
{
  CLIENT *clnt;
  readairports_ret  *result_1;
  struct coordinates  get_airports_1_arg;
  airportslist list;
  
#ifndef DEBUG
  clnt = clnt_create (host, AIRPORTSPROG, AIRPORTS_VERS, "udp");
  if (clnt == NULL) {
	clnt_pcreateerror (host);
	exit (1);
  }
#endif  /* DEBUG */

  result_1 = get_airports_1(coord, clnt);
  if (result_1 == (readairports_ret *) NULL) {
	clnt_perror (clnt, "call failed");
  }
  
  list = result_1->readairports_ret_u.list;
    
  return list;
  
#ifndef DEBUG
  clnt_destroy (clnt);
#endif   /* DEBUG */
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function takes in the state and city and searches the Trie
//:: structure for the location using the possibility of partial
//:: matches. Once the place is found it's latitude and longitude
//:: are sent to the airport_server. The nearest airports are then
//:: returned and then we build a list of the nearest aiport's info
//:: so we can return it back to the client.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

readplaces_ret *
get_places_1_svc(struct location *argp, struct svc_req *rqstp)
{
	static readplaces_ret  result;
	trieNode_t *search_result;
	char merge[255];
	char *found_state;
	char *found_name;
	int i = 0;
		
	// Merge state and city name into a single string (state|city) then convert whole string to lower case
	strcpy(merge, argp->state);
	strcat(merge, argp->city);
	for (i = 0; merge[i]; i++)
	{
	  merge[i] = tolower(merge[i]);
	}

	// Search Trie for place match, allow patial matches
	search_result = TrieSearchPartial(root->children,merge);

	xdr_free((xdrproc_t)xdr_readplaces_ret, (char *)&result);	


	airportslist al;
    placeslist head = NULL;
	placesnode * temp;	
	placeslist * list;

	// found places now make a call to airport_server
	if (search_result) {
	  coordinates *coord = malloc(sizeof *coord);
	  coord->lattitude = search_result->latitude;
	  coord->longitude = search_result->longitude;

	  strncpy(found_state, search_result->entry, 2);
	  strncpy(found_name, search_result->entry+2, strlen(search_result->entry));

	  // get nearest airports
	  al = airportsprog_1(coord, argp->host);
	  
	  // Build list of nearest airport information
	  while (al != NULL) {
		temp = (placesnode *)malloc(sizeof(placesnode));
		temp->code = strdup(al->code);
		temp->name = strdup(al->name);
		temp->state = strdup(al->state);
		temp->distance = al->distance;
		if (head == NULL) {
		  head = temp;
		  head->next = NULL;
		} else {
		  temp->next = head;
		  head = temp;
		}
		al = al->next;
	  }

	  // Add found city/state to list
	  temp = (placesnode *)malloc(sizeof(placesnode));
	  temp->state = strdup(found_state);
	  temp->name = strdup(found_name);
	  temp->code = strdup("");
	  temp->latitude = search_result->latitude;
	  temp->longitude = search_result->longitude;
	  temp->next = NULL;
	  if (head == NULL) {
		head = temp;
		head->next = NULL;
	  } else {
		temp->next = head;
		head = temp;
	  }
	  
	  free(coord);
	  list = &result.readplaces_ret_u.list;
	  *list = head;
	}
	
	return &result;
}
