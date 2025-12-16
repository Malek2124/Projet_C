#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <gtk/gtk.h>
#include "cour.h"

// Fonction pour générer un ID unique
static int generer_id_unique() {
    static int dernier_id = 0;
    if (dernier_id == 0) {
        // Lire le dernier ID du fichier
        FILE *f = fopen("cours.txt", "r");
        if (f) {
            int max_id = 0;
            char ligne[256];
            Cours c;
            while (fgets(ligne, sizeof(ligne), f)) {
                // Utiliser sscanf avec une gestion plus flexible
                if (sscanf(ligne, "%d %49s %49s %49s %19s %19s", 
                   &c.id, c.nom, c.centre, c.specialite, c.niveau, c.date_expiration) >= 1) {
                    if (c.id > max_id) max_id = c.id;
                }
            }
            fclose(f);
            dernier_id = max_id;
        } else {
            dernier_id = 1000; // Premier ID
        }
    }
    return ++dernier_id;
}

int ajouter_cours(Cours *c) {
    // Générer un ID si non spécifié
    if (c->id == 0) {
        c->id = generer_id_unique();
    }
    
    FILE *f = fopen("cours.txt", "a");
    if (f) {
        fprintf(f, "%d %s %s %s %s %s\n", 
                c->id, c->nom, c->centre, c->specialite, c->niveau, c->date_expiration);
        fclose(f);
        return 1;
    }
    return 0;
}

int charger_cours(Cours liste[], int max) {
    FILE *f = fopen("cours.txt", "r");
    int i = 0;
    if (f) {
        char ligne[256];
        while (i < max && fgets(ligne, sizeof(ligne), f)) {
            // Nettoyer la ligne (supprimer le saut de ligne)
            ligne[strcspn(ligne, "\n")] = '\0';
            
            // Vérifier si la ligne n'est pas vide
            if (strlen(ligne) > 0) {
                // Utiliser une lecture plus robuste
                int result = sscanf(ligne, "%d %49s %49s %49s %19s %19s", 
                       &liste[i].id, 
                       liste[i].nom, 
                       liste[i].centre, 
                       liste[i].specialite, 
                       liste[i].niveau, 
                       liste[i].date_expiration);
                
                if (result >= 6) { // Tous les champs sont lus
                    i++;
                } else if (result == 5) { // Date manquante
                    strcpy(liste[i].date_expiration, "01/01/2026"); // Date par défaut
                    i++;
                } else if (result > 0) { // Au moins l'ID est lu
                    // Remplir les champs manquants avec des valeurs par défaut
                    if (result < 6) strcpy(liste[i].date_expiration, "01/01/2026");
                    if (result < 5) strcpy(liste[i].niveau, "Débutant");
                    if (result < 4) strcpy(liste[i].specialite, "Fitness");
                    if (result < 3) strcpy(liste[i].centre, "CentreInconnu");
                    if (result < 2) strcpy(liste[i].nom, "CoursInconnu");
                    i++;
                }
            }
        }
        fclose(f);
    }
    return i;
}

// Fonction principale pour afficher les cours dans un TreeView
void afficher_cours_dans_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        g_print("Erreur: TreeView invalide\n");
        return;
    }
    
    // Créer un nouveau modèle avec 5 colonnes
    GtkListStore *store = gtk_list_store_new(5, 
        G_TYPE_INT,        // Colonne 0: ID
        G_TYPE_STRING,     // Colonne 1: Nom
        G_TYPE_STRING,     // Colonne 2: Centre
        G_TYPE_STRING,     // Colonne 3: Spécialité
        G_TYPE_STRING);    // Colonne 4: Niveau
    
    // Charger les données
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    
    g_print("DEBUG: Chargement de %d cours depuis cours.txt\n", nb);
    
    // Afficher le contenu pour débogage
    for (int i = 0; i < nb; i++) {
        g_print("  Cours %d: ID=%d, Nom=%s, Centre=%s, Specialite=%s, Niveau=%s, Date=%s\n", 
                i, liste[i].id, liste[i].nom, liste[i].centre, 
                liste[i].specialite, liste[i].niveau, liste[i].date_expiration);
    }
    
    // Remplir le modèle
    for (int i = 0; i < nb; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, liste[i].id,
            1, liste[i].nom,
            2, liste[i].centre,
            3, liste[i].specialite,
            4, liste[i].niveau,
            -1);
    }
    
    // Configurer le TreeView s'il n'a pas encore de colonnes
    GtkTreeModel *model_existant = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model_existant == NULL) {
        g_print("DEBUG: Création des colonnes du TreeView\n");
        
        // Créer les colonnes
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        // Colonne ID
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_min_width(column, 50);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Nom
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_min_width(column, 100);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Centre
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Centre", renderer, "text", 2, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_min_width(column, 100);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Spécialité
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_min_width(column, 100);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        // Colonne Niveau
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Niveau", renderer, "text", 4, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_column_set_min_width(column, 80);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    } else {
        g_print("DEBUG: TreeView a déjà des colonnes\n");
    }
    
    // Appliquer le modèle
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    g_print("DEBUG: TreeView mis à jour avec %d cours\n", nb);
}

// Alias pour compatibilité
void afficher_tous_cours(GtkWidget *treeview) {
    afficher_cours_dans_treeview(treeview);
}

// Fonction modifiée pour GTK 2.x - SIMPLIFIÉE
void remplir_combobox_specialite(GtkWidget *combobox) {
    if (!combobox) return;
    
    // Vérifier si la combo box a déjà des éléments
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox));
    if (model == NULL) {
        // Si pas de modèle, créer et remplir
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox), "Fitness");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox), "SportCollectif");
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox), "Musculation");
    }
    
    // Toujours sélectionner le premier élément
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0);
}

int existe_cours_id(const char *id_str) {
    int id = atoi(id_str);
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    
    for (int i = 0; i < nb; i++) {
        if (liste[i].id == id) {
            return 1;
        }
    }
    return 0;
}

Cours* rechercher_cours_par_id(const char *id_str) {
    int id = atoi(id_str);
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    
    for (int i = 0; i < nb; i++) {
        if (liste[i].id == id) {
            Cours *c = malloc(sizeof(Cours));
            if (c) {
                *c = liste[i];
            }
            return c;
        }
    }
    return NULL;
}

int rechercher_cours(const char *texte, Cours *resultats[], int max) {
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    int count = 0;
    
    if (texte == NULL || strlen(texte) == 0) {
        // Si texte vide, retourner tous les cours
        for (int i = 0; i < nb && count < max; i++) {
            resultats[count] = malloc(sizeof(Cours));
            if (resultats[count]) {
                *resultats[count] = liste[i];
                count++;
            }
        }
        return count;
    }
    
    char texte_lower[100];
    strncpy(texte_lower, texte, sizeof(texte_lower)-1);
    texte_lower[sizeof(texte_lower)-1] = '\0';
    
    // Convertir en minuscules
    for (int i = 0; texte_lower[i]; i++) {
        texte_lower[i] = tolower(texte_lower[i]);
    }
    
    for (int i = 0; i < nb && count < max; i++) {
        char nom_lower[100], centre_lower[100], specialite_lower[100], niveau_lower[100];
        
        // Copier et convertir chaque champ en minuscules
        strncpy(nom_lower, liste[i].nom, sizeof(nom_lower)-1);
        nom_lower[sizeof(nom_lower)-1] = '\0';
        for (int j = 0; nom_lower[j]; j++) nom_lower[j] = tolower(nom_lower[j]);
        
        strncpy(centre_lower, liste[i].centre, sizeof(centre_lower)-1);
        centre_lower[sizeof(centre_lower)-1] = '\0';
        for (int j = 0; centre_lower[j]; j++) centre_lower[j] = tolower(centre_lower[j]);
        
        strncpy(specialite_lower, liste[i].specialite, sizeof(specialite_lower)-1);
        specialite_lower[sizeof(specialite_lower)-1] = '\0';
        for (int j = 0; specialite_lower[j]; j++) specialite_lower[j] = tolower(specialite_lower[j]);
        
        strncpy(niveau_lower, liste[i].niveau, sizeof(niveau_lower)-1);
        niveau_lower[sizeof(niveau_lower)-1] = '\0';
        for (int j = 0; niveau_lower[j]; j++) niveau_lower[j] = tolower(niveau_lower[j]);
        
        // Rechercher dans tous les champs
        if (strstr(nom_lower, texte_lower) != NULL ||
            strstr(centre_lower, texte_lower) != NULL ||
            strstr(specialite_lower, texte_lower) != NULL ||
            strstr(niveau_lower, texte_lower) != NULL) {
            
            resultats[count] = malloc(sizeof(Cours));
            if (resultats[count]) {
                *resultats[count] = liste[i];
                count++;
            }
        }
    }
    
    return count;
}

void afficher_recherche_cours(GtkWidget *treeview, Cours *resultats[], int count) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        g_print("Erreur: TreeView invalide pour recherche\n");
        return;
    }
    
    // Créer un nouveau modèle avec 5 colonnes
    GtkListStore *store = gtk_list_store_new(5, 
        G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    
    // Remplir le modèle avec les résultats
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, resultats[i]->id,
            1, resultats[i]->nom,
            2, resultats[i]->centre,
            3, resultats[i]->specialite,
            4, resultats[i]->niveau,
            -1);
    }
    
    // Configurer le TreeView s'il n'a pas encore de colonnes
    GtkTreeModel *model_existant = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model_existant == NULL) {
        // Créer les colonnes
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Centre", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Niveau", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    
    // Appliquer le modèle
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    g_print("Affichage de %d résultats de recherche\n", count);
}

void vider_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) return;
    
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model) {
        GtkListStore *store = GTK_LIST_STORE(model);
        gtk_list_store_clear(store);
    }
}

int modifier_cours(const char *id_str, Cours *nouveau) {
    int id = atoi(id_str);
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    int trouve = 0;
    
    // Mettre à jour le cours
    for (int i = 0; i < nb; i++) {
        if (liste[i].id == id) {
            liste[i] = *nouveau;
            liste[i].id = id; // Garder le même ID
            trouve = 1;
            break;
        }
    }
    
    if (!trouve) return 0;
    
    // Réécrire le fichier
    FILE *f = fopen("cours.txt", "w");
    if (!f) return 0;
    
    for (int i = 0; i < nb; i++) {
        fprintf(f, "%d %s %s %s %s %s\n", 
                liste[i].id, 
                liste[i].nom, 
                liste[i].centre, 
                liste[i].specialite, 
                liste[i].niveau, 
                liste[i].date_expiration);
    }
    fclose(f);
    return 1;
}

int supprimer_cours(const char *id_str) {
    int id = atoi(id_str);
    Cours liste[100];
    int nb = charger_cours(liste, 100);
    int new_nb = 0;
    Cours nouvelle_liste[100];
    
    // Copier tous les cours sauf celui à supprimer
    for (int i = 0; i < nb; i++) {
        if (liste[i].id != id) {
            nouvelle_liste[new_nb++] = liste[i];
        }
    }
    
    if (new_nb == nb) return 0; // Rien à supprimer
    
    // Réécrire le fichier
    FILE *f = fopen("cours.txt", "w");
    if (!f) return 0;
    
    for (int i = 0; i < new_nb; i++) {
        fprintf(f, "%d %s %s %s %s %s\n", 
                nouvelle_liste[i].id, 
                nouvelle_liste[i].nom, 
                nouvelle_liste[i].centre, 
                nouvelle_liste[i].specialite, 
                nouvelle_liste[i].niveau, 
                nouvelle_liste[i].date_expiration);
    }
    fclose(f);
    return 1;
}

void formater_date(int jour, int mois, int annee, char *date_str) {
    sprintf(date_str, "%02d/%02d/%04d", jour, mois, annee);
}
