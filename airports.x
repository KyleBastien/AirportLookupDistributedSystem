/*airports.x*/

typedef struct airportsnode* airportslist;

/* a node in the airports list*/
struct airportsnode{
       string code<>;
       string name<>;
       string state<>;
       float distance;
       airportslist next;
};

struct coordinates {
       float lattitude;
       float longitude;
};


/*result of a readdir operation*/
union readairports_ret switch (int err) {
      case 0:
           airportslist list; /*no error, return airports listing*/
      case 1:
           void; /*error occured, nothing returned*/
};

/*IDL program*/
program AIRPORTSPROG {
        version AIRPORTS_VERS {
                readairports_ret get_airports(struct coordinates) = 1;
                } = 1;
} = 0x37777783;
