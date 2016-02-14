//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This is the server portion for airports which takes in the
//:: latitutde and longitude of our search origin and then finds
//:: the five nearest airpots to this search origin. This server
//:: is called by the places_server.c.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <math.h>
#include "placesairports.h"
#include "kdtree.h"

#define pi 3.14159265358979323846

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function converts decimal degrees to radians.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double deg2rad(double deg) {
  return (deg * pi / 180);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function converts radians to decimal degrees.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double rad2deg(double rad) {
  return (rad * 180 / pi);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function calculates the distance between two longitude and
//:: latitutde points.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1))* sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch (unit) {
  case 'M': break;
  case 'K':
	dist = dist * 1.609344;
	break;
  case 'N':
	dist = dist * 0.8684;
	break;
  }
  return (dist);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function takes in our search origin and uses the KD-Tree's
//:: nearest neighbor search in order to find the closest airports
//:: to that search origin. If our first range isn't producing enough
//:: result we increase our range until we get at least the number of
//:: results we need. We then loop through each result and calculate
//:: the distance between our search origin and that airport. All of
//:: results are then sorted using insertion sort by the distance
//:: between the aiport result and the search origin. The top results
//:: are then put into a linked list and returned out.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

airportslist sortAndAddAirports(int countRequired, float searchOrigin[]) {
  // first populate an array with results
  int countFound = 0;
  float range = .5;
  struct kdres *rangeSearchResult = kd_nearest_rangef(kd, searchOrigin, range);
  while (kd_res_size(rangeSearchResult) < countRequired) {
	range += .5;
	rangeSearchResult = kd_nearest_rangef(kd, searchOrigin, range);
  }

  int rangeSize = kd_res_size(rangeSearchResult);
  Airport airports[rangeSize];
  double position[2];
  int i;
  for (i = 0; i < rangeSize; i++) {
	// get the data and position of the current result item
	Airport * airport = (Airport*) kd_res_item(rangeSearchResult, position);

	// compute the distance to the current result
	// set the calculated distance to the airport struct
	double dist = distance(position[0], position[1], searchOrigin[0], searchOrigin[1], 'M');
	airport->dist = dist;

	airports[i] = *airport;

	// go to the next entry
	kd_res_next(rangeSearchResult);
  }
  // insertion sort of array
  for (i = 1; i < rangeSize; i++) {
	int iDoSwap = 1;
	int j;
	for (j = i; j > 0 && iDoSwap; j--) {
      if(airports[j].dist > airports[j-1].dist ) iDoSwap = 0;
      if (iDoSwap) {
          Airport temp;
          temp = airports[j];
          airports[j] = airports[j - 1];
          airports[j - 1] = temp;
      }
	}
  }
  
  // inserting into list
  airportslist head = NULL;
  airportsnode *temp;
    
  for (i=0; i<countRequired; i++) {
	temp = (airportsnode *)malloc(sizeof(airportsnode));
	temp->code = airports[i].code;
	temp->name = airports[i].name;
	temp->state = airports[i].state;
	temp->distance = airports[i].dist;
	if (head == NULL) {
	  head = temp;
	  head->next = NULL;
	} else {
	  temp->next = head;
	  head = temp;
	}	
  }
  return head;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function is called by the places_server.c in order to find
//:: the 5 nearest airports to the longitude and latitutde coordinates
//:: passed to it.
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

readairports_ret *
get_airports_1_svc(struct coordinates *argp, struct svc_req *rqstp)
{
  static readairports_ret  result;

  //xdr_free((xdrproc_t)xdr_readairports_ret, (char *)&result);

  airportslist list;
  airportslist * head;
  airportslist temp = list;
  float coords[2] = {argp->lattitude, argp->longitude};

  list = sortAndAddAirports(5, coords);
  
  head = &result.readairports_ret_u.list;
  *head = list;   
  
  return &result;
}
