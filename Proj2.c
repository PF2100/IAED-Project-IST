
#include "proj2.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0;		/* número de voos introduzidos */
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };


/*Verifies if there is space to allocate*/
void* mymalloc(long size) {
    void* space = malloc (size);
    if ( !(space) ) {
        printf("No memory\n");
        exit(0);
    }
    return space;
}

/*Frees a Reserve along with its content*/
void freeReserve(Reserve* node) {
    free(node->code);
    free(node->revCode);
    free(node);
}

/*Frees all the contents of a LinkedList*/
void destroyList(Reserve* head) {
    Reserve* cursor;
    while ( head ) {
		cursor = head;
		head= head->next;
        freeReserve(cursor);
    }
}

/*Allocates memory and creates a Reserve*/
Reserve* makeReserve(char* fltCode,Data date) {
    char name[65535] ;
    int pass;
    Reserve* newReserve = mymalloc(sizeof(Reserve));
    scanf("%s %d",name,&pass);
    newReserve->code = mymalloc(sizeof(char)*(strlen(fltCode)+1));
    newReserve->revCode = mymalloc(sizeof(char)*(strlen(name)+1));
    strcpy(newReserve->revCode,name);
    strcpy(newReserve->code,fltCode);
    newReserve->date = date;
    newReserve->pasNum = pass;
    return newReserve;
}

/*Adds a Reserve to the linkedList*/
Reserve* addReserve(Reserve *head,Reserve* newReserve) {
    newReserve->next=head;
    return newReserve;
}

/*Validates a Reserve Code*/
int validateRevCode(char*s) {
    int size = 0;
    size = strlen(s);
    while ( *s != '\0' ) {
        if ( !(*s >= '0' && *s<='9') && !(*s >= 'A' && *s<='Z') ) {
            return FALSE;
        }
        s++;
    }
    return (size >=10) ;
}

/*Sorts a vector of Reserves using Insertion Sort*/
void sortVec(Reserve** vec,int revNum) {
    int mark, count;
    Reserve* key;
    for (mark = 1; mark < revNum; mark++) {
        key = vec[mark];
        count = mark - 1;
        while (count >= 0 && strcmp(vec[count]->revCode,key->revCode) > 0){
            vec[count + 1] = vec[count];
            count = count - 1;
        }
        vec[count + 1] = key;
    }
}

/*Recieves a vector and prints every Reserve it has*/
void printvec(Reserve** vec,int size) {
    int i = 0;
    while ( i < size ) {
        printf("%s %d\n",vec[i]->revCode,vec[i]->pasNum);
        i++;
    }
}

/*Verifies if already exists a Reservation code similar to RevCode*/
int  findRevCode(Reserve* linkedList , char* revCode) {
    Reserve* cursor = linkedList;
    while ( cursor ) {
        if ( strcmp(cursor->revCode,revCode)==0) {
            return TRUE;
        }
        cursor = cursor->next;
    }
    return FALSE ;
}

/*Verifies if the capacity of the reserve is larger than the amount of available flight seats*/
int validateRevCap(Voo flight , Reserve* reserve) {
    return (flight.capacidade - reserve->pasNum) >= 0;
}

/*Verifies if the Reserve can be added*/
int validateRev(Reserve* linkedList,Reserve* reserve) {
    int idx;
    if ( validateRevCode(reserve->revCode) == FALSE) {
        printf("invalid reservation code\n");
    }
    else if ((idx = encontraVoo(reserve->code,reserve->date)) == NAO_EXISTE) {
		printf("%s: flight does not exist\n",reserve->code);
    }
    else if ( findRevCode(linkedList,reserve->revCode) == TRUE) {
        printf("%s: flight reservation already used\n",reserve->revCode);
    }
    else if( !(validateRevCap(_voos[idx],reserve)) ) {
        printf("too many reservations\n");
    }
    else if ( (validaData(reserve->date) == FALSE)) {
        printf("invalid date\n");
    }
    else if ( reserve->pasNum <=0) {
        printf("invalid passenger number\n");
    }
    else 
        return idx; /*returns the flight index*/
    return NAO_EXISTE;
}

/*compares two dated*/
int cmpDate(Data flightD,Data d) {
	return (converteDataNum(flightD) == converteDataNum(d));
}
/*Lists  all of the Reserves of a certain flight*/
void listReserves(Reserve* head,int idx) {
    int size = _voos[idx].reservas , i = 0;
    Reserve* cursor = head;
    Reserve** vec = mymalloc(sizeof(Reserve)*size);
    while ( cursor && i < size ) {
        if ( (strcmp(cursor->code,_voos[idx].id) == 0) && cmpDate(_voos[idx].data,cursor->date) ) {
            vec[i] = cursor;
            i++;
        }
        cursor=cursor->next;
    }
    sortVec(vec,size);/*sorts the reserve vector*/
    printvec(vec,size);/*prints the contents of the same vector*/
	free(vec);
}

/*Adds or Lists reserves to the linkedList*/
Reserve* addListReserve(Reserve* head) {
    char name[MAX_CODIGO_VOO], c;
    Reserve* reserve= NULL;
    Data d;
    int idx;
    leProximaPalavra(name);
    d = leData() ;
    if ( (c = getchar())!= '\n') {
        reserve= makeReserve(name,d);
        if ( (idx = validateRev(head,reserve) ) == NAO_EXISTE ) {
            freeReserve(reserve);
            return head;
        }
        _voos[idx].capacidade -= reserve->pasNum;
        _voos[idx].reservas++;
        head = addReserve(head,reserve);
    }
    else {
        idx =  encontraVoo(name,d);
        if ( idx == NAO_EXISTE) {
            printf("%s: flight does not exist\n",name);
        }
        else if ( validaData(d) == 0 ) {
            printf("invalid date\n");
        }
        else if ( head ) {
            listReserves(head,idx);
        }
    }
    return head;
}

/*Reads the code of a flight*/
char* readString ( ) {
    char name[65535] ;
    char* code;
    scanf("%s",name);
    code = mymalloc(sizeof(char)*strlen(name)+1);
    strcpy(code,name);
    return code;
}

/*Eliminates a Reserve from the LinkedList*/
Reserve* popReserve ( Reserve* head, char* code) {
    Reserve* cursor = head;
    Reserve* prev = NULL ;
    int idx;
    while ( cursor ) {
        if (strcmp(cursor->revCode,code) == 0) {
            if( prev != NULL) {
                prev->next = cursor->next;
            } 
            else {
                head = cursor->next;
            }
            idx = encontraVoo(cursor->code,cursor->date);
            _voos[idx].capacidade += cursor->pasNum; /*adds the number of passengers to the flight*/
            _voos[idx].reservas--; /*subtracts a reserve from its number of reserves*/
            freeReserve(cursor);
            break;
        }
        prev = cursor;
        cursor = cursor->next;
    }
    return head;
}

/*Verifies if a certain flight code already exists*/
int searchFlight(char* code ) {
    int idx ;
    for ( idx = 0 ; idx < _numVoos ; idx++) {
        if ( strcmp(_voos[idx].id,code) == 0) {
            return TRUE;
        }
    }
    return NAO_EXISTE;
}

/*Frees an airport by overwriting the position of the vector with the flights that do not have the code*/
int freeAeroporto(char* code) {
    int idx , count = 0 , airIdx , j ;
    for ( idx = 0 , j = 0; idx <=_numVoos ; idx++,j++) {
        if ( strcmp(_voos[idx].id,code) == 0 ) {
            airIdx=encontraAeroporto(_voos[idx].partida);
            _aeroportos[airIdx].numVoos--;
            count += _voos[idx].reservas;
            j--;
        }
        else {
            _voos[j] = _voos[idx];
        }
    }
    _numVoos = _numVoos - (idx - j); /*removes the number of flights that were deleted from the global variable*/
    return count;/*returns the number of reserves that have to be removed*/
}

/*Frees a certain reserve and returns its next*/
Reserve* popReserve2 ( Reserve* reserve) {
    Reserve* aux ;
    aux  = reserve->next;
    freeReserve(reserve);
    return aux;
}

/*Eliminates all of the Reserves of a LinkedList*/
Reserve* popAllReserve(Reserve*head , int size, char* code) {
    Reserve* cursor = head;
    Reserve* prev = NULL;
    while ( cursor && size > 0 ) {
        if ( strcmp(cursor->code,code) == 0 ) {
            if ( prev == NULL) {
                head = popReserve2(cursor);
                cursor = head;
                size--;
            }
            else {
                prev->next = popReserve2(cursor);
                cursor = prev->next;
                size--;
            }
        }
        else {
            prev = cursor;
            cursor = cursor->next;
        }
    }
    return head;
}

/*Eliminates All of the reserves from a flight*/
Reserve* freeFlighReserve(Reserve* head) {
    char* code = readString ();
	int revNum ;
    if (strlen(code) >= 10) { 
        if ( findRevCode(head,code) == FALSE) {
            printf("not found\n");
        }
        else {
            head = popReserve(head,code);
        }
    }
	else {
		if ( searchFlight(code) == NAO_EXISTE ) {
            printf("not found\n");
        }
        else {
            revNum=freeAeroporto(code);
            head = popAllReserve(head,revNum,code);
        }
	}
    free(code);
    return head;
}




int main() {
	int c;
	Reserve* head = NULL;
	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q':
            destroyList(head);
            return 0;
		case 'a': 
            adicionaAeroporto();
			break;
		case 'l': 
            listaAeroportos();
			break;
		case 'v': 
            adicionaListaVoos();
			break;
		case 'p': 
            listaVoosPartida();
			break;
		case 'c': 
            listaVoosChegada();
			break;
		case 't': 
            alteraData();
			break;
		case 'r': 
            head = addListReserve(head);
            break;
        case 'e':
            head = freeFlighReserve(head);
            break;
		}
	}
	return 0;
}