#ifndef COUR_H
#define COUR_H

#include <gtk/gtk.h>

// Structure Cours
typedef struct {
    int id;                    // ID numérique
    char nom[50];             // Nom du cours
    char centre[50];          // Centre sportif
    char specialite[50];      // Spécialité (Fitness, SportCollectif, Musculation)
    char niveau[20];          // Niveau (Débutant/Professionnel)
    char date_expiration[20]; // Date d'expiration
} Cours;

// Fonctions de base
int ajouter_cours(Cours *c);
int charger_cours(Cours liste[], int max);
void afficher_tous_cours(GtkWidget *treeview);
void afficher_cours_dans_treeview(GtkWidget *treeview);
void remplir_combobox_specialite(GtkWidget *combobox);

// Fonctions de recherche
int existe_cours_id(const char *id);
Cours* rechercher_cours_par_id(const char *id);
int rechercher_cours(const char *texte, Cours *resultats[], int max);
void afficher_recherche_cours(GtkWidget *treeview, Cours *resultats[], int count);
void vider_treeview(GtkWidget *treeview);

// Fonctions de modification
int modifier_cours(const char *id, Cours *nouveau);
int supprimer_cours(const char *id);

// Fonctions utilitaires
void formater_date(int jour, int mois, int annee, char *date_str);

#endif /* COUR_H */
