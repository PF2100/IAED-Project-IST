/* primeiro projeto de IAED
 * autor: vmm
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
	int reservas;
} Voo;


/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0;		/* número de voos introduzidos */
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	printf("%02d-%02d-%d",d.dia, d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc)(int a, int b)) {
	int i, j, done;

	for (i = 0; i < size - 1; i++) {
		done = 1;
		for (j = size - 1; j > i; j--)
			if ((*cmpFunc) (indexes[j - 1], indexes[j])) {
				int aux = indexes[j];
				indexes[j] = indexes[j - 1];
				indexes[j - 1] = aux;
				done = 0;
			}
		if (done)
			break;
	}
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
		_aeroportos[index].cidade, _aeroportos[index].pais,
		_aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
	return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
		_voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
			&& numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flihts\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
					_voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_aeroportos[encontraAeroporto(v.partida)].numVoos++;
	_voos[_numVoos].reservas = 0;
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}


typedef struct reserve {
    struct reserve* next;
    char *code;
    Data date;
    char* revCode;
    int pasNum;
}Reserve;

void* mymalloc(long size) {
    void* space = malloc (size);
    if ( !(space) ) {
        printf("no memory\n");
        exit(0);
    }
    return space;
}


void printReserve(Reserve* reserve) {
    while ( reserve ) {
        printf("%s ",reserve->code);
        mostraData(reserve->date);
        printf("\n%s %d\n",reserve->revCode,reserve->pasNum);
        reserve=reserve->next;
    }
    return ;
}

void freeReserve(Reserve* node) {
    free(node->code);
    free(node->revCode);
    free(node);
}

void destroyList(Reserve* head) {
    Reserve* cursor;
    while ( head ) {
		cursor = head;
		head= head->next;
        freeReserve(cursor);
    }
}

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

Reserve* searchReserve(Reserve* head,char* code) {
    while(head) {
        if ( strcmp(head->revCode,code) == 0) {
            return head;
        }
    }
    return NULL;
}

Reserve* addReserve(Reserve *head,Reserve* newReserve) {
    newReserve->next=head;
    return newReserve;
}

Reserve* cpyReserve(Reserve * res) {
    Reserve* copy = mymalloc(sizeof(Reserve)) ;
    copy->revCode = mymalloc(sizeof(char)*strlen(res->revCode)) ;
    copy->code = mymalloc(sizeof(char)*(strlen(res->code)+1));
    strcpy(copy->code,res->code);
    strcpy(copy->revCode,res->revCode);
    copy->date = res->date ;
    copy->pasNum = res->pasNum;
    return copy;
}

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

void printvec(Reserve** vec,int size) {
    int i = 0;
    while ( i < size ) {
        printf("%s %d\n",vec[i]->revCode,vec[i]->pasNum);
        i++;
    }
}

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

int validateRevCap(Voo flight , Reserve* reserve) {
    return (flight.capacidade - reserve->pasNum) >= 0;
}


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
        return idx;
    return NAO_EXISTE;
}

int cmpDate(Data flightD,Data d) {
	return (converteDataNum(flightD) == converteDataNum(d));
}

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
    sortVec(vec,size);
    printvec(vec,size);
	free(vec);
}

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

char* readString ( ) {
    char name[65535] ;
    char* code;
    scanf("%s",name);
    code = mymalloc(sizeof(char)*strlen(name)+1);
    strcpy(code,name);
    return code;
}

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
            _voos[idx].capacidade += cursor->pasNum;
            _voos[idx].reservas--;
            freeReserve(cursor);
            break;
        }
        prev = cursor;
        cursor = cursor->next;
    }
    return head;
}

Reserve* freeFlighReserve(Reserve* head) {
    char* code = readString ();
    if (strlen(code) >= 10) {
        if ( findRevCode(head,code) == FALSE) {
            printf("not found\n");
            free(code);
            return head;
        }
        head = popReserve(head,code);
    }
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