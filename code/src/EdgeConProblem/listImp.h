#ifndef LISTIMP_H
#define LISTIMP_H

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct ListeImpElem ListeImpElem;
struct ListeImpElem
{
    Liste *elem;
    ListeImpElem *suivant;
};


typedef struct ListeImp ListeImp;
struct ListeImp
{
    ListeImpElem *premier;
    int size;
};



ListeImp *initialisationImp();

void insertionList(ListeImp *liste, Liste nvElem);

void insertionAtImp(ListeImp *liste,int index ,Liste nvElem);


Liste * getListe(ListeImp *liste,int indix);
#endif
