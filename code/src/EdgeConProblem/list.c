#include <stdlib.h>
#include <stdio.h>
#include "list.h"


Liste *initialisation()
{
    Liste *liste = malloc(sizeof(*liste));
    Element *element = malloc(sizeof(*element));


    element->nombre = 0;
    element->suivant = NULL;
    liste->premier = element;

    liste->size = 0;
    return liste;
}

void insertion(Liste *liste, int nvNombre)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    nouveau->nombre = nvNombre;

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;

    liste->size++;
}

void insertionAt(Liste *liste,int index , int nvNombre)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    nouveau->nombre = nvNombre;

    Element *courant = liste->premier;
    for(int i=0; i<index-1; i++){
        courant = courant->suivant;
    }

    Element *tmp = courant->suivant;
    courant->suivant = nouveau;
    nouveau->suivant = tmp;
}

void suppression(Liste *liste)
{
    if (liste->premier != NULL)
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);

        liste->size--;

    }
}

void afficherListe(Liste *liste)
{
    Element *actuel = liste->premier;

    while (actuel != NULL)
    {
        printf("%d -> ", actuel->nombre);
        actuel = actuel->suivant;
    }
    printf("NULL\n");
}

int getElem(Liste *liste,int indix){
    Element *tmp=liste->premier;
    int i = 0;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL && i < indix)
    {
        tmp = tmp->suivant;
        i++;

    }
    if(i == indix){
        return tmp->nombre;
    }
    return -1;

}