
#ifndef PROJ2_H
#define PROJ2_H


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

typedef struct reserve {
    struct reserve* next;
    char *code;
    Data date;
    char* revCode;
    int pasNum;
}Reserve;

/*Variaveis Globais*/
extern int _numAeroportos;
extern Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];
extern int _numVoos;
extern Voo _voos[MAX_NUM_VOOS];
extern Data _hoje;
extern const int _diasMesAc [];


/* Funcoes primeiro projeto*/

/* cria uma estrutura Hora atraves do input*/
Hora leHora();

/*cria uma estrutura Data atraves do input*/
Data leData();

/*recebe um vetor de caracteres e preenche com o input do terminal*/
int leProximaPalavra(char str[]);

/*le uma palavra ate ao fim da linha*/
void lePalavraAteFimDeLinha(char str[]);

/*le todos os caracteres ate ao fim da linha*/
void leAteFimDeLinha();

/*imprime uma Data*/
void mostraData(Data d);

/*imprime uma Hora*/
void mostraHora(Hora h);

/*converte uma data em dias,tendo como referencia 2022 o ano inicial*/
int converteDataNum(Data d);

/*converte uma Hora em minutos*/
int converteHoraNum(Hora h);

/*converte uma Data em minutos*/
int converteDataHoraNum(Data d, Hora h);

/*converte minutos numa Hora*/
Hora converteNumHora(int num);

/*converte minutos numa Data*/
Data converteNumData(int num);

/*recebe uma Data e verifica se representa uma data no passado ou a um ano de distancia*/
int validaData(Data d);

/*recebe uma Hora e verifica se e maior que 12 horas ou menos que 0 horas*/
int validaHora(Hora h);

/*algoritmo de ordenacao bubbleSort*/
void bubbleSort(int indexes[], int size, int (*cmpFunc)(int a, int b));

/*verifica se a string representa um id de aeroporto valido*/
int aeroportoInvalido(char id[]);

/*procura um aeroporto no vetor global de aeroportos com*/
int encontraAeroporto(char id[]);

/*adiciona um aeroporto ao vetor global de aeroportos*/
void adicionaAeroporto();

/*imprime um aeroporto*/
void mostraAeroporto(int index);

/*compara id de dois aeroportos*/
int cmpAeroportos(int a, int b);

/*lista todos os aeroportos*/
void listaTodosAeroportos();

/*funcao do projeto que lista todos os aeroportos ou apenas os que sao colocados no input*/ 
void listaAeroportos();

/*imprime um voo*/
void mostraVoo(int index);

/*mostra o Voo de Partida*/
void mostraVooPartida(int index);

/*mostra o Voo de Chegada*/
void mostraVooChegada(int index);

/*verifica se um voo existe atraves de um codigo e de uma Data*/
int encontraVoo(char id[], Data d);

/*verifica se o id de voo contem as condicoes necessarias*/
int validaIDVoo(char id[]);

/*verifica se um voo e valido*/
int validaVoo(Voo v);

/*cria um voo*/
void criaVoo(Voo v);

/*adiciona um voo*/
void adicionaListaVoos();

/*compara voos de partida*/
int cmpVoosPartida(int a, int b);

/*compara voos de chegada*/
int cmpVoosChegada(int a, int b);

/*lista voos de partida*/
void listaVoosPartida();

/*lista voos de chegada*/
void listaVoosChegada();

/*altera uma data*/
void alteraData();

/*FUNCOES SEGUNDO PROJETO*/


void* mymalloc(long size);

void freeReserve(Reserve* node);

void destroyList(Reserve* head);

Reserve* makeReserve(char* fltCode,Data date);

Reserve* addReserve(Reserve *head,Reserve* newReserve);

int validateRevCode(char*s);

void sortVec(Reserve** vec,int revNum);

void printvec(Reserve** vec,int size);

int  findRevCode(Reserve* linkedList , char* revCode);

int validateRevCap(Voo flight , Reserve* reserve);

int validateRev(Reserve* linkedList,Reserve* reserve);

int cmpDate(Data flightD,Data d);

void listReserves(Reserve* head,int idx);

Reserve* addListReserve(Reserve* head);

char* readString ( );

Reserve* popReserve ( Reserve* head, char* code);

int freeAeroporto(char* code);

Reserve* popReserve2 ( Reserve* reserve);

Reserve* popAllReserve(Reserve*head , int size, char* code);

Reserve* freeFlighReserve(Reserve* head);

#endif