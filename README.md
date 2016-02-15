# AirportLookupDistributedSystem
A Distributed System for looking up the Five Nearest Airport given a City and State. For SEGR-5520.
This is a 3-tiered RPC which contains a client (places_client), a server for places (places_server), 
and a server for airports(airports_server)


Compilation:
To compile the code, execute 'make' in the command line.

Running RPC:
To run the RPC perform the following steps:
- compile by executing 'make'
- run ./airports_server&
- run ./places_server&
- run ./places_client <server_path> <"city name"> <state>


##Summary of Activity List within each .c file

####places_client.c

- [x] Read arguments in main
- [x] Call placesprog_1 function to connect to places_server.c (server) and pass lat/long arguments
- [x] This returns back a placeslist (defined in places.x)
- [x] Each node in the list has a code, name, state, distance, and *next.
- [x] Print out the list

####places_svc.c

- [x] Add code to do the following:
- [x] Read places2k.txt
- [x] Parse the line that is read and store in a place structure
- [x] Create trie tree
- [x] Insert place in trie tree

 
####places_server.c

- [x] Search for city and state in prefix tree
- [x] Get lat/long information from prefix tree
- [x] Make a call to get_aiports_1 function to connect to airports_server.c (server) and get 5 nearest neighbors. 
- [x] This call returns a list of 5 nodes. Copy that list in placeslist (defined in places.x)
- [x] Each node in the list has a code, name, state, distance, and *next.

 
####airports_svc.c

- [x] Add code to do the following:
- [x] Read airport-locations.txt
- [x] Parse the line that is read and store in an airport structure
- [x] Make KDtree
- [x] Insert each airport into the KD Tree

 
airports_server.c

- [x] Implement nearest neighbor search by searching KDTree with given params from places_server.c (lat/long).
- [x] Store 5 nearest neighbors in a structure defined in airports.x (airportslist)
- [x] Each Node in the list has a code, name, state, distance, and *next.
- [x] Distance is calculated using given function by Dr. Zhu
- [x] Return linked list with 5 nodes.


Other Activities:

- [x] Write .x IDL files
- [x] Add kdtree.h, kdtree.c, trie.h, and trie.c
- [x] Connect places_server.c and airports_server.c
- [x] Test using places2k.txt and airprot-list.txt
