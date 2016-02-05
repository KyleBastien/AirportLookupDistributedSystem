
/*places.x*/
const MAXLEN = 255;
typedef string airportstype<MAXLEN>;
typedef float airportsnum<MAXLEN>;

typedef struct airportsnode* airportslist;
/* a node in the places list*/
struct airportsnode{
       airportstype code;
       airportstype name;
       airportstype state;
       airportsnum distance;
       airportslist next;
};

struct coordinates {
       airportsnum lattitude;
       airportsnum longitude; 
};


/*result of a readdir operation*/
union readairports_ret switch (int err) {
      case 0:
           airportslist list; /*no error, return places listing*/
      case 1:
           void; /*error occured, nothing returned*/
};

/*IDL program*/
program AIRPORTSPROG {
        version AIRPORTS_VERS {
                readairports_ret get_aiports(struct coordinates) = 1;
                } = 1;
} = 0x37777783;
