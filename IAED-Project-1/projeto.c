#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


/*Max characters of a code */
#define MAXCODE 7
/*Max characters of a ID */
#define MAXIDX 4
/*Max characters of a Country */
#define MAXCTRY 31
/*Max characters of a City */
#define MAXCITY 51
/*Max number of airports */
#define MAXAIR 40
/*Max number of flights */
#define MAXFLT 30000

/*Date structure */
typedef struct
{
    int day, month, year;
} date;

/* System Date */
date currentDate = {1, 1, 2022};

/* Number of airports created*/
int n_ports = 0;

/*Number of flights created*/
int totalFlts = 0;


/*Time structure */
typedef struct
{
    int hour, minute;
} time;

/*Flight structure*/
typedef struct
{
    char code[MAXCODE], idDepart[MAXIDX], idArrival[MAXIDX];
    date departDate;
    time departTime;
    time durationTime;
    date arrivalDate;
    time arrivalTime;
} Flights;

/*Airport structure*/
typedef struct
{
    char id[MAXIDX];
    char country[MAXCTRY];
    char city[MAXCITY];
    int nflights;
} Airport;

/* Vector of Airport stuctures*/
Airport Aero[MAXAIR]; 
/* Vector of flight stuctures*/
Flights Flts[MAXFLT];


/*all of main commands*/
void command_a();
void command_l();
void command_v();
void command_p();
void command_c();
void command_t();

/*Auxiliar Functions*/
void insertSortA();
int airportExists(char id[]);
int validateCode(char code[]);
int validateFlightCodeNumber(char code[]);
int validateFlight(char code[], int yy, int mm, int dd);
int validateFlightAirports(char idDepart[], char idArrival[]);
int validateDate(int dd, int mm, int yy);
int validateDuration(int durHour, int durMin);
int validateCapacity();
void saveFlightIDs(char code[], char idDepart[], char idArrival[]);
date saveDate(int dd, int mm, int yy, date dates);
void saveTime(int hr, int min, int durHour, int durMin);
void sortFlightsP(Flights tempFlt[],int idx);
int compareDates( date depDate , time depTime , date keyDate , time keyTime);
Flights createArrivalDate(Flights flight );
void sortFlightsC( Flights tempFlt[],int idx);





int main()
{
    char command;
    int stop = 1;
    while (stop)
    {/*First character of every input line will determine wich case will happen */
        command = getchar(); 
        switch (command)
        {
        case 'q':
            stop = 0;
            break;
        case 'a':
            command_a();
            break;
        case 'l':
            command_l();
            break;
        case 'v':
            command_v();
            break;
        case 'p':
            command_p();
            break;
        case 'c':
            command_c();
            break;
        case 't':
            command_t();
            break;
        default:
            break;
        }
    }
    return 0;
}

/*if any of the error conditions apply , prints corresponding message , otherwise adds a new airport to Aero*/
void command_a()
{
    int count;
    char id[MAXIDX], country[MAXCTRY], city[MAXCITY];
    scanf("%s %s %[^\n]", id, country, city);
    for (count = 0; count < MAXIDX - 1; count++)
    { /*prints the message "invalid airport ID\n" if the a letter of the id is not a uppercase letter */
        if ((id[count] < 'A' || id[count] > 'Z'))
        {
            printf("invalid airport ID\n");
            return;
        }
    }

    for (count = 0; count < n_ports; count++)
    { /*prints the message "duplicate airport" if already exists a airport with the same id*/
        if (strcmp(Aero[count].id, id) == 0)
        {
            printf("duplicate airport\n");
            return;
        }
    }

    if (n_ports < MAXAIR)
    { /*Adds a new airport to the vector of airports Aero */
        strcpy(Aero[n_ports].id, id);
        strcpy(Aero[n_ports].country, country);
        strcpy(Aero[n_ports].city, city);
        /*iniciates the number of flights the airplane has */
        Aero[n_ports].nflights = 0;
        printf("airport %s\n", Aero[n_ports].id);
        /*adds 1 to the number of flights created*/
        n_ports++;
        if (n_ports > 1)
        { /* Sorts the vector of Airport structures after adding a new airport to it */
            insertSortA();
        }
    }
    else
    {/*prints the message "too many airports" if the number of airports isnt smaller than MAXAIR */
        printf("too many airports\n");
    }
}

void insertSortA()
{/*sorts all of the airports by alphabetical order*/
    int mark, count;
    Airport Key;
    for (mark = 1; mark < n_ports; mark++)
    {
        Key = Aero[mark];
        count = mark - 1;
        while (count >= 0 && strcmp(Aero[count].id, Key.id) > 0)
        {/*compares both ids.If key is alphabetically first then the airport id,pushes the flight one idx , and continues to compare ids with the next airport*/    /*ACABAR*/
            Aero[count + 1] = Aero[count];
            count = count - 1;
        }
        Aero[count + 1] = Key;
    }
}

/*Lists the airport details for every ID it encounters.If no ID is found , then list all airports in alphabetical order */
void command_l()
{
    char c, id[MAXIDX];
    int count = 0, id_pos;
    /*c is the first charachter after the command */
    c = getchar();
    while (count < n_ports && c == '\n')
    { /* If c is not a newline character , then for the amount of airports in Aero print the member of the structure */
        printf("%s %s %s %d\n", Aero[count].id, Aero[count].city, Aero[count].country, Aero[count].nflights);
        count++;
    }
    while (c != '\n')
    {/* if c is a newline */
        scanf("%s", id);  /* id_pos is the value returned by the function compareID which can be either -1 or the index position of the airport in Aero */
        if ((id_pos = airportExists(id)) == -1)
        {
            printf("%s: no such airport ID\n", id);
            c = getchar();
        }
        else
        {
            printf("%s %s %s %d\n", Aero[id_pos].id, Aero[id_pos].city, Aero[id_pos].country, Aero[id_pos].nflights);
            c = getchar();
        }
    }
}


/*airportExists finds the corresponding airport to the input ID , if the airport exists.*/
int airportExists(char id[])
{
    int count = 0;
    while (count < n_ports)
    { /*Compares all of the existing airport ids , returning its index in the vector if it finds a match*/
        if (strcmp(id, Aero[count].id) == 0)
        {
            return count;
        }
        count++;
    }
    return -1; /* Otherwise return the value of -1 */
}

/*command_v adds a new airport if all of the parameters apply.If the command is inputted without any arguments , list all of the flight by creation order */
void command_v()
{
    date dates;
    char code[MAXCODE], idDepart[MAXIDX], idArrival[MAXIDX], c;
    int count = 0, idx ,yy, mm, dd, hr, min, durHour, durMin, Cap;
    if ((c = getchar()) != '\n')
    {
        scanf("%s %s %s", code, idDepart, idArrival);
        scanf("%d-%d-%d %d:%d", &dd, &mm, &yy, &hr, &min);
        scanf("%d:%d", &durHour, &durMin);
        if (validateCode(code) == 0 || validateFlight(code, yy, mm, dd) == -1 || (idx = validateFlightAirports(idDepart, idArrival))== -1 || 
                validateDate(dd, mm, yy) == 0 || (validateDuration(durHour, durMin) == -1 || (Cap = validateCapacity()) == -1))
        {

            return;
        }
        saveFlightIDs(code, idDepart, idArrival);
        Flts[totalFlts].departDate = saveDate(dd, mm, yy, dates);
        saveTime(hr, min, durHour, durMin);
        /*Adds 1 to the number of flights the departure airport has*/
        Aero[idx].nflights++;
        /*Adds 1 to the number of created flights*/
        totalFlts++;
        return;
    }
    while (count < totalFlts)
    {
        printf("%s %s %s ", Flts[count].code, Flts[count].idDepart, Flts[count].idArrival);

        printf("%02d-%02d-%02d %02d:%02d\n", Flts[count].departDate.day, Flts[count].departDate.month,
               Flts[count].departDate.year, Flts[count].departTime.hour, Flts[count].departTime.minute);
        count++;
    }
    return;
}

/*validateFlightCodeNumberverifies if the integer part of the code is 4 digit long max, and if it is initialized by a 0*/
int validateFlightCodeNumber(char code[])
{
    int i = 2;
    int nonZero = 0;
    while (code[i] != '\0')
    {
        if (code[i] == '0' && nonZero == 0)
        {
            return 0;
        }
        else if (code[i] >= '0' && code[i] <= '9')
        {
            nonZero = 1;
        }
        i++;
    }
    return 1;
}

/*validateCode verifies if the Code has 2 uppercase Letters and uses validateFlightCodeNumber to check the integer part of the code */
int validateCode(char code[])
{
    if (isupper(code[0]) && isupper(code[1]) && validateFlightCodeNumber(code))
    {
        return 1;
    }
    printf("invalid flight code\n");
    return 0;
}

/*validateFlight verifies if exists a flight with the same code and with the same departure Date*/
int validateFlight(char code[], int yy, int mm, int dd)
{
    int count;
    for (count = 0; count < totalFlts; count++)
    {
        if (strcmp(code, Flts[count].code) == 0)
        {
            if (yy == Flts[count].departDate.year && mm == Flts[count].departDate.month && dd == Flts[count].departDate.day)
            {
                printf("flight already exists\n");
                return -1;
            }
        }
    }
    return 0;
}

/*validateFlightAirports if the departure Airport and the arrival Airport exist*/
int validateFlightAirports(char idDepart[], char idArrival[])
{
    int id;
    if ((id = airportExists(idDepart)) == -1)
    {
        printf("%s: no such airport ID\n", idDepart);
        return -1;
    }
    else if ( airportExists(idArrival)  == -1)
    {
        printf("%s: no such airport ID\n", idArrival);
        return -1;
    }
    return id ;
}

/*validateDate verifies if the departure date is a year or longer from the currentDate or if the date is in the past*/
int validateDate(int dd, int mm, int yy)
{
    if (yy < currentDate.year || (yy == currentDate.year && mm < currentDate.month) || 
        (yy > currentDate.year && mm > currentDate.month))
    {
        printf("invalid date\n");
        return 0;
    }
    else if (yy == currentDate.year && mm == currentDate.month && dd < currentDate.day)
    {
        printf("invalid date\n");
        return 0;
    }
    else if ((yy > currentDate.year && mm == currentDate.month && dd > currentDate.day)|| yy > 2023)
    {
        printf("invalid date\n");
        return 0;
    }
    return 1;
}

/*ValidateDuration verifies if the flight duration isnt above 12 hours*/
int validateDuration(int durHour, int durMin)
{
    if (durHour == 12 && durMin > 0)
    {
        printf("invalid duration\n");
        return -1;
    }
    return 0;
}

/*validateCapacity verifies if the amount of passagers is between 10 and 100 */ 
int validateCapacity()
{
    int Type, Cap;
    Type = scanf("%d", &Cap);
    if (Type == 0 || Cap < 10 || Cap > 100)
    {
        printf("invalid capacity\n");
        return -1;
    }
    return Cap;
}

/*saveFlightIDs copies the code , departure and arrival ids into the flight corresponding members*/
void saveFlightIDs(char code[], char idDepart[], char idArrival[])
{
    strcpy(Flts[totalFlts].code, code);
    strcpy(Flts[totalFlts].idDepart, idDepart);
    strcpy(Flts[totalFlts].idArrival, idArrival);
    return;
}
/*saveDate saves the flights dapartureDate */
date saveDate(int dd, int mm, int yy, date dates)
{
    dates.year = yy;
    dates.month = mm;
    dates.day = dd;
    return dates;
}

/*saveTime saves the flights dapartureTime */
void saveTime(int hr, int min, int durHour, int durMin)
{
    Flts[totalFlts].departTime.hour = hr;
    Flts[totalFlts].departTime.minute = min;
    Flts[totalFlts].durationTime.hour = durHour;
    Flts[totalFlts].durationTime.minute = durMin;
    return;
}


/*command p recieves a ID and prints all of the departure flights latest to recent, if the airport exists*/
void command_p() 
{
    Flights tempFlt[MAXFLT]; 
    char airportID[MAXIDX];
    int j,idx , nPortFlights,count;
    scanf("%s",airportID);
    if ( (idx = airportExists(airportID)) == -1 ) 
    {
        printf("%s: no such airport ID\n",airportID);
        return;
    }
    /*nPortFlights is the number of flights the airport has*/
    nPortFlights = Aero[idx].nflights;
    /*Goes through all of the flights until it finds nPortflights amountt of flights and puts them in a temporary Flights vector*/
    for ( count = 0 , j = 0; (j < nPortFlights) && count < totalFlts ; count++ ) 
    {
        if ( strcmp(airportID,Flts[count].idDepart) == 0 ) 
        {
            tempFlt[j] = Flts[count];
            j++;
        }
    }
    /*Sorts all of the flights by time , from the latest to recent*/
    sortFlightsP(tempFlt,j);
    for ( count = 0 ; count < nPortFlights ; count++ ) 
    {
        printf("%s %s %02d-%02d-%d %02d:%02d\n",tempFlt[count].code,tempFlt[count].idArrival,
            tempFlt[count].departDate.day, tempFlt[count].departDate.month,
            tempFlt[count].departDate.year, tempFlt[count].departTime.hour, tempFlt[count].departTime.minute);
    }
    return;
}




/*sortFlightsP uses insertion sort to sort all of the flights from latest to recent*/
void sortFlightsP( Flights tempFlt[],int idx) 
{
    int mark, count;
    Flights key;
    for (mark = 1; mark < idx; mark++)
    {
        key = tempFlt[mark];
        count = mark - 1;
        while (count >= 0 && compareDates(tempFlt[count].departDate,tempFlt[count].departTime,
                key.departDate,key.departTime) == 0)
        {
            tempFlt[count + 1] = tempFlt[count];
            count = count - 1;
        }
        tempFlt[count + 1] = key;
    }
}

/*comparesDates compares two dates and returns 0 if keydate happens later than the flightsDate*/
int compareDates(date fltsDate , time fltsTime , date keyDate , time keyTime ) 
{
    if (keyDate.year > fltsDate.year || (keyDate.year == fltsDate.year && keyDate.month > fltsDate.month) 
             || (keyDate.year == fltsDate.year && keyDate.month == fltsDate.month && keyDate.day > fltsDate.day)) 
    {
        return 1;
    }
    else if ((keyDate.year == fltsDate.year) && (keyDate.month == fltsDate.month) && (keyDate.day == fltsDate.day)) 
    {
        if (keyTime.hour > fltsTime.hour ||(keyTime.hour == fltsTime.hour && keyTime.minute > fltsTime.minute)) 
        {
            return 1;
        }
        else {return 0;}
    }
    return 0;
}

/*command_c scans a arrivalID and if it exists , prints the arrivalflights with the corresponding arrivalDate and Time*/ 
void command_c() {
    Flights tempFlt[MAXFLT]; 
    char airportID[MAXIDX];
    int j=0,idx ,count;
    scanf("%s",airportID);
    if ( (idx = airportExists(airportID)) == -1 ) 
    {
        printf("%s: no such airport ID\n",airportID);
        return;
    }
    for ( count = 0 ; count < totalFlts ; count++ ) 
    {/*goes through all of the flights and if there is a matching arrivalAirport , then adds it to the temporary Flights vector*/
        if ( strcmp(airportID,Flts[count].idArrival) == 0 ) 
        {
            tempFlt[j] = Flts[count];
            /*Creates a new flight with the correct arrival Date and Time */
            tempFlt[j]=createArrivalDate(tempFlt[j]);
            j++;
        }
    }
    /*sorts all of the flights by their arrival Date and Time*/
    sortFlightsC(tempFlt,j);
    for ( count = 0 ; count < j ; count++ ) 
    {/*print flights by order of time */
        printf("%s %s %02d-%02d-%d %02d:%02d\n",tempFlt[count].code,tempFlt[count].idDepart,
                tempFlt[count].arrivalDate.day, tempFlt[count].arrivalDate.month,
               tempFlt[count].arrivalDate.year, tempFlt[count].arrivalTime.hour, tempFlt[count].arrivalTime.minute);
    }
    return;
}


/*createArrivalDate creates a ArivalDate and Time*/
Flights createArrivalDate (Flights flight) 
{
    /*has all of the days of the months by order*/
    int months[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    /*iniciates all of the values of the arrivalDate and arrivalTime*/
    flight.arrivalTime.minute = flight.departTime.minute +flight.durationTime.minute;
    flight.arrivalTime.hour = flight.departTime.hour +flight.durationTime.hour;
    flight.arrivalDate.day = flight.departDate.day;
    flight.arrivalDate.month = flight.departDate.month;
    flight.arrivalDate.year =  flight.departDate.year ;
    /* if any of the values exceeds its maximum , reset it as the diference between the maximum and its value and add 1 to next member*/
    if (flight.arrivalTime.minute > 59) 
    {
        flight.arrivalTime.minute -= 60;
        flight.arrivalTime.hour++;
    }
    if (flight.arrivalTime.hour > 23) 
    {
        flight.arrivalTime.hour -= 24;
        flight.arrivalDate.day++;
    }
    if ( (flight.arrivalDate.day) > months[flight.departDate.month]) 
    {
        flight.arrivalDate.day -= months[flight.departDate.month];
        flight.arrivalDate.month++;
    }
    if (flight.arrivalDate.month > 12) 
    {
        flight.arrivalDate.month -= 12;
        flight.arrivalDate.year++;
    }
    return flight;
}

/*sortFlightsC sorts flights by their arrivalDate and Time*/
void sortFlightsC( Flights tempFlt[],int idx) 
{
    int mark, count;
    Flights key;
    for (mark = 1; mark < idx; mark++)
    {
        key = tempFlt[mark];
        count = mark - 1;
        while (count >= 0 && compareDates(tempFlt[count].arrivalDate,tempFlt[count].arrivalTime,
                key.arrivalDate,key.arrivalTime) == 0)
        {
            tempFlt[count + 1] = tempFlt[count];
            count = count - 1;
        }
        tempFlt[count + 1] = key;
    }
}



/*command_t scans for a date and schanges its currentDate to the one recieved if its not a year in advance or a date in the past*/
void command_t() {
    date dates;
    int dd, mm, yy;
    scanf("%d-%d-%d", &dd, &mm, &yy);
    if (validateDate(dd, mm, yy) == 0){
        return;
    }
    currentDate = saveDate(dd, mm, yy, dates);
    printf("%02d-%02d-%02d\n", currentDate.day, currentDate.month, currentDate.year);
}