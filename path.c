#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdint.h>
#include<limits.h>
#include<stdbool.h>


#define MAXS 16
#define INF 999999
#define MIN_HEAP_CAPACITY 1870
#define CACHE_CAP 6000
#define DEFAULT_COST 1

typedef struct{
    int x;
    int y;
}offsetcoord_t;

typedef struct{
    int q;
    int r;
}hex_t;

typedef struct{
    int tot_dist;
    int x;
    int y;
}queue_elem_t;

typedef struct {
    queue_elem_t arr[MIN_HEAP_CAPACITY];
}min_heap_t;

typedef struct air_route{
    uint32_t dest_key:20;
    uint8_t occupied :1;
}air_route_t;

typedef struct{
    int dist;
    int costo;
    uint8_t generation;
    air_route_t *ar;
    uint8_t num_ar;
}cella_t;


typedef struct{
    int dist;
    int dest_key;
    int s_key;
}cache_t;


static inline int max(int, int);
static inline int mini(int, int);


static inline cella_t* init(cella_t* map, int , int);

static inline void toggle_air_route(int, int, int, int, air_route_t* h, uint8_t* num_ar);
static inline int search_ar(int,air_route_t*h, int* de);
static inline void insert_ar(air_route_t* h, int);

static inline int va(int);
static inline void oddr_to_axial(hex_t* hex, int, int);
static inline offsetcoord_t axial_to_oddr(int, int, offsetcoord_t* c);
static inline int change_cost(int curr_cost, int r, int v, int dist);


static inline void heapify_down(int i);
int dijkstra(cella_t* map,  int xp, int yp, int xd, int yd, int x, int y, cache_t cache[]);
static inline void minheapify(int i);
void resize_mh(min_heap_t* min);

static inline void insert_in_mh(int x, int y, int dist);
static inline queue_elem_t extract_mh();
static inline void check_dist_v(cella_t* map,int i, int j, int ndis);
static inline void check_generation(cella_t* c);
static inline int function(int, int);

uint8_t current_generation = 0;
static uint16_t min_noe=0;

static min_heap_t min;
static int max_y;
static int max_x;

int main(){
    cache_t *cache;
    cella_t *map;
    int x, y, i, j, xp, yp, xd, yd, v, raggio, N;
    int ciao;
    char command[MAXS+1];
    int ris;
    hex_t hex, a;
    offsetcoord_t c;
    cella_t*curr;


    cache = calloc(CACHE_CAP, CACHE_CAP*sizeof(cache_t));

    ciao = 0;
    map = NULL;
    while(scanf("%s", command)!=EOF){

        if(strcmp(command, "init")==0){
            free(cache);
            cache = calloc(CACHE_CAP, sizeof(cache_t));
            
            ciao = scanf("%d %d", &y, &x);
            map = init(map, x, y);
        } else if(strcmp(command, "toggle_air_route")==0){
            ciao = scanf("%d %d %d %d", &yp, &xp, &yd, &xd);
            if(xp>=0 && xp<x && yp>=0 && yp<y 
                && xd>=0 && xd<x && yd>=0 && yd<y){

                free(cache);
                cache = calloc(CACHE_CAP, sizeof(cache_t));

                curr = &map[xp*max_y+yp];

                if(curr->num_ar==0){
                    curr->ar = calloc(4, sizeof(air_route_t));
                }
                toggle_air_route(xp, yp, xd, yd, curr->ar, &curr->num_ar);
            }else{
                printf("KO\n");
            }

        }else if(strcmp(command, "change_cost")==0){
            ciao = scanf("%d %d %d %d", &yp, &xp, &v, &raggio);
            if(v>=-10 && v<=10 && raggio!=0 && (xp>=0 && xp<x && yp>=0 && yp<y)){
                free(cache);
                cache = calloc(CACHE_CAP, sizeof(cache_t));

                oddr_to_axial(&a, xp, yp);
                N = raggio-1;
                for(hex.q = -N; hex.q<= +N; hex.q++){
                    for(hex.r = max(-N, -hex.q-N); hex.r<=mini(+N, -hex.q+N); hex.r++){
                            c = axial_to_oddr(a.r+hex.r, a.q+hex.q, &c);
                            i = c.x;
                            j = c.y;
                            if(i>=0 && i<x && j>=0 && j<y){
                                curr = &map[i*max_y+j];
                                
                                if(curr){
                                    int dist = (va(hex.q)+va(hex.r)+va(hex.q+hex.r))/2;
                                    curr->costo = change_cost(curr->costo, raggio, v, dist);
                                    if(curr->costo>100){
                                         curr->costo = 100;
                                    }else if(curr->costo<0)
                                         curr->costo = 0;
                                }
                            }   
                        }
                    }
                printf("OK\n");
            }else
                printf("KO\n");
        }else if(strcmp(command, "travel_cost")==0){

            ciao = scanf("%d %d %d %d", &yp, &xp, &yd, &xd);
            if(xp>=0 && xp<x && yp>=0 && yp<y && xd>=0 && xd<x && yd>=0 && yd<y){
                current_generation++;
                ris = dijkstra(map, xp , yp, xd, yd, x, y, cache);
                if(ris>=0){
                    printf("%d\n", ris);
                }else
                    printf("-1\n");
            }else
                printf("-1\n");
        }
    }

    ciao++;


//Funioni di free della memoria utilizzata
        free(cache);

        free(map);

    return 0;
}



//INIZIO: FUNZIONI INIT, e funzini per hash map

static inline cella_t* init(cella_t* map, int x, int y){
    int i,j;


    if(map){

        free(map);
    }

        map = malloc(x*y*sizeof(cella_t));
        if(map){
            for(i=0; i<x; i++){
                for(j=0; j<y; j++){
                    map[i*y+j].costo = DEFAULT_COST;
                    map[i*y+j].num_ar=0;
                    map[i*y+j].ar = NULL;
                }
            }
        }else{
            printf("Errore nell'allocazione della mappa\n");
            return NULL;
        }

    printf("OK\n");
    max_y = y;
    max_x = x;

    return map;
}


//funzione che calcola il valore assoluto di un intero
static inline int va(int num){

    if(num<0)
        num = -num;

    return num;
}


//INIZIO: FUNZIONI PER ROTTE AEREE///////////////////////////////


static inline void insert_ar(air_route_t h[], int new_key){
    int  i;

    for(i=0; i<4; i++){
        if(!(h[i].occupied)){
            h[i].dest_key = new_key;
            h[i].occupied = 1;
            break;
        }
    }
    
}

static inline int search_ar(int new_key, air_route_t h[], int*de){
    int i;

    for(i=0; i<4; i++){
        if(h[i].occupied && h[i].dest_key == new_key){
            *de = i;
            return 1;
        }
    }

    return 0;
}

//fai un check della validità delle coordinate
static inline void toggle_air_route(int x1, int y1, int x2, int y2, air_route_t *h, uint8_t* num_ar){
    int da_eliminare;
    int new_key = x2*max_y + y2;

    if(search_ar(new_key, h, &da_eliminare)){
        h[da_eliminare].occupied=0;
        *num_ar = *num_ar-1;
    }else if(*num_ar<4){
        insert_ar(h, new_key);
        *num_ar = *num_ar+1;
    }

    printf("OK\n");
}


//FINE: FUNZIONI PER ROTTE AEREE///////////////////////////////////


//INZIO: FUNZIONI PER CHANGE_COST/////////////////////////////////////////


static inline int max(int a, int b){
    if(a>b)
        return a;
    return b;
}

static inline int mini(int a, int b){

    if(a<b)
        return a;
    return b;

}

static inline offsetcoord_t axial_to_oddr(int r, int q,  offsetcoord_t* cart){
    int parity;

    if(cart){
        parity = r&1;
        cart->y = q + (r - parity)/2;
        cart->x = r;
    }

    return *cart;
}


static inline void oddr_to_axial(hex_t* hex, int x, int y){
    int parity;

    if(hex){
        parity = x&1;
        hex->q = y -(x-parity)/2;
        hex->r = x;
    }
}

static inline int change_cost(int curr_cost, int r, int v, int dist){
    int ris;
    float k;

    k = (((float)r-(float)dist)/(float)r);
    ris = curr_cost + floor((float)v*k);

    return ris;
}


//INZIO: FUNZIONI PER TRAVEL_COST//////////////////////////////////////////////




//INZIO: FUNZIONI PER TRAVEL_COST//////////////////////////////////////////////

static inline void minheapify(int i){
    int parent_i;
    queue_elem_t tmp;

    parent_i = (int)((i-1)/2);  
    while(i>0 && min.arr[i].tot_dist<min.arr[parent_i].tot_dist){   

        tmp = min.arr[i];
        min.arr[i] = min.arr[parent_i];
        min.arr[parent_i] = tmp;
        i = parent_i;
        parent_i = (int)((i-1)/2);
    }
}

static inline void insert_in_mh(int x, int y, int dist){

    if(min_noe<MIN_HEAP_CAPACITY){
            min.arr[min_noe].x = x;
            min.arr[min_noe].y = y;
            min.arr[min_noe].tot_dist = dist;

            minheapify(min_noe);
            min_noe++;
    }else{
        printf("ciao\n");

        insert_in_mh(x, y, dist);
    }

}


static inline void heapify_down(int i) {

while(true){
    int mini = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    queue_elem_t tmp;

    if (l < min_noe && min.arr[l].tot_dist < min.arr[mini].tot_dist) {
        mini = l;
    }

    if (r < min_noe && min.arr[r].tot_dist < min.arr[mini].tot_dist) {
        mini = r;
    }
        if(mini == i){
            break;
        }

        tmp = min.arr[i];
        min.arr[i] = min.arr[mini];
        min.arr[mini] = tmp;

        i = mini;

    }
}


static inline queue_elem_t extract_mh(){
    queue_elem_t minimo;
    int end;

    if(min_noe<1){
        minimo.x = -1;
        minimo.y = -1;
        return minimo;
    }
    end = min_noe-1; 
    minimo = min.arr[0];
    min.arr[0] = min.arr[end];
    min_noe--;
   
    if(min_noe>0){
        heapify_down(0);
    }
    
    return minimo;
}

static inline void check_dist_v(cella_t* map, int i, int j, int ndis){
                    int key;
                    key = i*max_y+j;

                    check_generation(&map[key]);

                    if(ndis< map[key].dist){
                        map[key].dist = ndis;
                        insert_in_mh(i, j, ndis);
                    }                    
}

static inline void check_generation(cella_t* c){

    if(c->generation != current_generation){
        c->dist = INF;
        c->generation = current_generation;
    }
}


static inline int function(int s_key, int d_key){
    int index;

    index = s_key+d_key;

    return index%CACHE_CAP;
}


int dijkstra(cella_t* map,  int xp, int yp, int xd, int yd, int x, int y, cache_t cache[]){
        int xu, yu, costo_u, ndis;  //coordinate del nodo corrente preso in considerazione
        queue_elem_t u;
        int cache_index;
        air_route_t* curr;
        int i, j, k, s_key, d_key;
        int ris, num_ar;
        cella_t *u_curr;

        ris = -1;
        min_noe = 0;

        s_key = xp*max_y+yp;
        d_key = xd*max_y+yd;

        cache_index = function(s_key, d_key);
        if(cache[cache_index].dist!=0 && cache[cache_index].s_key==s_key && cache[cache_index].dest_key==d_key){
            return cache[cache_index].dist;
        }


            u_curr= &map[s_key];
            if(u_curr){
                u_curr->dist = 0;

                insert_in_mh(xp, yp, 0);

                    while(min_noe!=0){
                        
                        u = extract_mh(min);
                        if(u.x==xd && u.y==yd){
                            ris = u.tot_dist;    // risultato trovato 
                            cache[cache_index].s_key=s_key;
                            cache[cache_index].dest_key=d_key;
                            cache[cache_index].dist = ris;
                            break;
                        }
                            
                            xu = u.x;
                            yu = u.y;

                            //inizializzazione del costo del nodo corrente
                            u_curr = &map[xu*max_y+yu];
                                if(u.tot_dist>u_curr->dist){
                                    continue;
                                }
                               //non era un duplicato  
                                costo_u = u_curr->costo;
                                
                                if(costo_u>0){
                                    //check degli adiacenti di u, i
                                    ndis = u.tot_dist + costo_u;
                                    if(xu%2==0){
                                        for(i=xu-1; i<=xu+1; i++){
                                            for(j=yu-1; j<=yu; j++){
                                                if(i>=0 && i<x && j>=0 && j<y){
                                                    if(!(i==xu && j==yu)){
                                                        check_dist_v(map, i, j, ndis);
                                                    }
                                                }   
                                            }
                                        }   
                                        //casella a dx di quella attuale
                                        i = xu, j = yu+1;
                                        if(i>=0 && i<x && j>=0 && j<y){
                                           check_dist_v(map,  i, j, ndis);
                                        }
                                    }else{
                                        for(i=xu-1; i<=xu+1; i++){
                                            for(j=yu; j<=yu+1; j++){
                                                if(i>=0 && i<x && j>=0 && j<y){
                                                    if(!(i==xu && j==yu)){
                                                        check_dist_v(map, i, j, ndis);
                                                    }
                                                }   
                                            }
                                        }   
                                        //casella a sx di quella attuale
                                        i = xu, j = yu-1;
                                        if(i>=0 && i<x && j>=0 && j<y){
                                            check_dist_v(map, i, j, ndis);
                                        }
                                    }
                                    //check air_rooute
                                    if(u_curr && u_curr->num_ar>0){   
                                        curr = u_curr->ar;
                                        num_ar = u_curr->num_ar;
                                        for(k=0; k<num_ar; k++){
                                            if(curr[k].occupied){
                                                i = curr[k].dest_key/max_y;
                                                j = curr[k].dest_key%max_y;
                                                if(!(i==xu && j ==yu)){
                                                    check_dist_v(map,i, j, ndis);
                                                }
                                            }
                                        }
                                    }
                                }
                    }    
        }

    cache[cache_index].s_key=s_key;
    cache[cache_index].dest_key=d_key;
    cache[cache_index].dist = ris;
    
    return ris;
}