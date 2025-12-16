#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cours.h"

/* Fonction pour ajouter un cours dans le fichier - comme dans les ateliers */
int ajouter_cours(Cours c)
{
    FILE *f = fopen("cours.txt", "a");
    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier cours.txt\n");
        return 0;
    }
    
    /* Écrire le cours dans le fichier */
    fprintf(f, "%s;%s;%s;%s;%d;%d;%d;%d\n", 
            c.id, c.nom, c.specialite, c.centre,
            c.date_expiration.jour, c.date_expiration.mois, c.date_expiration.annee,
            c.niveau);
    
    fclose(f);
    return 1;
}

/* Fonction pour initialiser le fichier des cours */
void initialiser_fichier_cours()
{
    FILE *f = fopen("cours.txt", "a");
    if (f != NULL) {
        fclose(f);
    }
}

/* Fonction pour modifier un cours */
int modifier_cours(Cours c)
{
    /* À implémenter selon vos besoins */
    return 1;
}

/* Fonction pour supprimer un cours */
int supprimer_cours(char id[])
{
    /* À implémenter selon vos besoins */
    return 1;
}

/* Fonction pour chercher un cours */
Cours chercher_cours(char id[])
{
    Cours c;
    /* À implémenter selon vos besoins */
    return c;
}
