# This is a template Makefile generated by rpcgen

# Parameters

CLIENT = places_client
SERVER = places_server
SERVER2 = airports_server

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
#SOURCES.x = places.x

TARGETS_SVC.c = places_svc.c places_server.c places_xdr.c trie.c
TARGETS_SVC2.c = airports_svc.c airports_server.c airports_xdr.c kdtree.c

TARGETS_CLNT.c = places_clnt.c places_client.c places_xdr.c
TARGETS_CLNT2.c = airports_clnt.c airports_client.c airports_xdr.c

TARGETS = places.h places_xdr.c places_clnt.c places_svc.c places_client.c places_server.c
TARGETS2 = airports.h airports_xdr.c airports_clnt.c airports_svc.c airports_client.c airports_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_CLNT2 = $(SOURCES_CLNT2.c:%.c=%.o) $(TARGETS_CLNT2.c:%.c=%.o)

OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)
OBJECTS_SVC2 = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC2.c:%.c=%.o)

# Compiler flags 

CFLAGS += -g 
LDLIBS += -lnsl -lm
RPCGENFLAGS = 

# Targets 

all : $(CLIENT) $(SERVER) $(SERVER2)


#$(TARGETS) : $(SOURCES.x) 
#	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

$(OBJECTS_CLNT2) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT2.c)

$(OBJECTS_SVC2) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC2.c)

#$(CLIENT) : $(OBJECTS_CLNT)
#	  $(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS)

$(SERVER2) : $(OBJECTS_SVC2)
	   $(LINK.c) -o $(SERVER2) $(OBJECTS_SVC2) $(LDLIBS)

clean:
	$(RM) core $(OBJECTS_CLNT) $(OBJECTS_SVC) $(OBJECTS_SVC2) $(CLIENT) $(SERVER) $(SERVER2)

