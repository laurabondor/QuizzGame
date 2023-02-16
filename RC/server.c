#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "db.h"

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

//numarul de intrebari din joc
int quizz_number;

//timer
int timer = 5;

typedef struct thData
{
  int idThread; // id-ul thread-ului tinut in evidenta de acest program
  int cl;       // descriptorul intors de accept
  char name[10];
  bool disconnect;
} thData;

struct data{
  char name[10];
  int cl;
  int score; 
  bool finishQuizz; //a parcurs toate intrebarile
  bool gameOver; //este descalificat 
} clients[100];

struct win{
  char name[10];
  int score;
} winner;

int nr_players; //numarul de jucatori
int nr_finishQuizz_players; //numarul de jucatori care au raspuns la toate intrebarile
int nr_connected_players; //numarul de jucatori nedescalificati

void newGame()
{
  for(int i=0; i<nr_players; i++)
  {
    clients[i].score=0;
    close(clients[i].cl);
  }

  winner.score=0;
  nr_players=0;
  nr_finishQuizz_players=0;
  nr_connected_players=0;

  printf("Am inceput un nou joc!\n");
}

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main()
{
  struct sockaddr_in server; // structura folosita de server
  struct sockaddr_in from;
  int sd; // descriptorul de socket
  int pid;
  pthread_t th; 

  //cream tabela Quizz
  quizz_number=createDB(1);
  printf("ultimul id inserat: %d\n",quizz_number);

  /* crearea unui socket */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }
  /* utilizarea optiunii SO_REUSEADDR */
  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  /* pregatirea structurilor de date */
  bzero(&server, sizeof(server));
  bzero(&from, sizeof(from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
  server.sin_family = AF_INET;
  /* acceptam orice adresa */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  /* utilizam un port utilizator */
  server.sin_port = htons(PORT);

  /* atasam socketul */
  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen(sd, 2) == -1)
  {
    perror("[server]Eroare la listen().\n");
    return errno;
  }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
  {
    int client;
    thData *td; // parametru functia executata de thread
    int length = sizeof(from);

    printf("[server]Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    /* acceptam un client (stare blocanta pana la realizarea conexiunii) */
    if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
    {
      perror("[server]Eroare la accept().\n");
      continue;
    }

    /* s-a realizat conexiunea */
    td = (struct thData *)malloc(sizeof(struct thData));
    td->idThread = nr_players;
    nr_players++;
    td->cl = client;
    strcpy(td->name,"\0");
    td->disconnect = false;
    clients[td->idThread].cl = client;
    nr_connected_players++;

    pthread_create(&th, NULL, &treat, td);
  } // while
};

static void *treat(void *arg)
{
  struct thData tdL;
  tdL = *((struct thData *)arg);
  printf("[thread]- %d - Asteptam ca jucatorul sa se logheze...\n", tdL.idThread);
  fflush(stdout);

  //citim username-ul clientului
  char name[10];
  if(read(tdL.cl, name, 10) < 0)
  {
    printf("[Thread %d]\n", tdL.idThread);
    perror("Eroare la read() de la client.\n");
  }

  strcpy(tdL.name, name);
  strcpy(clients[tdL.idThread].name, name);
  clients[tdL.idThread].finishQuizz=false;
  clients[tdL.idThread].gameOver=false;

  printf("%s s-a alaturat (TID: %ld)\n", tdL.name, pthread_self());

  //trimitem clientului numarul de intrebari la care trebuie sa raspunda
  if(write(tdL.cl, &quizz_number, sizeof(int)) < 0)
  {
    perror("Eroare la write() spre client.\n");
  }

  //incepem jocul
  raspunde((struct thData *)arg);
  
  close((intptr_t)arg);

  if(nr_connected_players == nr_finishQuizz_players)
    {
      printf("Jocul s-a incheiat!\n");

      char castigator[100];
      sprintf(castigator, "%s - %d puncte\n", winner.name, winner.score);

      //trimitem jucatorilor castigatorul jocului
      for (int i=0; i<nr_players; i++)
      {
        if (clients[i].gameOver == false)
          {
            if(write(clients[i].cl, castigator, 100) < 0)
            {
               perror("Eroare la write() spre client.\n");
            }
            close(clients[i].cl);
          }       
      }

      //incepem un nou joc
      newGame();
    }

  return (NULL);
};

void raspunde(void *arg)
{
  struct thData tdL;
  tdL = *((struct thData *)arg);

  int i=1;
  while(i<=quizz_number)
  {
    if(tdL.disconnect==true) break;

    //Luam intrebarea si raspunsurile pt interogarea i si le trimitem jucatorului
    i_query(i);
    if(write(tdL.cl, question.q_text, 100) < 0)
    {
     printf("[Thread %d] ", tdL.idThread);
     perror("Eroare la write() spre client.\n");
    }
    if(write(tdL.cl, question.answers, 100) < 0)
    {
     printf("[Thread %d] ", tdL.idThread);
     perror("Eroare la write() spre client.\n");
    }

    //pregatim timerul
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Clear the file descriptor set */
    FD_ZERO(&rfds);
    FD_SET(clients[tdL.idThread].cl, &rfds);

    /* Set the timeout period */
    tv.tv_sec = timer; 

    /* Wait for data to be available on any socket */
    retval = select(clients[tdL.idThread].cl + 1, &rfds, NULL, NULL, &tv);

    //timpul a fost depasit
    if(retval==0)
    {
      printf("%s la intrebarea %d a depasit timpul alocat\n",clients[tdL.idThread].name, i);
      //citim raspunsul dar nu il luam in considerare la punctaj
      int cl_answer;
      if(read(tdL.cl, &cl_answer, sizeof(cl_answer)) < 0)
      {
        printf("[Thread %d]\n", tdL.idThread);
        perror("Eroare la read() de la client.\n");
      }
      //deconectam jucatorul
      if(cl_answer==0)
      {
        tdL.disconnect=true;
        nr_connected_players--;
        clients[tdL.idThread].gameOver=true;
        clients[tdL.idThread].finishQuizz=true;

        char feedback[100];
        strcpy(feedback, "gameOver=true");
        if(write(tdL.cl, feedback, 100) < 0)
        {
          printf("[Thread %d] ", tdL.idThread);
          perror("Eroare la write() spre client.\n");
        }

        printf("%s a parasit jocul!\n", clients[tdL.idThread].name);
        shutdown(clients[tdL.idThread].cl, SHUT_RDWR);
        close(clients[tdL.idThread].cl);
        break;
      }
      //feedback
      char feedback[100];
      clients[tdL.idThread].score += 0; //scorul ramane nemodificat
      strcpy(feedback, "Ai depasit intervalul de secunde! Raspunsul nu a fost punctat!\n");
      if(write(tdL.cl, feedback, 100) < 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("Eroare la write() spre client.\n");
      }
      
    //jucatorul a raspuns la timp
    }else{
    //Citim raspunsurile jucatorilor si le verificam corectitudinea
    int cl_answer;
    if(read(tdL.cl, &cl_answer, sizeof(cl_answer)) < 0)
    {
     printf("[Thread %d]\n", tdL.idThread);
     perror("Eroare la read() de la client.\n");
    }
    //deconectam jucatorul
    if(cl_answer==0)
    {
      tdL.disconnect=true;
      nr_connected_players--;
      clients[tdL.idThread].gameOver=true;
      clients[tdL.idThread].finishQuizz=true;

      char feedback[100];
      strcpy(feedback, "gameOver=true");
      if(write(tdL.cl, feedback, 100) < 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("Eroare la write() spre client.\n");
      }

      printf("%s a parasit jocul!\n", clients[tdL.idThread].name);
      shutdown(clients[tdL.idThread].cl, SHUT_RDWR);
      close(clients[tdL.idThread].cl);
      break;
    }
    //feedback: verificam raspunsurile
    char feedback[100];
    if(cl_answer==question.correct_ans)
    {
      clients[tdL.idThread].score += 10;
      if(clients[tdL.idThread].score > winner.score)
      {
        winner.score = clients[tdL.idThread].score;
        strcpy(winner.name, clients[tdL.idThread].name);
      }
      strcpy(feedback, "Ai raspuns corect!");
      if(write(tdL.cl, feedback, 100) < 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("Eroare la write() spre client.\n");
      }
    } else{
        strcpy(feedback, "Ai raspuns gresit! Raspunsul corect era ");
        char converted_number[5];
        sprintf(converted_number, "%d", question.correct_ans);
        strcat(feedback, converted_number);
        if(write(tdL.cl, feedback, 100) < 0)
         {
          printf("[Thread %d] ", tdL.idThread);
          perror("Eroare la write() spre client.\n");
         }
      }
    }

    //trecem la urmatoarea intrebare
    i++;
  }

  //jucatorul a parcurs toate intrebarile si nu este descalificat
  if(i==quizz_number+1 && tdL.disconnect==false){
    nr_finishQuizz_players++;
    clients[tdL.idThread].finishQuizz=true;

    //trimitem scorul pe care l-a obtinut jucatorul
    if(write(tdL.cl, &clients[tdL.idThread].score, sizeof(int)) <= 0)
    {
      printf("[Thread %d] ", tdL.idThread);
      perror("Eroare la write() spre client.\n");
    }
  }

  //jucatorul este desalificat
  if(tdL.disconnect==true) return;
}