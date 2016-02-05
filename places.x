
/*places.x*/
const MAXLEN = 255;
typedef string placestype<MAXLEN>;
typedef float placesnum<MAXLEN>;
typedef struct placesnode* placeslist;

/* a node in the places list*/
struct placesnode{
       placestype code;
       placestype name;
       placestype state;
       placesnum distance;
       placeslist next;
};

struct location{
       placestype city;
       placestype state;
       placestype host;
};


/*result of a readdir operation*/
union readplaces_ret switch (int err) {
      case 0:
      	   placeslist list; /*no error, return places listing*/
      case 1:
      	   void; /*error occured, nothing returned*/
};

/*IDL program*/
program PLACESPROG {
	version PLACES_VERS {
		readplaces_ret get_places(struct location) = 1;
		} = 1;
} = 0x3777777D;
