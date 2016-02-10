/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: cc
 *
 * Created on February 8, 2016, 9:32 AM
 */


#include "kdtree.h"
#include <stdio.h>
#include <stdlib.h>
//#include <../rpc/map_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>


#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif



//===========================ADDED CODE=============================
//__NetBeans somehow does not recognize MAXLEN...cc
#define MAXLEN 200
static const char filename[] = "airport-locations.txt";
static struct kdtree *kd;

#include <math.h>
#define pi 3.14159265358979323846

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function converts decimal degrees to radians:
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double deg2rad(double deg) {
    return (deg * pi / 180);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: This function converts radians to decimal degrees: 
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
double rad2deg(double rad) {
    return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
    double theta, dist;
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
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

typedef struct {
    char code[3];
    char name[MAXLEN];
    char state[2];
    float latitude;
    float longitude;
    float dist;
} Airport;

typedef struct {
	u_int airportsnum_len;
	float *airportsnum_val;
} airportsnum;

typedef struct airportsnode *airportslist;
typedef char *airportstype;
typedef struct  {
	airportstype code;
	airportstype name;
	airportstype state;
	airportsnum distance;
	airportslist next;
}airportsnode;


//___insertion sort of array
void sortAirportsByDistance(Airport airports[], int size) {
    int i;
    for (i = 1; i < size; i++) {
        int iDoSwap = 1;
        int j;
        for (j = i; j > 0 && iDoSwap; j--) 
        {
            if( airports[j].dist > airports[-1].dist ) iDoSwap = 0;
            if (iDoSwap) {
                Airport temp;
                temp = airports[j];
                airports[j] = airports[j - 1];
                airports[j - 1] = temp;
            }
        }
    }
}


airportslist sortAndAddAirports(int countRequired, float searchOrigin[]) {
    //____first populate an array with results

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
        /* get the data and position of the current result item */
        Airport * airport = (Airport*) kd_res_item(rangeSearchResult, position);

        /* compute the distance to the current result */
        //set the calculated distance to the airport struct
        double dist = distance(position[0], position[1], searchOrigin[0], searchOrigin[1], 'M');
        airport->dist = dist;
        
        airports[i] = *airport;

        /* go to the next entry */
        kd_res_next(rangeSearchResult);
    }

    for (i = 1; i < rangeSize; i++) {
        int iDoSwap = 1;
        int j;
        for (j = i; j > 0 && iDoSwap; j--) 
        {
            if( airports[j].dist > airports[j-1].dist ) iDoSwap = 0;
            if (iDoSwap) {
                Airport temp;
                temp = airports[j];
                airports[j] = airports[j - 1];
                airports[j - 1] = temp;
            }
        }
    }
    //sortAirportsByDistance(airports, rangeSize);
    airportslist result = malloc(sizeof(airportslist));
    
    //*result = iterator;
    for (i=0; i<countRequired; i++)
    {
        // need to fill result from sorted array here
    }
    return result;
}


char *trim(char *s) {
    while (isspace(*s)) s++;
    s[strlen(s) - 1] = '\0';
    return s;
}

// This function parses the line that is read and stores data in the "airport" struct

Airport * parseLine(char * line) {

    Airport *a = malloc(sizeof *a);
    int length = strlen(line);
    char temp[MAXLEN];

    //code
    strncpy(a->code, line + 1, 3); // code

    //state
    strncpy(a->name, line + 19, length - 19 + 3);
    strncpy(a->name, trim(a->name), strlen(a->name)); // trimming spaces

    //latitiude
    strncpy(temp, line + 6, 5);
    a->latitude = atof(temp);

    //longitude
    strncpy(temp, line + 12, 7);
    a->longitude = atof(temp);


    // Uncomment code below if you want to see the data being printed out

    /*
    printf(a->code);
    printf(a->name);
    printf("%.6f",a->latitude);
    printf("%.6f\n",a->longitude);
     */
    return a;

};

// Function to read file airport-locations.txt

void readFile() {
    FILE *file = fopen(filename, "r");
    char line[MAXLEN]; //temp storage for line
    //int count = 0;
    if (file != NULL) {
        fgets(line, sizeof line, file); // disregard first line
        while (fgets(line, sizeof line, file) != NULL) { //read line
            //printf("working on line # %i\n",count++);
            // parse line and get data in struct
            if (strchr(line, ',')) {
                Airport *a = parseLine(line);
                float coords[] = {a->latitude, a->longitude};

                kd_insertf(kd, coords, a);
            }
            // TODO: Use Airport struct store in datastructure

        }
        fclose(file);
    }
}

void printResults(struct kdres *results, double origin[]) {
    double position[2];

    while (!kd_res_end(results)) {
        /* get the data and position of the current result item */
        Airport * airport = (Airport*) kd_res_item(results, position);

        /* compute the distance of the current result from the pt */
        //#### ToDo

        /* print out the retrieved data */

        double dist = distance(position[0], position[1], origin[0], origin[1], 'M');
        printf("Code %s, Name %s, Distance %.3f\n", *airport->code, *airport->name, dist);

        /* go to the next entry */
        kd_res_next(results);
    }
}

int main(int argc, char **argv) {
    /*
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
     */

    kd = kd_create(2);
    readFile(); //kd);
    //svc_run ();
    float coords[2] = {40.0f, -80.0f};
    sortAndAddAirports(5, coords);
    double origin[2] = {40.0, -80.0};
    struct kdres *searchResults = kd_nearest_rangef(kd, coords, 1.0f);
    printf("found %d results:\n", kd_res_size(searchResults));

    printResults(searchResults, origin);


    exit(1);
    /* NOTREACHED */
}


