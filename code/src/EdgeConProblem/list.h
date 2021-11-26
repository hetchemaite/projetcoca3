#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>


typedef struct Element Element;
struct Element
{
    int nombre;
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
    int size;
};

Liste *initialisation();

void insertion(Liste *liste, int nvNombre);

void insertionAt(Liste *liste,int index , int nvNombre);

void suppression(Liste *liste);

void afficherListe(Liste *liste);

int getElem(Liste *liste,int indix);

#endif