#include <graphics.h>
#include<fstream>
#include <math.h>
#include <conio.h>
#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#define tip char
#define MAX 1000
using namespace std;

struct eroare{
    int pozitia;
    char err[100];
};

struct vectoor{
                char info[MAX];
                unsigned nr;
}vec[MAX],polonez[MAX], v[MAX];

struct nod {
		tip info[MAX];
		nod * urm;
};

struct stiva {
		nod* varf;
		int nrElemente;
}S;

struct nod_a {
        tip info[MAX];
        nod_a *st;
        nod_a *dr;
} *arb, *arb_der1, *arb_sim1, *arb_der2, *arb_der, *arb_sim2;
typedef nod_a * arbore;

struct nod_sa {
		nod_a *arb;
		nod_sa *urm;
};

struct stiva_a {
		nod_sa* varf;
		int nrElemente;
};

int prioritate(char op)   //Laura Bondor
{
int p;
switch (op) {
        case '(': case ')': p=0;
            break;
        case '+': case '-': p=1;
            break;
        case '*': case '/': p=2;
            break;
        case '^': p=3;
            break;
        case 's': case 'c': case 'l': case 'e': case 't': case 'a': p=4;
            break;
}
  return p;
}

void vectorizeaza(char expresie[], vectoor vec[])   //Laura Bondor
{
int i,j,k;
i=0; k=0;
while(i<strlen(expresie))
{
  if(strchr("scleta",expresie[i])!=NULL){
      switch(expresie[i]) {
        case 's': {
           if(expresie[i+1]=='q') { vec[k].info[0]=expresie[i];
                                    vec[k].info[1]=expresie[i+1];
                                    vec[k].info[2]=expresie[i+2];
                                    vec[k].info[3]=expresie[i+3];
                                    vec[k].info[4]='\0'; i+=4;}

           else { vec[k].info[0]=expresie[i];
                  vec[k].info[1]=expresie[i+1];
                  vec[k].info[2]=expresie[i+2];
                  vec[k].info[3]='\0'; i+=3; }
            break;
        }

        case 'c':
                 vec[k].info[0]=expresie[i];
                 vec[k].info[1]=expresie[i+1];
                 vec[k].info[2]=expresie[i+2];
                 vec[k].info[3]='\0';
                 i+=3;
                 break;

         case 'l': {
            switch(expresie[i+1])
                case 'n':
                case 'g':
                    vec[k].info[0]=expresie[i];
                    vec[k].info[1]=expresie[i+1];
                    vec[k].info[2]='\0';
                    i+=2;
                    break;
            }

        case 'e':
            vec[k].info[0]=expresie[i];
            vec[k].info[1]='\0';
            i++;
            break;

        case 't':
            vec[k].info[0]=expresie[i];
            vec[k].info[1]=expresie[i+1];
            vec[k].info[2]='\0';
            i+=2;
            break;

        case 'a': {
            if(expresie[i+3]=='t') { //arctg
                                     vec[k].info[0]=expresie[i];
                                     vec[k].info[1]=expresie[i+1];
                                     vec[k].info[2]=expresie[i+2];
                                     vec[k].info[3]=expresie[i+3];
                                     vec[k].info[4]=expresie[i+4];
                                     vec[k].info[5]='\0';
                                     i+=5; }
            else { //arccos arcsin arcctg
                  vec[k].info[0]=expresie[i];
                  vec[k].info[1]=expresie[i+1];
                  vec[k].info[2]=expresie[i+2];
                  vec[k].info[3]=expresie[i+3];
                  vec[k].info[4]=expresie[i+4];
                  vec[k].info[5]=expresie[i+5];
                  vec[k].info[6]='\0';
                  i+=6; }
                  }
                      }
   k++;
 }
  else if(strchr("(+-*/^",expresie[i])!=NULL) {
     vec[k].info[0]=expresie[i];
    vec[k].info[1]='\0';
     k++; i++;
 }
 else if(expresie[i]=='x') {
     vec[k].info[0]=expresie[i];
    vec[k].info[1]='\0';
     k++; i++;
     if(strchr("+-*/^)",expresie[i])==NULL) {vec[k].info[0]='*'; k++;}
 }
  else if(expresie[i]==')') {
     vec[k].info[0]=expresie[i];
    vec[k].info[1]='\0';
     k++; i++;
     if(strchr("+-*/^)",expresie[i])==NULL) {vec[k].info[0]='*'; vec[k].info[1]='\0'; k++;}
 }
  else{ // numar
     j=0;
       while(strchr("()+-*/^xscleta",expresie[i])==NULL)
       {
        vec[k].info[j]=expresie[i];
        j++; i++;
       }
       vec[k].info[j]='\0';
      k++;
      if(strchr("+-*/^)",expresie[i])==NULL) {vec[k].info[0]='*'; vec[k].info[1]='\0'; k++;}
     }
}
vec[0].nr=k;
}
bool isOperator(char c[10])   //Laura Bondor+ Iustina Apostol
{
    return (strlen(c)==1&&(c[0] == '+' || c[0] == '-' || c[0] == '*' || c[0] == '/' || c[0] == '^'));
}

bool isfunction(char c[10])   //Laura Bondor+ Iustina Apostol
{
    return (!(strcmp(c, "sin")&&strcmp(c, "cos")&&strcmp(c, "tg")&&strcmp(c, "ctg")&&strcmp(c, "ln")&&strcmp(c, "lg")&&strcmp(c, "sqrt")&&strcmp(c, "arctg")&&strcmp(c, "arccos")&&strcmp(c, "arcsin")&&strcmp(c, "arcctg")));
}
int isParanteza(char c[10])    //Iustina Apostol
{
    if(strlen(c)==1&&(c[0]=='('||c[0]=='['||c[0]=='{'))
        return 1;
    if(strlen(c)==1&&(c[0]==')'||c[0]==']'||c[0]=='}'))
        return -1;
    return 0;
}
bool isNumar(char c[10])    //Iustina Apostol
{
    if(c[0]=='e'&&strlen(c)==1)
        return 1;
    for(int i=0; i<strlen(c); i++)
        if(!strchr("1234567890", c[i]))
           return 0;
    return 1;
}
bool esteVida(stiva S)   //Laura Bondor
	{
		return S.nrElemente==0;
	}

void initializeaza(stiva& S)   //Laura Bondor
	{
		S.varf=NULL; S.nrElemente=0;
	}

void pop(stiva &S)   //(curs)
	// scoate elementul din varful stivei
	{
		if (!esteVida(S))
		{
		    nod* vf;
		    vf=S.varf->urm;
		    delete S.varf;
		    S.varf=vf;
		    S.nrElemente--;
		}

	}

void push(stiva &S, tip element[MAX])   //(curs)
	// adauga un element la stiva, in varf (LIFO)
	{
		nod * nod_nou;
		if (esteVida(S))
		{
			S.nrElemente=1;
			S.varf=new nod;
			strcpy(S.varf->info,element);
			S.varf->urm=NULL;
		}
		else
		{
			S.nrElemente++;
			nod_nou = new nod;
			strcpy(nod_nou->info,element);
			nod_nou->urm=S.varf;
			S.varf=nod_nou;
		}
	}


void forma_poloneza(vectoor vec[],vectoor polonez[])   //Laura Bondor
{
  unsigned i,j;
  initializeaza(S);

  j=0;
  for(i=0;i<vec[0].nr;i++) {
    if(strchr("x0123456789",vec[i].info[0])!=NULL) {strcpy(polonez[j].info,vec[i].info); j++;}
    else
        if(vec[i].info[0]=='(')
            push(S,"(");
    else
        if(vec[i].info[0]==')') {
            while(S.varf->info[0]!='(')
                  {
                    strcpy(polonez[j].info,S.varf->info); j++;
                    pop(S);
                  }
                  pop(S);
                              }
      else {
          while(!esteVida(S) && prioritate(vec[i].info[0])<=prioritate(S.varf->info[0])) {
            strcpy(polonez[j].info,S.varf->info);  j++;
            pop(S);
          }
          push(S,vec[i].info);
      }
}

while(esteVida(S)!=1) {
   strcpy(polonez[j].info,S.varf->info); j++;
   pop(S);
}
polonez[0].nr=j;
}

bool esteVida_a(stiva_a S)   //Laura Bondor
{
		return S.nrElemente==0;
}

void initializeaza_a(stiva_a& S)   //Laura Bondor
{
		S.varf=NULL; S.nrElemente=0;
}

void pop_a(stiva_a &S)   // (curs) Laura Bondor
// scoate elementul din varful stivei
{
		if (!esteVida_a(S))
		{
		    nod_sa* vf;
		    vf=S.varf->urm;
		    delete S.varf;
		    S.varf=vf;
		    S.nrElemente--;
		}

}

void push_a(stiva_a &S, nod_a *a)   //(curs) Laura Bondor
// adauga un element la stiva, in varf (LIFO)
{
		nod_sa* nod_nou;
		if (esteVida_a(S))
		{
			S.nrElemente=1;
			S.varf=new nod_sa;
			S.varf->arb=a;
			S.varf->urm=NULL;
		}
		else
		{
			S.nrElemente++;
			nod_nou = new nod_sa;
			nod_nou->arb=a;
			nod_nou->urm=S.varf;
			S.varf=nod_nou;
		}
}

char *inordine(arbore arb)   //Laura Bondor+ Iustina Apostol
{
    char *derivata;
    derivata=new char[MAX];
    strcpy(derivata, "");
    if(arb!=NULL){
      if(isOperator(arb->info)||isfunction(arb->info)) strcat(derivata, "(");
      strcat(derivata, inordine(arb->st));
      strcat(derivata,arb->info);
      strcat(derivata, inordine(arb->dr));
      if(isOperator(arb->info)||isfunction(arb->info)) strcat(derivata, ")");
    }
    return derivata;
}
void arborizeaza(vectoor polonez[],arbore &arb)   //Laura Bondor
{
 stiva_a S;
 initializeaza_a(S);

 unsigned i;
 arbore a;

 for(i=0;i<polonez[0].nr;i++) {
    if(isOperator(polonez[i].info)) {
        a=new nod_a;
        strcpy(a->info,polonez[i].info);
        a->dr=S.varf->arb;
        pop_a(S);
        a->st=S.varf->arb;
        pop_a(S);
        push_a(S,a);
    }
    else if(isfunction(polonez[i].info)) {
        a=new nod_a;
        strcpy(a->info,polonez[i].info);
        a->dr=S.varf->arb;
        a->st=NULL;
        pop_a(S);
        push_a(S,a);
    }
    else{
        a=new nod_a;
        strcpy(a->info,polonez[i].info);
        a->st=a->dr=NULL;
        push_a(S,a);
    }
 }
arb=a;
}

int tip1(char element[10])
{
    if(isNumar(element))
        return 1;
    if(isOperator(element))
        return 2;
    if(isfunction(element))
        return 3;
    if(isParanteza(element)==1)
        return 4;
    if(isParanteza(element)==-1)
        return -4;
    if(strlen(element)==1&&element[0]=='x')
        return 5;
    return 0;
}
char *verificare(vectoor functie[])     //Iustina Apostol
{
    char *eroare;
    eroare=new char[MAX];
    strcpy(eroare, "#");
    int i;
    if(tip1(functie[0].info)==-4||tip1(functie[0].info)==2)
    {
        strcpy(eroare, "EROARE in elementul: ");
        strcat(eroare, functie[0].info);
        return eroare;
    }
    for(i=0; i<functie[0].nr; i++)
    {
        switch(tip1(functie[i].info))
        {
        case 1:
            break;
        case 2:
            if(tip1(functie[i+1].info)==-4||tip1(functie[i+1].info)==2)
            {
                strcpy(eroare, "EROARE dupa operatorul ");
                strcat(eroare, functie[i].info);
                return eroare;
            }
            break;
        case 3:
            if(tip1(functie[i+1].info)==-4||tip1(functie[i+1].info)==3||tip1(functie[i+1].info)==2)
            {
                strcpy(eroare, "EROARE dupa functia ");
                strcat(eroare, functie[i].info);
                return eroare;
            }
            break;
        case 4:
            if(tip1(functie[i+1].info)==-4||tip1(functie[i+1].info)==2)
            {
                strcpy(eroare, "EROARE dupa ");
                strcat(eroare, functie[i].info);
                return eroare;
            }
            break;
        case -4:
            break;
        case 5:
            break;
        case 0:
            strcpy(eroare, "EROARE in elementul: ");
            strcat(eroare, functie[i].info);
            return eroare;
            break;
        default:
            break;
        }
    }
    return eroare;
}

eroare verificare_par(vectoor functie[])    //Iustina Apostol
{
    int pozitie=0, i;
    int sir[10]={0};
    eroare e;
    strcpy(e.err, "EROARE la paranteze");
    for(i=0; i<functie[0].nr; i++)
    {
        switch(functie[i].info[0])
        {
            case'(':
                pozitie++;
                sir[pozitie]=1;
                break;
            case'[':
                pozitie++;
                sir[pozitie]=2;
                break;
            case'{':
                pozitie++;
                sir[pozitie]=3;
                break;
            case')':
                if(sir[pozitie]!=1)
                {
                    e.pozitia=1;
                    return e;
                }
                else
                    pozitie--;
                break;
            case']':
                if(sir[pozitie]!=2)
                {
                    e.pozitia=1;
                    return e;
                }
                else
                    pozitie--;
                break;
            case'}':
                if(sir[pozitie]!=3)
                {
                    e.pozitia=1;
                    return e;
                }
                else
                    pozitie--;
                break;
            default:
                break;
        }
    }
    if(pozitie!=0)
        return e;
    strcpy(e.err, "");
    e.pozitia=0;
    return e;
}
int parcurgere(arbore arb, arbore arb1)    //Iustina Apostol
{
    if(arb==NULL&&arb1==NULL)
        return 0;
    if((arb==NULL||arb1==NULL))
        return 1;
    if(!strcmp(arb->info, arb1->info))
    {
        if(parcurgere(arb->dr, arb1->dr))
            return 1;
        if(parcurgere(arb->st, arb1->st))
            return 1;
        return 0;
    }
    return 1;

}
int impartire(arbore arb)    //Iustina Apostol
{
    if(arb==NULL)
        return 1;
    if(arb->info[0]=='/')
    {
        if(arb->dr->info[0]=='0')
            return 0;
        if(arb->dr->info[0]=='-')
        {
            if(parcurgere(arb->dr->dr, arb->dr->st)==0)
                return 0;
            return 1;
        }
        if(arb->dr->info[0]=='*')
        {
            if(arb->dr->dr->info[0]=='0'||arb->dr->st->info[0]=='0')
                return 0;
        }
        return 1;
    }
    impartire(arb->dr);
    impartire(arb->st);
}

void stergeK(vectoor lista[], int pozitie)    //Iustina Apostol
{
    for(int i=pozitie; i<lista[0].nr-1; i++)
        strcpy(lista[i].info, lista[i+1].info);
    lista[0].nr--;
}
void sterge(char element[], char deSters)    //Iustina Apostol
{
    int numar=strlen(element);
    for(int i=0; i<numar; i++)
    {
        if(element[i]==deSters)
        {
            strcpy(element+i, element+i+1);
            numar--;
            i--;
        }
    }
}
void stergereSpatii(vectoor functie[])    //Iustina Apostol
{
    for(int i=0; i<functie[0].nr; i++)
    {
        sterge(functie[i].info, ' ');
        if(functie[i].info[0]==' '|| functie[i].info[0]=='\0')
            stergeK(functie, i);
    }
}

void log(arbore &arb)    //Iustina Apostol
{
    if(arb==NULL)
        return;
    arbore arb_dr, arb_st, sterge;
    if(arb->info[0]=='l')
    {
    switch(arb->info[1]){
        case 'n':
            if(arb->dr->info[0]=='^')
            {
                arb_dr=arb->dr->dr;
                arb_st=arb->dr->st;
                strcpy(arb->info, "*");
                arb->st=arb_dr;
                strcpy(arb->dr->info, "ln");
                arb->dr->dr=arb_st;
                arb->dr->st=NULL;

            }
            break;
        default:
            break;
    }
    }
    log(arb->st);
    log(arb->dr);
}


void deriveaza(arbore arb,arbore &arb_der)   //Laura Bondor
{
  arbore arb1,arb2,arb3,arb4,arb5,arb6,arb7,arb8;
  arb_der=new nod_a;
  strcpy(arb_der->info,"new");
  arb_der->st=arb_der->dr=NULL;
  switch (arb->info[0]) {
       case '0': case '1': case '2': case '3': case '4': case 'e':
       case '5': case '6': case '7': case '8': case '9':
           strcpy(arb_der->info,"0");
            break;

       case 'x':
            strcpy(arb_der->info,"1");
           break;

       case '+':
       case '-':
            deriveaza(arb->st,arb3); arb_der->st=arb3;
            deriveaza(arb->dr,arb4); arb_der->dr=arb4;
            if(arb->info[0]=='+') strcpy(arb_der->info,"+");
             else strcpy(arb_der->info,"-");
          break;

        case '*': // (f*g)= f'*g + f*g'
            arb1=new nod_a; arb2=new nod_a;
            deriveaza(arb->st,arb3);
            deriveaza(arb->dr,arb4);
            strcpy(arb_der->info,"+"); arb1->st=arb3; arb1->dr=arb->dr;
            strcpy(arb1->info,"*"); arb2->st=arb->st; arb2->dr=arb4;
            strcpy(arb2->info,"*"); arb_der->st=arb1; arb_der->dr=arb2;
            break;

        case '/': // (f/g)= (f'*g - f*g')/g^2
            arb1=new nod_a; arb2=new nod_a; arb5=new nod_a;
            deriveaza(arb->st,arb3); deriveaza(arb->dr,arb4);
            strcpy(arb1->info,"*"); arb1->st=arb->st; arb1->dr=arb4;
            strcpy(arb2->info,"*"); arb2->st=arb3; arb2->dr=arb->dr;
            strcpy(arb5->info,"-"); arb5->st=arb2; arb5->dr=arb1;
            arb6=new nod_a; arb7=new nod_a;
            strcpy(arb6->info,"2"); arb6->st=arb6->dr=NULL;
            strcpy(arb7->info,"^"); arb7->st=arb->dr; arb7->dr=arb6;
            strcpy(arb_der->info,"/"); arb_der->st=arb5; arb_der->dr=arb7;
            break;

        case 's': {
           if(strcmp(arb->info,"sin")==0) {
               arb1=new nod_a; strcpy(arb1->info,"cos");
               arb1->st=NULL; arb1->dr=arb->dr;
               deriveaza(arb->dr,arb2);
               strcpy(arb_der->info,"*"); arb_der->st=arb1; arb_der->dr=arb2;
               }
            else{ //sqrt
               arb2=new nod_a; arb3=new nod_a;
               strcpy(arb3->info,"2"); arb3->st=arb3->dr=NULL;
               strcpy(arb2->info,"*"); arb2->st=arb3; arb2->dr=arb;
               deriveaza(arb->dr,arb1);
               strcpy(arb_der->info,"/"); arb_der->st=arb1; arb_der->dr=arb2;
                }
               break;
               }

        case 'c': {
           if(arb->info[1]=='o') { //cos
               deriveaza(arb->dr,arb4);
               arb1=new nod_a; arb2=new nod_a; arb3=new nod_a;
               strcpy(arb1->info,"0"); arb1->st=arb1->dr=NULL;
               strcpy(arb2->info,"sin"); arb2->dr=arb->dr; arb2->st=NULL;
               strcpy(arb3->info,"*"); arb3->st=arb2; arb3->dr=arb4;
               strcpy(arb_der->info,"-"); arb_der->st=arb1; arb_der->dr=arb3; }
          else{ //ctg
               arb4=new nod_a; arb5=new nod_a; arb6=new nod_a;
               strcpy(arb5->info,"sin"); arb5->st=NULL; arb5->dr=arb->dr;
               strcpy(arb6->info,"2"); arb6->st=arb6->dr=NULL;
               strcpy(arb4->info,"^"); arb4->st=arb5; arb4->dr=arb6;
               deriveaza(arb->dr,arb2);
               arb1=new nod_a; strcpy(arb1->info,"/"); arb1->st=arb2; arb1->dr=arb4;
               arb3=new nod_a; strcpy(arb3->info,"0"); arb3->st=arb3->dr=NULL;
               strcpy(arb_der->info,"-"); arb_der->st=arb3; arb_der->dr=arb1; }
              break;
                  }

        case 'l': {
          if(strcmp(arb->info,"ln")==0) {
            deriveaza(arb->dr,arb1);
            strcpy(arb_der->info,"/"); arb_der->st=arb1; arb_der->dr=arb->dr;
            }
          else
            if(strcmp(arb->info,"lg")==0){
              deriveaza(arb->dr,arb1);
              arb2=new nod_a; arb2=arb->dr;
              arb4=new nod_a; strcpy(arb4->info,"10"); arb4->st=arb4->dr=NULL;
              arb3=new nod_a; strcpy(arb3->info,"ln"); arb3->st=NULL; arb3->dr=arb4;
              arb5=new nod_a; strcpy(arb5->info,"*"); arb5->st=arb2; arb5->dr=arb3;
              strcpy(arb_der->info,"/"); arb_der->st=arb1; arb_der->dr=arb5;
          }
                }
        case 't': // tg
            arb4=new nod_a; arb5=new nod_a; arb6=new nod_a;
            strcpy(arb5->info,"cos"); arb5->st=NULL; arb5->dr=arb->dr;
            strcpy(arb6->info,"2"); arb6->st=arb6->dr=NULL;
            strcpy(arb4->info,"^"); arb4->st=arb5; arb4->dr=arb6;
            deriveaza(arb->dr,arb2);
            strcpy(arb_der->info,"/"); arb_der->st=arb2; arb_der->dr=arb4;
            break;

        case '^': {

          if(arb->st->info[0]=='e') { // (e^u)' = e^u * u'
            deriveaza(arb->dr,arb2);
            strcpy(arb_der->info,"*");
            arb_der->st=arb; arb_der->dr=arb2;
          }
          else if(strchr("1234567890",arb->st->info[0])!=NULL) { //(a^u)' = a^u * ln a * u'
            arb4=new nod_a; arb3=new nod_a; arb2=new nod_a;
            strcpy(arb4->info,arb->st->info); arb4->st=arb4->dr=NULL;
            strcpy(arb3->info,"ln"); arb3->st=NULL; arb3->dr=arb4;
            strcpy(arb2->info,"*"); arb2->st=arb; arb2->dr=arb3;
            deriveaza(arb->dr,arb1);
            strcpy(arb_der->info,"*");  arb_der->st=arb2; arb_der->dr=arb1;
          }
          else { // (u^n)' = n* u^(n-1) *u'
             arb5=new nod_a; arb4=new nod_a; arb3=new nod_a; arb2=new nod_a;
             strcpy(arb5->info,"1"); arb5->st=arb5->dr=NULL;
             strcpy(arb4->info,"-"); arb4->st=arb->dr; arb4->dr=arb5;
             strcpy(arb3->info,"^"); arb3->st=arb->st; arb3->dr=arb4;
             strcpy(arb2->info,"*"); arb2->st=arb->dr; arb2->dr=arb3;
             deriveaza(arb->st,arb1);
             strcpy(arb_der->info,"*");  arb_der->st=arb2; arb_der->dr=arb1;
            }
            break;
              }

        case 'a': {

        if(strcmp(arb->info,"arcsin")==0) {
            arb7=new nod_a; strcpy(arb7->info,"2"); arb7->st=arb7->dr=NULL;
            arb6=new nod_a; strcpy(arb6->info,"^"); arb6->st=arb->dr; arb6->dr=arb7;
            arb5=new nod_a; strcpy(arb5->info,"1"); arb5->st=arb5->dr=NULL;
            arb4=new nod_a; strcpy(arb4->info,"-"); arb4->st=arb5; arb4->dr=arb6;
            arb3=new nod_a; strcpy(arb3->info,"sqrt"); arb3->st=NULL; arb3->dr=arb4;
            deriveaza(arb->dr,arb2);
            strcpy(arb_der->info,"/");  arb_der->st=arb2; arb_der->dr=arb3;
            }
         else if(strcmp(arb->info,"arccos")==0) {
            arb7=new nod_a; strcpy(arb7->info,"2"); arb7->st=arb7->dr=NULL;
            arb6=new nod_a; strcpy(arb6->info,"^"); arb6->st=arb->dr; arb6->dr=arb7;
            arb5=new nod_a; strcpy(arb5->info,"1"); arb5->st=arb5->dr=NULL;
            arb4=new nod_a; strcpy(arb4->info,"-"); arb4->st=arb5; arb4->dr=arb6;
            arb3=new nod_a; strcpy(arb3->info,"sqrt"); arb3->st=NULL; arb3->dr=arb4;
            deriveaza(arb->dr,arb2);
            arb1=new nod_a; strcpy(arb1->info,"/"); arb1->st=arb2; arb1->dr=arb3;
            arb8=new nod_a; strcpy(arb8->info,"0"); arb8->st=arb8->dr=NULL;
            strcpy(arb_der->info,"-");  arb_der->st=arb8; arb_der->dr=arb1;
            }
         else if(strcmp(arb->info,"arctg")==0) {
            arb6=new nod_a; strcpy(arb6->info,"2"); arb6->st=arb6->dr=NULL;
            arb5=new nod_a; strcpy(arb5->info,"^"); arb5->st=arb->dr; arb5->dr=arb6;
            arb4=new nod_a; strcpy(arb4->info,"1"); arb4->st=arb4->dr=NULL;
            arb3=new nod_a; strcpy(arb3->info,"+"); arb3->st=arb5; arb3->dr=arb4;
            deriveaza(arb->dr,arb2);
            strcpy(arb_der->info,"/");  arb_der->st=arb2; arb_der->dr=arb3;
            }
          else if(strcmp(arb->info,"arcctg")==0) {
            arb6=new nod_a; strcpy(arb6->info,"2"); arb6->st=arb6->dr=NULL;
            arb5=new nod_a; strcpy(arb5->info,"^"); arb5->st=arb->dr; arb5->dr=arb6;
            arb4=new nod_a; strcpy(arb4->info,"1"); arb4->st=arb4->dr=NULL;
            arb3=new nod_a; strcpy(arb3->info,"+"); arb3->st=arb5; arb3->dr=arb4;
            deriveaza(arb->dr,arb2);
            arb1=new nod_a; strcpy(arb1->info,"/"); arb1->st=arb2; arb1->dr=arb3;
            arb8=new nod_a; strcpy(arb8->info,"0"); arb8->st=arb8->dr=NULL;
            strcpy(arb_der->info,"-");  arb_der->st=arb8; arb_der->dr=arb1;
            }
            break;
              }
    }
}
int toInt(char s[MAX])
{
  int num = 0;
    for (int i=0; i<strlen(s); i++)
        num = num*10 + (int(s[i])-48);

 return num;
}

char* convertIntegerToChar(int N)
{
  int m = N;
  int digit = 0;
  while (m) {
           digit++;
           m /= 10;
           }
  char* arr;
  char arr1[digit];
  arr = (char*)malloc(digit);
  int index = 0;
  while (N) {
          arr1[++index] = N % 10 + '0';
          N /= 10;
         }
  int i;
  for (i = 0; i < index; i++) {
          arr[i] = arr1[index - i];
      }
  arr[i] = '\0';

    return (char*)arr;
}


int ok1=1;
void simplifica(arbore &arb)   //Laura Bondor
{
 switch (arb->info[0]) {

     case '+': {
      if(arb->st!=NULL)
         if(strchr("*+-^/",arb->st->info[0])!=NULL)
            simplifica(arb->st);
      if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            simplifica(arb->dr);
      if(arb->st!=NULL && arb->dr!=NULL)
         if(strcmp(arb->st->info,"0")==0)
                                  {arb=arb->dr; ok1=0;}
         else
            if(strcmp(arb->dr->info,"0")==0)
                                  {arb=arb->st; ok1=0;}
         break;
        }

     case '-': {
        if(arb->st!=NULL)
            if(strchr("*+-^/",arb->st->info[0])!=NULL)
              simplifica(arb->st);
        if(arb->dr!=NULL)
            if(strchr("*+-^/",arb->dr->info[0])!=NULL)
              simplifica(arb->dr);
         if(arb->st!=NULL && arb->dr!=NULL)
             if(strcmp(arb->dr->info,"0")==0)
                {arb=arb->st; ok1=0;}
            else
              if(strcmp(arb->st->info,"0")==0)
                {delete arb->st; arb->st=0; ok1=0;}
        break;
       }

     case '*': {
        if(arb->st!=NULL)
          if(strchr("*+-^/",arb->st->info[0])!=NULL)
            simplifica(arb->st);
        if(arb->dr!=NULL)
          if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            simplifica(arb->dr);
        if(arb->st!=NULL && arb->dr!=NULL)
          if(strcmp(arb->st->info,"1")==0)
                              {arb=arb->dr; ok1=0;}
          else
            if(strcmp(arb->dr->info,"1")==0)
                              {arb=arb->st; ok1=0;}
          else
            if(strcmp(arb->st->info,"0")==0 || strcmp(arb->dr->info,"0")==0) {
              strcpy(arb->info,"0");
              arb->st=arb->dr=NULL;
              ok1=0;
            }
        break;
       }

     case '^': {
        if(arb->st!=NULL)
          if(strchr("*+-^/",arb->st->info[0])!=NULL)
            simplifica(arb->st);
        if(arb->dr!=NULL)
          if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            simplifica(arb->dr);
        if(arb->st!=NULL && arb->dr!=NULL)
           if(strcmp(arb->dr->info,"1")==0)
                            {arb=arb->st; ok1=0;}
          else
           if(strcmp(arb->st->info,"1")==0)
                            {arb=arb->dr; ok1=0;}
          else
           if(strcmp(arb->dr->info,"0")==0) {
               strcpy(arb->info,"1");
               arb->st=arb->dr=NULL;
               ok1=0;
           }
        break;
       }

     case '/': {
       if(arb->st!=NULL)
         if(strchr("*+-^/",arb->st->info[0])!=NULL)
            simplifica(arb->st);
       if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            simplifica(arb->dr);
       if(arb->st!=NULL && arb->dr!=NULL)
         if(strcmp(arb->dr->info,"0")==0)
                {
                  strcpy(arb->info,"0");
                  arb->st=arb->dr=NULL;
                  ok1=0;
                }
         else
           if(strcmp(arb->dr->info,"1")==0)
                        {arb=arb->st; ok1=0; }
       break;
      }

      case 's': case 'c': case 'l': case 't': case 'a':
        if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            simplifica(arb->dr);
      break;
}
}

 int  ok2=1;
void calculeaza(arbore &arb)   //Laura Bondor
{
 arbore a;
 switch (arb->info[0]) {

     case '+': {
    if(arb->st!=NULL)
        if(strchr("*+-^/",arb->st->info[0])!=NULL)
           calculeaza(arb->st);
    if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            calculeaza(arb->dr);
    if(arb->st!=NULL && arb->dr!=NULL)
         if(strchr("1234567890",arb->st->info[0])!=NULL &&
             strchr("1234567890",arb->dr->info[0])!=NULL) { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st+dr;
               char* arr = convertIntegerToChar(rezultat);
               strcpy(arb->info,arr);
               arb->st=arb->dr=NULL;
             }
          else
            if(arb->st->info[0]=='-' && arb->st->st==NULL && arb->st->dr!=NULL &&
                strchr("1234567890",arb->st->dr->info[0])!=NULL && strchr("1234567890",arb->dr->info[0])!=NULL)
            { ok2=0;
               int st=toInt(arb->st->dr->info);
               int dr=toInt(arb->dr->info);
               if(st<=dr) {
                 int rezultat=dr-st;
                 char* arr = convertIntegerToChar(rezultat);
                 strcpy(arb->info,arr);
                 arb->st=arb->dr=NULL;
                }
               else {
                int rezultat=st-dr;
                char* arr = convertIntegerToChar(rezultat);
                a=new nod_a;
                strcpy(a->info,arr);
                a->st=a->dr=NULL;
                strcpy(arb->info,"-");
                arb->st=NULL; arb->dr=a;
               }
             }
           else
             if(arb->dr->info[0]=='-' && arb->dr->st==NULL && arb->dr->dr!=NULL &&
                strchr("1234567890",arb->dr->dr->info[0])!=NULL && strchr("1234567890",arb->st->info[0])!=NULL)
             { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->dr->info);
               if(st>=dr) {
                 int rezultat=st-dr;
                 char* arr = convertIntegerToChar(rezultat);
                 strcpy(arb->info,arr);
                 arb->st=arb->dr=NULL;
                }
               else {
                int rezultat=dr-st;
                char* arr = convertIntegerToChar(rezultat);
                a=new nod_a;
                strcpy(a->info,arr);
                a->st=a->dr=NULL;
                strcpy(arb->info,"-");
                arb->st=NULL; arb->dr=a;
               }
             }
          else
            if(arb->st->st!=NULL && arb->st->dr!=NULL && arb->dr->st!=NULL && arb->dr->dr!=NULL)
            {
              arbore s; arbore d;
              s=arb->st; d=arb->dr;
              int da=1;
              while(s->dr!=NULL && d->dr!=NULL && da==1)
                {
                  s=s->dr; d=d->dr;
                  if(strcmp(s->info,d->info)!=0) da=0;
                }
             if(da) { //trebuie inclus si cazul in care e -
               int st=toInt(arb->st->st->info);
               int dr=toInt(arb->dr->st->info);
               int rezultat=dr+st;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=arb->dr->dr;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
               ok2=0;
             }
          }
         break;
        }

     case '-': {
        if(arb->st!=NULL)
           if(strchr("*+-^/",arb->st->info[0])!=NULL)
              calculeaza(arb->st);
        if(arb->dr!=NULL)
           if(strchr("*+-^/",arb->dr->info[0])!=NULL)
              calculeaza(arb->dr);
        if(arb->st!=NULL && arb->dr!=NULL)
           if(strchr("1234567890",arb->st->info[0])!=NULL &&
              strchr("1234567890",arb->dr->info[0])!=NULL) { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->info);
               if(st<dr) {
                 int rezultat=dr-st;
                 char* arr = convertIntegerToChar(rezultat);
                 a=new nod_a;
                 strcpy(a->info,arr);
                 a->st=a->dr=NULL;
                 strcpy(arb->info,"-");
                 arb->st=NULL; arb->dr=a;
                }
               else if(st>=dr) {
                int rezultat=st-dr;
                char* arr = convertIntegerToChar(rezultat);
                strcpy(arb->info,arr);
                arb->st=arb->dr=NULL;
               }
                }
            else
             if(arb->st->info[0]=='-' && arb->st->st==NULL && arb->st->dr!=NULL &&
                 strchr("1234567890",arb->st->dr->info[0])!=NULL && strchr("1234567890",arb->dr->info[0])!=NULL)
            { ok2=0;
               int st=toInt(arb->st->dr->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st+dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               strcpy(arb->info,"-");
               arb->st=NULL; arb->dr=a;
           }
           else
             if(arb->dr->info[0]=='-' && arb->dr->st==NULL && arb->dr->dr!=NULL &&
               strchr("1234567890",arb->dr->dr->info[0])!=NULL && strchr("1234567890",arb->st->info[0])!=NULL)
             { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->dr->info);
               int rezultat=st+dr;
               char* arr = convertIntegerToChar(rezultat);
               strcpy(arb->info,arr);
               arb->st=arb->dr=NULL;
             }
           else
            if(arb->st->st!=NULL && arb->st->dr!=NULL && arb->dr->st!=NULL && arb->dr->dr!=NULL
               && arb->st->dr->st!=NULL && arb->dr->dr->st!=NULL
               && strchr("1234567890",arb->st->dr->st->info[0])==NULL && strchr("1234567890",arb->dr->dr->st->info[0])==NULL)
            {
              arbore s; arbore d;
              s=arb->st; d=arb->dr;
              int da=1;
              while(s->dr!=NULL && d->dr!=NULL && da==1)
                {
                  s=s->dr; d=d->dr;
                  if(strcmp(s->info,d->info)!=0) da=0;
                }
             if(da) { //trebuie includ si cazul in care e - in arb->st->st
               int st=toInt(arb->st->st->info);
               int dr=toInt(arb->dr->st->info);
               if(st>=dr) { ok2=0;
               int rezultat=st-dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=arb->dr->dr;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
               }
               else{ ok2=0;
               int rezultat=dr-st;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               strcpy(arb->dr->info,"*");
               arb->dr->st=a;
               delete arb->st;
               arb->st=0;
               }
             }
          }
       break;
      }

     case '*': {
        if(arb->st!=NULL)
          if(strchr("*+-^/",arb->st->info[0])!=NULL)
            calculeaza(arb->st);
        if(arb->dr!=NULL)
          if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            calculeaza(arb->dr);
        if(arb->st!=NULL && arb->dr!=NULL)
          if(strchr("1234567890",arb->st->info[0])!=NULL &&
             strchr("1234567890",arb->dr->info[0])!=NULL) { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arb=a;
             }
          else
            if(arb->st->info[0]=='-' && arb->st->st==NULL && arb->st->dr!=NULL &&
               strchr("1234567890",arb->st->dr->info[0])!=NULL && strchr("1234567890",arb->dr->info[0])!=NULL)
            { ok2=0;
               int st=toInt(arb->st->dr->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               strcpy(arb->info,"-");
               arb->st=NULL; arb->dr=a;
             }
          else
           if(arb->dr->info[0]=='-' && arb->dr->st==NULL && arb->dr->dr!=NULL &&
              strchr("1234567890",arb->dr->dr->info[0])!=NULL && strchr("1234567890",arb->st->info[0])!=NULL)
           { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               strcpy(arb->info,"-");
               arb->st=NULL; arb->dr=a;
           }
         else
           if(arb->dr->info[0]=='*' && arb->dr->dr!=NULL)
        {
            if(strchr("1234567890",arb->st->info[0])!=NULL && strchr("1234567890",arb->dr->st->info[0])!=NULL)
              { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->st->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=new nod_a;
               a1=arb->dr->dr;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
             }
           else
            if(strchr("1234567890",arb->st->info[0])!=NULL && strchr("1234567890",arb->dr->dr->info[0])!=NULL)
             { ok2=0;
              int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=new nod_a;
               a1=arb->dr->st;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
             }
           }
         else
           if(arb->st->info[0]=='*' && arb->st->dr!=NULL)
          {
            if(strchr("1234567890",arb->dr->info[0])!=NULL && strchr("1234567890",arb->st->st->info[0])!=NULL)
              { ok2=0;
               int st=toInt(arb->st->st->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=new nod_a;
               a1=arb->st->dr;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
             }
           else if(strchr("1234567890",arb->dr->info[0])!=NULL && strchr("1234567890",arb->st->dr->info[0])!=NULL)
             { ok2=0;
               int st=toInt(arb->st->dr->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st*dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arbore a1;
               a1=new nod_a;
               a1=arb->st->st;
               strcpy(arb->info,"*");
               arb->st=a; arb->dr=a1;
             }
           }
        break;
       }

     case '^': {
        if(arb->st!=NULL)
          if(strchr("*+-^/",arb->st->info[0])!=NULL)
            calculeaza(arb->st);
        if(arb->dr!=NULL)
          if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            calculeaza(arb->dr);
        if(arb->st!=NULL && arb->dr!=NULL)
          if(strchr("123456789",arb->st->info[0])!=NULL &&
             strchr("123456789",arb->dr->info[0])!=NULL) { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->info);
               int rezultat=pow(st,dr);
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arb=a;
             }
        break;
       }

     case '/':{
       if(arb->st!=NULL)
         if(strchr("*+-^/",arb->st->info[0])!=NULL)
            calculeaza(arb->st);
       if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            calculeaza(arb->dr);
       if(arb->st!=NULL && arb->dr!=NULL)
         if(strchr("1234567890",arb->st->info[0])!=NULL &&
            strchr("1234567890",arb->dr->info[0])!=NULL) { ok2=0;
               int st=toInt(arb->st->info);
               int dr=toInt(arb->dr->info);
               int rezultat=st/dr;
               char* arr = convertIntegerToChar(rezultat);
               a=new nod_a;
               strcpy(a->info,arr);
               a->st=a->dr=NULL;
               arb=a;
             }
         //else {  ok=0;


         //}
     break;
     }

     case 's': case 'c': case 'l': case 't': case 'a':
        if(arb->dr!=NULL)
         if(strchr("*+-^/",arb->dr->info[0])!=NULL)
            calculeaza(arb->dr);
      break;

}
}

int calc_ok;
char *nesimplif;
void deriv(char functie[1000], char derivata[1000])   //Iustina Apostol
{
    strcpy(derivata, "");
    nesimplif=new char[MAX];
    strcpy(nesimplif, "");
    calc_ok=0;
    for(int i=0; i<vec[0].nr; i++)
    {
        strcpy(vec[i].info, "");
    }
    arb_sim1=NULL;
    eroare e;
    char *aux, *er;
    er=new char[100];
    strcpy(er, "");
    aux=new char[MAX];
    strcpy(aux, "");
    vectorizeaza(functie,vec);
    e=verificare_par(vec);
    if(e.pozitia!=0)
    {
        strcpy(derivata, "EROARE: paranteze");
        return;
    }
    stergereSpatii(vec);
    er=verificare(vec);
    if(er[0]!='#')
    {
        strcpy(derivata, er);
        return;
    }
    forma_poloneza(vec,polonez);
    arborizeaza(polonez,arb);
    if(impartire(arb)==0)
    {
        strcpy(derivata, "EROARE: *impartire la 0");
        return;
    }
    log(arb);
    deriveaza(arb,arb_sim1);
    nesimplif=inordine(arb_sim1);
    int ok=0;
    do{
    ok1=1; ok2=1;
    simplifica(arb_sim1);
    calculeaza(arb_sim1);
    ok=(ok1 && ok2);
    }while(ok==0);
    aux=inordine(arb_sim1);
    strcpy(derivata, aux);
    calc_ok=1;
}
void text(int x, int y, char *n="    ")    //Iustina Apostol
{
  outtextxy(x, y, n);
}
void text1(int x, int y, char n[1000])    //Iustina Apostol
{
    char *p;
    int linie=0;
    if(!strchr(n, '%'))
    {
        outtextxy(x,y,n);
    }

    else{
        p=strtok(n, "%");
        while(p!=NULL)
        {
            outtextxy(x, y+linie*20, p);
            linie++;
            p=strtok(NULL, "%");
        }
    }

}

ifstream fin("fisier.txt");
ofstream fout("fisier.txt");

void pune_in_fisier(arbore arb)   //Laura Bondor
{
    if(arb!=NULL){
      pune_in_fisier(arb->st);
      fout<<arb->info;
      pune_in_fisier(arb->dr);
    }
}

void pune_in_vector(vectoor v[])   //Laura Bondor
{
    fout.close();
    char sir[MAX];
    fin>>sir;
    fin.close();
    vectorizeaza(sir,v);
}

int tip_expresie;
void tip_ecuatie()   //Laura Bondor
{
  for(int i=0; i<v[0].nr; i++)
  {
    if(strcmp(v[i].info,"x")==0)
    {
        if(strcmp(v[i+1].info,"^")==0)
          if(strcmp(v[i+2].info,"2")==0)
          {
               tip_expresie=2;
              return;
          }
            else{tip_expresie=0; return;}
        cout<<v[i].info<<" ";
    }

    if(strcmp(v[i].info,"x")==0 && tip_expresie!=2) tip_expresie=1;
    if(strchr("sltc/", v[i].info[0])) {tip_expresie=0; return;}
  }
}

void afla_x()   //Laura Bondor
{
  int i,a=0,b=0,c=0;

 if(tip_expresie==1) // ecuatie gr I
 {

    if(v[0].info[0]=='-' && strchr("123456789",v[1].info[0])!=NULL && v[2].info[0]=='*') { a=toInt(v[1].info); a*=-1; }
    else  if(v[0].info[0]=='-' && strchr("123456789",v[1].info[0])!=NULL && v[2].info[0]!='*') {b=toInt(v[1].info); b*=-1; }
    else if(v[0].info[0]=='-' && v[1].info[0]=='x') a=-1;
    else if(v[0].info[0]=='x') a=1;
    else if(strchr("123456789",v[0].info[0])!=NULL && v[1].info[0]=='*') a=toInt(v[0].info);
    else  if(strchr("123456789",v[0].info[0])!=NULL && v[1].info[0]!='*') b=toInt(v[0].info);

    for(i=1;i<v[0].nr;i++)
       {
           if(strchr("123456789",v[i].info[0])!=NULL && a!=0)
                if(v[i-1].info[0]=='-') {b=toInt(v[i].info); b*=-1; }
                 else b=toInt(v[i].info);
          else if(strchr("123456789",v[i].info[0])!=NULL && b!=0)
               if(v[i-1].info[0]=='-') {a=toInt(v[i].info); a*=-1;}
                 else a=toInt(v[i].info);
       }

    int variabila=-b/a;
    char s1[30],s2[30];
    char s_a[6],s_b[6];
    char* arr;

    strcpy(s1,"a=");
    if(a<0)
        { a*=-1;
          arr = convertIntegerToChar(a);
          strcpy(s_a,"-");
          strcat(s_a,arr);
         }
     else
        if(a>0)
         {arr= convertIntegerToChar(a);
          strcpy(s_a,arr);
         }
      else
        if(a==0)
            strcpy(s_a,"0");

    strcat(s1,s_a);
    strcat(s1," b=");
    if(b<0)
        { b*=-1;
          arr = convertIntegerToChar(b);
          strcpy(s_b,"-");
          strcat(s_b,arr);
        }
     else
        if(b>0)
          { arr = convertIntegerToChar(b);
            strcpy(s_b,arr);
           }
     else
        if(b==0)
             strcpy(s_b,"0");

    strcat(s1,s_b);
    strcpy(s2,"variabila x=-");
    strcat(s2,s_b);
    strcat(s2,"/");
    strcat(s2,s_a);
    strcat(s2," , x=");

    if(variabila<0)
        {variabila*=-1;
         arr = convertIntegerToChar(variabila);
         strcat(s2,"-");
         strcat(s2,arr);}
     else
        if(variabila>0)
         { arr = convertIntegerToChar(variabila);
           strcat(s2,arr);
         }
      else
        if(variabila==0)
             strcat(s2,"0");

    text1(125, 265, s1);
    text1(125, 300, s2);

 }

  else
    if(tip_expresie==2){ // ecuatie gr II

    if(v[0].info[0]=='-') //nr negativ la intrare
    {if(strchr("123456789",v[1].info[0])!=NULL)
      {if(v[2].info[0]=='*')
         {if(v[3].info[0]=='x')
            if(v[4].info[0]=='^')
            {
              if(v[5].info[0]=='2')
                    { a=toInt(v[1].info); a*=-1; }
            }
            else  { b=toInt(v[1].info); b*=-1; }
         }
       else {c=toInt(v[1].info); c*=-1;}
      }
      else
       if(v[1].info[0]=='x')
         if(v[2].info[0]=='^')
             a=-1;
         else b=-1;
    }

  if(strchr("123456789",v[0].info[0])!=NULL) //nr pozitiv la intrare
      {
        if(v[1].info[0]=='*')
         {if(v[2].info[0]=='x')
            if(v[3].info[0]=='^')
            {
              if(v[4].info[0]=='2')
                     a=toInt(v[0].info);
            }
            else b=toInt(v[0].info);
         }
       else c=toInt(v[0].info);
      }
  else {
    if(v[0].info[0]=='x')
       if(v[1].info[0]=='^')
             a=1;
         else b=1;
    }

//la intrare pot avea un a sau b sau c
 if(a!=0 && v[0].nr>6)
   for(i=5;i<v[0].nr;i++)
   {
    if(v[i].info[0]=='x')
        {
         if(v[i-1].info[0]=='*')
           {if(v[i-3].info[0]=='-')
                { b=toInt(v[i-2].info); b*=-1; }
             else if(v[i-3].info[0]=='+')
                  b=toInt(v[i-2].info);
           }
         else if(v[i-1].info[0]=='-')
              b=-1;
         else if(v[i-1].info[0]=='+')
              b=1;
        }
     else
        if(strchr("123456789",v[i].info[0])!=NULL)
          if(i==v[0].nr-1) // ma aflu pe ultima componenta din vector
           {
            if(v[i-1].info[0]=='-')
                  { c=toInt(v[i].info); c*=-1; }
               else if(v[i-1].info[0]=='+')
                    c=toInt(v[i].info);
           }
          else
            if(v[i+1].info[0]!='*')
               if(v[i-1].info[0]=='-')
                  { c=toInt(v[i].info); c*=-1; }
               else if(v[i-1].info[0]=='+')
                    c=toInt(v[i].info);
    }

 else
  if(b!=0 || c!=0)
   for(i=1;i<v[0].nr;i++)
   {
    if(v[i].info[0]=='x' && b!=0)
     {if(v[i+1].info[0]=='^' && v[i+2].info[0]=='2')
        if(v[i-1].info[0]=='*')
          {
            if(v[i-3].info[0]=='-')
                  { a=toInt(v[i-2].info); a*=-1; }
               else if(v[i-3].info[0]=='+')
                    a=toInt(v[i-2].info);
          }
        else
           if(v[i-1].info[0]=='-')
                   a=-1;
            else
             if(v[i-1].info[0]=='+')
                    a=1;
     }
   else
    if(v[i].info[0]=='x' && b==0) //sunt in cazul in care il am pe c
        if(i==v[0].nr-1) // ma aflu pe ultima componenta din vector
           {
            if(v[i-1].info[0]=='*')
                {
                  if(v[i-3].info[0]=='-')
                  { b=toInt(v[i-2].info); b*=-1; }
                  else if(v[i-3].info[0]=='+')
                    b=toInt(v[i-2].info);
                }
            else
              if(v[i-1].info[0]=='-')
                   b=-1;
               else if(v[i-1].info[0]=='+')
                    b=1;
           }
        else {
           if(v[i+1].info[0]=='^')
             {
               if(v[i-1].info[0]=='*')
                {
                  if(v[i-3].info[0]=='-')
                  { a=toInt(v[i-2].info); a*=-1; }
                  else if(v[i-3].info[0]=='+')
                    a=toInt(v[i-2].info);
                }
              else
                if(v[i-1].info[0]=='-')
                     a=-1;
                 else if(v[i-1].info[0]=='+')
                     a=1;
             }
           else
             if(v[i-1].info[0]=='*')
               {
                  if(v[i-3].info[0]=='-')
                  { b=toInt(v[i-2].info); b*=-1; }
                  else if(v[i-3].info[0]=='+')
                    b=toInt(v[i-2].info);
                }
             else
               if(v[i-1].info[0]=='-')
                     b=-1;
                 else if(v[i-1].info[0]=='+')
                     b=1;
           }

  if(strchr("123456789",v[i].info[0])!=NULL && c==0) //sunt in cazul in care il am pe b
      if(i==v[0].nr-1) // ma aflu pe ultima componenta din vector
        {if(v[i-1].info[0]=='-')
               { c=toInt(v[i].info); c*=-1; }
          else
            if(v[i-1].info[0]=='+')
                 c=toInt(v[i].info);
        }
       else{
           if(v[i+1].info[0]!='*')
              if(v[i-1].info[0]=='-')
               { c=toInt(v[i].info); c*=-1; }
             else
              if(v[i-1].info[0]=='+')
                   c=toInt(v[i].info);
          }
 }

    char s2[30];
    char* arr;
    arr=new char[20];
    strcpy(arr, "");

 int delta=b*b-4*a*c;

  if(a!=0&&b!=0 || a!=0&&c!=0)
 {
  if(delta>0)
      {float x1=(-b+sqrt(delta))/(2*a);
       float x2=(-b-sqrt(delta))/(2*a);
       int ax1, ax2;

       //prelucrarea lor pt a afisa sub forma de sir
       char s_x1[20],s_x2[20];
       strcpy(s_x1, "");
       strcpy(s_x2, "");
       if(x1<0)
        {
            strcpy(arr, "");
            x1+=0.000001;
            ax1=x1*1000000*1000000;
            ax1*=-1;
            arr = convertIntegerToChar(ax1);
            strcpy(s_x1,"-");
            strcat(s_x1,arr);
            strcpy(arr, "");
            ax1=x1*1000000;
            ax1=ax1%1000000;
            arr = convertIntegerToChar(ax1);
            strcat(s_x1, ",");
            strcat(s_x1,arr);

         }
     else
        if(x1>0)
         {
              strcpy(arr, "");
              x1+=0.000001;
              ax1=x1*1000000/1000000;
              arr = convertIntegerToChar(ax1);
              strcat(s_x1,arr);
              strcpy(arr, "");
              ax1=x1*1000000;
              ax1=ax1%1000000;
              arr = convertIntegerToChar(ax1);
              strcat(s_x1, ",");
              strcat(s_x1,arr);
         }
      else
        if(x1==0)
            strcpy(s_x1,"0");

   if(x2<0)
        {
            strcpy(arr, "");
            x2+=0.000001;
            ax2=x2*1000000*1000000;
            ax2*=-1;
            arr = convertIntegerToChar(ax2);
            strcpy(s_x2,"-");
            strcat(s_x2,arr);
            strcpy(arr, "");
            ax2=x2*1000000;
            ax2=ax2%1000000;
            arr = convertIntegerToChar(ax2);
            strcat(s_x2, ",");
            strcat(s_x2,arr);
        }
     else
        if(x2>0)
          {
              strcpy(arr, "");
              x2+=0.000001;
              ax2=x2*1000000/1000000;
              arr = convertIntegerToChar(ax2);
              strcat(s_x2,arr);
              strcpy(arr, "");
              ax2=x2*1000000;
              ax2=ax2%1000000;
              arr = convertIntegerToChar(ax2);
              strcat(s_x2, ",");
              strcat(s_x2,arr);
           }
     else
        if(x2==0)
             strcpy(s_x2,"0");

        strcpy(s2,"x1=");
        strcat(s2,s_x1);
        strcat(s2," x2=");
        strcat(s2,s_x2);
      }
  else
   if(delta<0)
           strcpy(s2,"Ecuatia nu are solutii in R : delta<0");
  else
   if(delta==0)
   {
       int x0=-b/(2*a);

       //prelucrarea lui x0 sub forma de sir
       char s_x0[6];
       if(x0<0)
        { x0*=-1;
          arr = convertIntegerToChar(x0);
          strcpy(s_x0,"-");
          strcat(s_x0,arr);
        }
     else
        if(x0>0)
          { arr = convertIntegerToChar(x0);
            strcpy(s_x0,arr);
           }
     else
        if(x0==0)
             strcpy(s_x0,"0");

       strcpy(s2,"x=");
       strcat(s2,s_x0);
   }
 }
    if(b==0 && c==0)
        strcpy(s2,"variabila x=0");

  //prelucrare pt afisarea lor ca siruri
    char s_a[6],s_b[6],s_c[6];
    if(a<0)
        { a*=-1;
          arr = convertIntegerToChar(a);
          strcpy(s_a,"-");
          strcat(s_a,arr);
         }
     else
        if(a>0)
         {arr= convertIntegerToChar(a);
          strcpy(s_a,arr);
         }
      else
        if(a==0)
            strcpy(s_a,"0");

  if(b<0)
        { b*=-1;
          arr = convertIntegerToChar(b);
          strcpy(s_b,"-");
          strcat(s_b,arr);
        }
     else
        if(b>0)
          { arr = convertIntegerToChar(b);
            strcpy(s_b,arr);
           }
     else
        if(b==0)
             strcpy(s_b,"0");

  if(c<0)
        { c*=-1;
          arr = convertIntegerToChar(c);
          strcpy(s_c,"-");
          strcat(s_c,arr);
        }
     else
        if(c>0)
          { arr = convertIntegerToChar(c);
            strcpy(s_c,arr);
           }
     else
        if(c==0)
             strcpy(s_c,"0");

  char s1[30]; // s1: a= b= c=
  strcpy(s1,"a=");
  strcat(s1,s_a);
  strcat(s1," b=");
  strcat(s1,s_b);
  strcat(s1," c=");
  strcat(s1,s_c);

  char s3[30]; // delta =
  char s_delta[6];
   if(delta<0)
        { delta*=-1;
          arr = convertIntegerToChar(delta);
          strcpy(s_delta,"-");
          strcat(s_delta,arr);
        }
     else
        if(delta>0)
          { arr = convertIntegerToChar(delta);
            strcpy(s_delta,arr);
           }
     else
        if(delta==0)
             strcpy(s_delta,"0");

    strcpy(s3,"delta=");
    strcat(s3,s_delta);

    text1(125, 265, s1);
    text1(125, 300, s3);
    text1(125, 340, s2);
}
}

void calculeaza_x()   //Laura Bondor
{
    tip_ecuatie();
   if (tip_expresie>=1)
      {
        afla_x();
      }
   else text1(125, 265, "nu am inclus acest caz");
}


bool clik(int x1, int y1, int x2, int y2)
{
  if(mousex()>x1 && mousex()<x2 && mousey()>y1 && mousey()<y2 && ismouseclick(WM_LBUTTONDOWN))
    {
	    clearmouseclick(WM_LBUTTONDOWN);
    	return 1;
	}
  else
    {
    	clearmouseclick(WM_LBUTTONDOWN);
  	    return 0;
    }
}

void stare (int x1, int y1, int x2, int y2, int c1, int c2)
{
    if(mousex()>x1 && mousex()<x2 && mousey()>y1 && mousey()<y2 )
	{
    	setcolor(c1);
	}
	else
	{
		setcolor(c2);
	}
}
void CitesteSir(int x, int y, char mesaj[100], char S[100] )
{
    char Enter = 13; char BackSpace = 8; char Escape = 27; char s2[2]; s2[1]='\0';
    char MultimeDeCaractereAcceptabile[100]="0123456789abcdefghijklmnopqrstuvwxyz~!`@#$%+-^&*/\\()_=[],;.?<>:;{} ";
    char S_initial[100]; char tasta; char S_[100]; char mesaj_[100];
    strcpy(mesaj_,mesaj);
    strcpy(S_initial,S); settextstyle(DEFAULT_FONT,HORIZ_DIR,2); setcolor(WHITE);
    settextjustify(0,0); outtextxy(x,y,mesaj_);
    x=x+textwidth(mesaj);strcpy(S,"");
    strcpy(S_,S); strcat(S_,"_");
    setcolor(BLACK); outtextxy(x,y,S_);
    s2[0]=tasta; strcat(S,s2); strcpy(S_,S); strcat(S_,"_");
    setcolor(WHITE); outtextxy(x,y,S_);

    do {
        tasta=getch();
        if (tasta==0) { tasta=getch(); Beep(1000,500); }
        else
            if (strchr(MultimeDeCaractereAcceptabile, tasta))
            {
                strcpy(S_,S); strcat(S_,"_");
                setcolor(BLACK); outtextxy(x,y,S_);
                s2[0]=tasta; strcat(S,s2); strcpy(S_,S); strcat(S_,"_");
                setcolor(WHITE); outtextxy(x,y,S_);
            }
            if (tasta==BackSpace)
                    if (!(strcmp(S,""))) Beep(500,100);
                    else
                    {
                        setcolor(BLACK); outtextxy(x,y,S_); setcolor(WHITE);
                        S[strlen(S)-1]='\0'; strcpy(S_,S); strcat(S_,"_") ;
                        outtextxy(x,y,S_); Beep(200,20);
                    }
            else
                if (tasta!=Enter && tasta!=Escape) { Beep(200,20); }
    } while (tasta!=Enter && tasta!=Escape);
    if (tasta == Escape) strcpy(S,S_initial);
    setcolor(BLACK); outtextxy(x,y,S_);setcolor(WHITE); outtextxy(x,y,S); settextjustify(0, 2);
}
int nrNiveluri(nod_a *a)
{
    if(a==NULL)return 0;
    else
    {
        int n1=nrNiveluri(a->st);
        int n2=nrNiveluri(a->dr);
        return 1+max(n1,n2);
    }
}
int nrColoane(nod_a *a)
{
    if(a==NULL)return 0;
    else
    {
        int n1=nrColoane(a->st);
        int n2=nrColoane(a->dr);
        return 1+n1+n2;
    }
}

void deseneazaNod(char s[MAX], int xc, int yc, int latime, int inaltime)
{
    char arr[50];
    sprintf(arr,"%s",s);
    setcolor(GREEN); setfillstyle(SOLID_FILL,LIGHTGREEN );
    fillellipse(xc,yc,textwidth(arr)+10, textheight("M")/2+8);
    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,1);
    setbkcolor(LIGHTGREEN );
    setcolor(RED);
    settextjustify(1,1);
    outtextxy(xc,yc+4,arr);
}

void deseneazaArbore(nod_a *a, int niv, int stanga, int latime, int inaltime)
{
    if(a!=NULL)
    {
        int n1=nrColoane(a->st);
        int xc=stanga+latime*n1+latime/2;
        int yc=niv*inaltime-inaltime/2;

        if (a->st!=NULL)
        {
            int xcs=stanga+latime*nrColoane(a->st->st)+latime/2;
            setcolor(YELLOW);
            line(xc,yc,xcs,yc+inaltime);
        }
        if (a->dr!=NULL)
        {
            int xcd=stanga+latime*(n1+1)+latime*nrColoane(a->dr->st)+latime/2;
            setcolor(YELLOW);
            line(xc,yc,xcd,yc+inaltime);
        }
        deseneazaNod(a->info,xc,yc,latime, inaltime);
        deseneazaArbore(a->st,niv+1,stanga, latime, inaltime);
        deseneazaArbore(a->dr,niv+1,stanga+latime*(n1+1), latime, inaltime);
    }
}

void tipareste_arbore(int derivata)     //curs+modificat
{
    int height=600, width=750;
    int latime, inaltime;
    if(arb==NULL)
        return;
    int window1=initwindow(width,height, "Arbore Functie", 0, 20);
    setcurrentwindow(window1);
    setcolor(WHITE); setbkcolor(BLACK); cleardevice();
    rectangle(1,1,width-1,height-1);
    latime=width/nrColoane(arb);
    inaltime=height/nrNiveluri(arb);
    deseneazaArbore(arb,1,0,latime,inaltime);

    if(arb_sim1==NULL)
        return;
    int window2=initwindow(width,height, "Arbore Derivata", 750, 20);
    setcurrentwindow(window2);
    setcolor(WHITE); setbkcolor(BLACK); cleardevice();
    rectangle(1,1,width-1,height-1);
    latime=width/nrColoane(arb_sim1);
    inaltime=height/nrNiveluri(arb_sim1);
    deseneazaArbore(arb_sim1,1,0,latime,inaltime);

    getch();
    closegraph(window2);
    closegraph(window1);
    setcurrentwindow(derivata);
}

void paginaPrin(arbore arb, arbore arb_der, char functie[1000], int derivata)    //Iustina Apostol
{
    setcurrentwindow(derivata);
    int ok=0, numar;
    char der[1000], der2[1000];
    char mesaj[100], aux1[1000], aux2[1000], raspuns[10];
    strcpy(aux1, "");
    strcpy(aux2, "");
    strcpy(der, "");
    strcpy(der2, "");
    strcpy(mesaj,"introduceti functia: f(x)=");
    stare(100, 125, 900, 450, 4, 15);
    rectangle(100, 125, 900, 450);

    if(ismouseclick(WM_LBUTTONDOWN)&&mousex()>100&&mousey()>125&&mousex()<900&&mousey()<450)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        setviewport(0, 51, getmaxx(), getmaxy(), 1);
        setbkcolor(RGB(9, 19, 33));
        clearviewport();
        setviewport(0, 0, getmaxx(), getmaxy(), 1);
        setviewport(100, 125, 900, 450, 1);
        setbkcolor(0);
        clearviewport();
        setviewport(0, 0, getmaxx(), getmaxy(), 1);
        setbkcolor(0);
        rectangle(100, 125, 900, 450);
        for(int i=0; i<vec[0].nr; i++)
        {
            vec[i].info[0]='\0';
        }
        for(int i=0; i<polonez[0].nr; i++)
        {
            polonez[i].info[0]='\0';
        }
        arb=NULL;
        arb_der=NULL;
        arb_sim1=NULL;
        functie[0]='\0';
        CitesteSir(125, 160, mesaj, functie);
        ok=1;
    }
    if(functie[0]=='\0')
        return;
    if(ok==1)
    {

        deriv(functie, der);
        strcpy(aux2, der);
        fout<<"";
        pune_in_fisier(arb_sim1);
        pune_in_vector(v);
        tip_expresie=0;
        tip_ecuatie();
        if(calc_ok)
        {
            numar=strlen(der);
            if(numar>75)
            {
                strcpy(aux1,der+75);
                der[75]='%';
                strcpy(der+76,aux1);

                strcpy(aux1,der+25);
                der[25]='%';
                strcpy(der+26,aux1);

                strcpy(aux1,der);
                strcpy(der, "                             ");
               strcat(der, aux1);
            }
            else{
                if(numar>25)
                {
                    strncpy(aux1, der, 25);
                    strcat(aux1, "%");
                    strcat(aux1, der+25);
                    strcpy(der, "                             ");
                    strcat(der, aux1);
                }
            }
            if(numar<=25)
            {
                strcpy(der, "                             ");
                strcat(der, aux2);
            }
            text1(125, 165, der);
            text(125, 165, "derivata de gradul 1: f'(x)=");
            if(tip_expresie==0)
            {
                text(125, 220, "calculati derivata de gradul 2?(Y/N):");
                CitesteSir(655, 235, " ", raspuns);
                if(raspuns[0]=='y'||raspuns[0]=='Y')
                {
                    arb=NULL;
                    deriv(nesimplif, der2);
                    strcpy(der, der2);
                    if(strlen(der2)>90)
                    {
                        strcpy(aux1,der2+90);
                        der2[90]='%';
                        strcpy(der2+91,aux1);

                        strcpy(aux1,der2+40);
                        der2[40]='%';
                        strcpy(der2+41,aux1);

                        strcpy(aux1,der2);
                        strcpy(der2, "       ");
                        strcat(der2, aux1);
                    }
                    else{
                        if(strlen(der2)>40)
                        {
                            strncpy(aux1, der2, 40);
                            strcat(aux1, "%");
                            strcat(aux1, der2+40);
                            strcpy(der2, "       ");
                            strcat(der2, aux1);
                        }
                        else
                        {
                            strcpy(der2, "       ");
                            strcat(der2, der);
                        }
                    }
                    text1(125, 240, der2);
                    text(125, 240, "f''(x)=");
                }
            }
            if(tip_expresie==1||tip_expresie==2)
            {
               text(125, 220, "calculati radacinile (1)/calculati f''(x)(2): ");
                CitesteSir(755, 235, " ", raspuns);

                if(raspuns[0]=='2'||raspuns[0]=='2')
                {
                    deriv(nesimplif, der2);
                    strcpy(der, der2);
                    if(strlen(der2)>75)
                    {
                        strcpy(aux1,der2+75);
                        der2[75]='%';
                        strcpy(der2+76,aux1);

                        strcpy(aux1,der+25);
                        der2[25]='%';
                        strcpy(der2+26,aux1);

                        strcpy(aux1,der2);
                        strcpy(der2, "                             ");
                        strcat(der2, aux1);
                    }
                    else{
                        if(strlen(der2)>25)
                        {
                            strncpy(aux1, der2, 25);
                            strcat(aux1, "%");
                            strcat(aux1, der2+25);
                            strcpy(der2, "                             ");
                            strcat(der2, aux1);
                        }
                        else
                        {
                            strcpy(der2, "                             ");
                            strcat(der2, der);
                        }
                    }
                    text1(125, 240, der2);
                    text(125, 240, "derivata de gradul 2:f''(x)=");
                }
                if(raspuns[0]=='1')
                {
                    calculeaza_x();
                }
            }

        }
        else
            text1(125, 165, der);

    }

    setviewport(0, 0, getmaxx(), getmaxy(), 1);
}
void vizualizare(int derivata, int &ok)    //Iustina Apostol
{
    char scris[1000];
    if(ok)
    {
        tipareste_arbore(derivata);
        ok=0;
    }
    setcurrentwindow(derivata);
    settextstyle(8, 0, 1);
    setbkcolor(RGB(15, 38, 35));
    settextjustify(1, 1);
    settextstyle(8, 0, 2);
    circle(802, 78, 25);
    outtextxy(802, 78, "+");
    circle(712, 162, 25);
    outtextxy(712, 162, "*");
    circle(892, 162, 25);
    outtextxy(892, 162, "sin");
    circle(622, 252, 25);
    outtextxy(622, 252, "5");
    circle(802, 252, 25);
    outtextxy(802, 252, "x");
    circle(972, 252, 25);
    outtextxy(972, 252, "x");
    line(780,95, 735, 140);
    line(825, 95, 870, 140);
    line(690, 185, 645, 230);
    line(735, 185, 780, 230);
    line(905, 185, 950, 230);

    circle(670, 400, 25);
    outtextxy(670, 400,"5");
    circle(750, 320, 25);
    outtextxy(750, 320,"+");
    circle(830, 400, 25);
    outtextxy(830, 400,"cos");
    circle(910, 480, 25);
    outtextxy(910, 480,"x");
    line(730, 340, 690, 380);
    line(770, 340, 810, 380);
    line(850, 420, 890, 460);
    settextjustify(0, 2);
    setbkcolor(RGB(15, 38, 35));
    strcpy(scris, "Vizualizarea functiei si derivatei ei in%arbori ajuta la intelegerea procesului de %calcul al derivatei folosita de calculator. %Algoritmul parcurge in preordine arborele %functie derivand fiecare component conform %regulilor de derivare.%%In exemplul alaturat - + ramane +, %- ramura * se transforma in 5, %        /%       5   x%- iar ramura sin se transforma in cos.");
    line(95, 210, 200, 340);
    text1(60, 150, scris);
    setbkcolor(0);
}
void informatii(int derivata)    //Iustina Apostol
{
    char info[1000];
    settextstyle(9, 0, 1);
    setbkcolor(RGB(14, 36, 18));
    strcpy(info, "Calculatorul de derivate face calculul derivatei unei functii f(x) introduse de utilizator in %raport cu variabila x. % %Calculatorul nostru include si verificarea functiei (aceasta include: verificarea validitatii %introduse, asiguarea ca parantezele introduse sunt corecte precum si semnalizarea %impartirii la 0), si simplificarea functiei derivata si evaluarea ecuatiei f’(x)=0 in cazul %unei ecuatii de gradul 1 sau 2 pentru determinarea punctelor de extrem pentru f(x).%*Sunt sustinute doare ecuatiile de tipul a*x^2+b*x+c=0 sau a*x+b=0 % %Calculatorul nu include numere cu virgula, acestea pot fi inlocuite cu fractii echivalente. %Aceeasi regula este valabila pentru logaritmii de tip log_(baza)(argument) si trebuie %inlocuit de utilizator cu (ln(argument)/ln(baza)).");

    settextjustify(0, 2);
    text1(60, 100, info);
}
void meniu()    //Iustina Apostol
{

    arbore arb_func, arb_der;
    int t, i, control=0, derivata, ok=1;
	derivata=initwindow( 1000,550, "Calculator Derivata", 300,100 );
	setbkcolor(RGB(0, 0, 0));
	cleardevice();
    setfillstyle(SOLID_FILL,RED);
    char functie[100]="";
    char mesaj[100];
    strcpy(mesaj," ");
    setviewport(0, 51, getmaxx(), getmaxy(), 1);
    setbkcolor(RGB(9, 19, 33));
    clearviewport();
    setviewport(1, 1, 249, 50, 1);
    setbkcolor(RGB(9, 19, 33));     //  bk col pag prin
    clearviewport();
    setbkcolor(RGB(0, 0, 0));
    setviewport(100, 125, 900, 450, 1);
    setbkcolor(0);
    clearviewport();
    setviewport(0, 0, getmaxx(), getmaxy(), 1);
	while(true)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            for(i=0; i<4; i++)
            {
                if(mousex()>i*250&&mousey()>0&&mousex()<249+i*250&&mousey()<50)
                {
                    control=i;
                switch(i)
                {
                    case 0:
                        setviewport(0, 51, getmaxx(), getmaxy(), 1);
                        setbkcolor(RGB(9, 19, 33));
                        clearviewport();
                        setviewport(1, 1, 249, 50, 1);
                        setbkcolor(RGB(9, 19, 33));     //  bk col pag prin
                        clearviewport();
                        setbkcolor(RGB(0, 0, 0));
                        setviewport(100, 125, 900, 450, 1);
                        setbkcolor(0);
                        clearviewport();

                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(250, 0, 499, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(250, 0, 499, 50  , 2, 15);
                        rectangle(250, 0, 499, 50);
                        text(275,20,"vizualizare");

                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(500, 0, 749, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(500, 0, 749, 50, 2, 15);
                        rectangle(500, 0, 749, 50);
                        text(525,20,"informatii");

                        stare(750, 0, 1000, 50, 4, 15);
                        rectangle(750, 0, 1000, 50);
                        text(775,20,"close");
                        setcolor(15);
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        break;
                    case 1:
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(0,0, 249, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(0,0, 249, 50, 2, 15);
                        rectangle(0,0, 249, 50);
                        text(10,20,"pagina principala");

                        setviewport(0, 51, getmaxx(), getmaxy(), 1);
                        setbkcolor(RGB(15, 38, 35));
                        clearviewport();
                        setviewport(250, 0, 499, 50, 1);
                        setbkcolor(RGB(15, 38, 35));        //bk col viz
                        clearviewport();
                        setbkcolor(RGB(0, 0, 0));

                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(500, 0, 749, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(500, 0, 749, 50, 2, 15);
                        rectangle(500, 0, 749, 50);
                        text(525,20,"informatii");

                        stare(750, 0, 1000, 50, 4, 15);
                        rectangle(750, 0, 1000, 50);
                        text(775,20,"close");
                        setcolor(15);
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        ok=1;
                        vizualizare(derivata, ok);
                        break;

                    case 2:
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(0,0, 249, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(0,0, 249, 50, 2, 15);
                        rectangle(0,0, 249, 50);
                        text(10,20,"pagina principala");

                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        settextstyle(0, 0, 2);
                        setviewport(250, 0, 499, 50, 1);
                        setbkcolor(0);
                        clearviewport();
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        stare(250, 0, 499, 50, 2, 15);
                        rectangle(250, 0, 499, 50);
                        text(275,20,"vizualizare");

                        setviewport(0, 51, getmaxx(), getmaxy(), 1);
                        setbkcolor(RGB(14, 36, 18));
                        clearviewport();
                        setviewport(500, 0, 749, 50, 1);
                        setbkcolor(RGB(14, 36, 18));        //bk col info
                        clearviewport();
                        setbkcolor(RGB(0, 0, 0));

                        stare(750, 0, 1000, 50, 4, 15);
                        rectangle(750, 0, 1000, 50);
                        text(775,20,"close");
                        setcolor(15);
                        setviewport(0, 0, getmaxx(), getmaxy(), 1);
                        informatii(derivata);
                        break;
                }

            }
        }
    }
        switch(control)
        {
            case 0:

                settextstyle(0, 0, 2);
                setbkcolor(RGB(9, 19, 33));
                stare(0,0, 249, 50, 2, 15);
                text(10,20,"pagina principala");
                setbkcolor(RGB(0, 0, 0));
                rectangle(0,0, 249, 50);

                stare(250, 0, 499, 50  , 2, 15);
                rectangle(250, 0, 499, 50);
                text(275,20,"vizualizare");

                stare(500, 0, 749, 50, 2, 15);
                rectangle(500, 0, 749, 50);
                text(525,20,"informatii");

                stare(750, 0, 1000, 50, 4, 15);
                rectangle(750, 0, 1000, 50);
                text(775,20,"close");
                setcolor(15);

                paginaPrin(arb, arb_der, functie, derivata);

                break;
            case 1:

                settextstyle(0, 0, 2);
                stare(0,0, 249, 50, 2, 15);
                rectangle(0,0, 249, 50);
                text(10,20,"pagina principala");

                setbkcolor(RGB(15, 38, 35));
                stare(250, 0, 499, 50  , 2, 15);
                text(275,20,"vizualizare");
                setbkcolor(RGB(0, 0, 0));
                rectangle(250, 0, 499, 50);

                stare(500, 0, 749, 50, 2, 15);
                rectangle(500, 0, 749, 50);
                text(525,20,"informatii");

                stare(750, 0, 1000, 50, 4, 15);
                rectangle(750, 0, 1000, 50);
                text(775,20,"close");
                setcolor(15);


                break;
            case 2:

                settextstyle(0, 0, 2);
                stare(0,0, 249, 50, 2, 15);
                rectangle(0,0, 249, 50);
                text(10,20,"pagina principala");

                stare(250, 0, 499, 50  , 2, 15);
                rectangle(250, 0, 499, 50);
                text(275,20,"vizualizare");

                setbkcolor(RGB(14, 36, 18));
                stare(500, 0, 749, 50, 2, 15);
                text(525,20,"informatii");
                setbkcolor(RGB(0, 0, 0));
                rectangle(500, 0, 749, 50);

                stare(750, 0, 1000, 50, 4, 15);
                rectangle(750, 0, 1000, 50);
                text(775,20,"close");
                setcolor(15);

                break;
            case 3:
                closegraph();
                return;
                break;
            default:
                break;
        }
	}

	getch();
	closegraph(derivata);
}
int main() {
    meniu();
    return 0;
}
