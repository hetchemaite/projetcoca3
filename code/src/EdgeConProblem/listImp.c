
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "listImp.h"


ListeImp *initialisationImp()
{
    ListeImp *listeimp = malloc(sizeof(*listeimp));
    ListeImpElem *liste = malloc(sizeof(*liste));

    //init liste
    liste->elem = initialisation();
    liste->suivant =NULL;

    //init listeimp
    listeimp->size = 0;
    listeimp->premier =  liste;


    return listeimp;
}

void insertionList(ListeImp *liste, Liste nvElem)
{
    ListeImpElem *nvListe = malloc(sizeof(*nvListe));
    nvListe->elem = &nvElem;
    nvListe->suivant = NULL;


    ListeImpElem *tmp = liste->premier;
    for(int i=0; i<liste->size; i++){
        tmp = tmp->suivant;
    }
    /* Insertion de l'élément au début de la liste */
    tmp->suivant = nvListe;

    liste->size++;
}

void insertionAtImp(ListeImp *liste,int index ,Liste nvElem)
{

    ListeImpElem *nvListe = malloc(sizeof(*nvListe));
    nvListe->elem = &nvElem;
    nvListe->suivant = NULL;

    ListeImpElem *courant = liste->premier;
    for(int i=0; i<index-1; i++){
        courant = courant->suivant;
    }

    ListeImpElem *tmp = courant->suivant;
    courant->suivant = nvListe;
    nvListe->suivant = tmp;
}


Liste * getListe(ListeImp *liste,int indix){
    ListeImpElem *tmp=liste->premier;
    int i = 0;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL && i < indix)
    {
        tmp = tmp->suivant;
        i++;

    }
    if(i == indix){
        return tmp->elem;
    }
    return NULL;
}
