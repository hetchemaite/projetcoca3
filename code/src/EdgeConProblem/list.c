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

Liste *initialisation()
{
    Liste *liste = malloc(sizeof(*liste));
    Element *element = malloc(sizeof(*element));


    element->nombre = 0;
    element->suivant = NULL;
    liste->premier = element;

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
    Element *tmp=liste;
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
    return NULL;

}