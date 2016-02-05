# AirportLookupDistributedSystem
A Distributed System for looking up the Five Nearest Airport given a City and State. For SEGR-5520.

##Activity List

####places_client.c

- [ ] Read arguments in main
- [ ] Call placesprog_1 function to connect to places_server.c (server) and pass lat/long arguments
- [ ] This returns back a placeslist (defined in places.x)
- [ ] Each node in the list has a code, name, state, distance, and *next.
- [ ] Print out the list

####places_svc.c

- [ ] Add code to do the following:
- [x] Read places2k.txt
- [x] Parse the line that is read and store in a place structure
- [ ] Create prefix tree
- [ ] Insert place in prefix tree

 

####places_server.c

- [ ] Search for city and state in prefix tree
- [ ] Get lat/long information from prefix tree
- [ ] Make a call to get_aiports_1 function to connect to airports_server.c (server) and get 5 nearest neighbors. (yes, the function is called get_aiports_1, I accidentally misspelled “airports” in the .x file. We can go back later and fix this after we get this rpc working)
- [ ] This call returns a list of 5 nodes. Copy that list in placeslist (defined in places.x)
- [ ] Each node in the list has a code, name, state, distance, and *next.

 

####airports_svc.c

- [ ] Add code to do the following:
- [x] Read airport-locations.txt [Hesham: Done]
- [x] Parse the line that is read and store in an airport structure [Hesham: Done]
- [ ] Make KDtree
- [ ] Insert each airport into the KD Tree

 

airports_server.c

- [ ] Implement nearest neighbor search by searching KDTree with given params from places_server.c (lat/long).
- [ ] Store 5 nearest neighbors in a structure defined in airports.x (airportslist)
- [ ] Each Node in the list has a code, name, state, distance, and *next.
- [ ] Distance is calculated using given function by Dr. Zhu
- [ ] Return list with 5 nodes.

 

Other Activities:

- [x] Write .x files
- [x] Add kdtree.h and kdtree.c
- [ ] Connect places_server.c and airports_server.c
- [ ] Test

 

##Additional Notes:

- [ ] I haven’t connected the places and airports yet. First we need to implement the functionality individually on each and then connect them. The code is already setup for connection.
- That means, the two makefiles are independent. To compile the code just execute this in the command line:
  - make clean -f Makefile.places
  - make -f Makefile.places
- You can do the same thing for airports just replace “.places” with “.airports”
- I added kdtree.c and kdtree.h
