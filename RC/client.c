#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

//numarul de intrebari
int quizz_number;

//statusul jucatorului
bool gameOver=false;

int main (int argc, char *argv[])
{
  int sd;                       // descriptorul de socket
  struct sockaddr_in server;    // structura folosita pentru conectare

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  //ne logam prin username
  printf ("[client]Introduceti un nume: ");
  fflush (stdout);
  char name[10];
  scanf("%s", name);

  /* trimiterea mesajului la server */
  if (write (sd, name, 10) < 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

 //Citim in quizz_number numarul de intrebari 
  if(read(sd, &quizz_number, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server 1.\n");
      return errno;
    }
  printf("Felicitari, te-ai conectat cu succes!\n");

  printf("REGULAMENT!\nPentru a parasi jocul apasati tasta 0\n");
  printf("Raspunsul valid la intrebari este de forma: 1 | 2 | 3\n");
  printf("Jocul a inceput!\n");

while(quizz_number>0 && gameOver==false)
{
    char question[100];
    char answers[100];

  /* citirea raspunsului dat de server(apel blocant pana cand serverul raspunde) */
  if (read (sd, question, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server in while 1.\n");
      return errno;
    }
   if(read (sd, answers, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server in while 2.\n");
      return errno;
    }

  /* afisam mesajele primite */
    printf("\nIntrebarea %d: %s\n", quizz_number, question);
    printf("Raspunsuri: %s\n", answers);

INPUT:
    int myAnswer;
    printf("Raspuns: ");
    fflush(stdout);
    scanf("%d", &myAnswer);
    if(myAnswer==0 || myAnswer==1 || myAnswer==2 || myAnswer==3){
       if (write (sd,&myAnswer,sizeof(int)) < 0)
      {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
      }
    }
    else{
      printf("INPUT INCORECT, incercati din nou!\n");
      goto INPUT;
    }

    //feedback
    char feedback[100];
    if(read(sd, feedback, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server in while 3.\n");
      return errno;
    }
    if(strcmp(feedback,"gameOver=true")==0)
    {
      gameOver=true;
    }else {
    printf("%s\n",feedback);
    }

  quizz_number--;
}

  if(gameOver==true)
  {
    printf("GAME OVER!\n");
    return 0;
  }

  //citim scorul obtinut
  int score;
  if(read(sd, &score, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la client.\n");
      return errno;
    }
  printf("Ai obtinut %d puncte\n", score);

  //castigatorul jocului
  char winner[100];
  if(read(sd, winner, 100) < 0)
    {
      perror ("[client]Eroare la read() de la client.\n");
      return errno;
    }
  printf("\nCastigatorul jocului este: %s\n", winner);

  /* inchidem conexiunea, am terminat */
  close(sd);
}