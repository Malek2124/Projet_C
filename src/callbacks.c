#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "cour.h"

// Variables globales
int niveau_selectionne = 0;

// Fonction pour afficher un message d'erreur
static void afficher_erreur(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent != NULL ? GTK_WINDOW(parent) : NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message);
    
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Fonction pour afficher un message d'information
static void afficher_info(GtkWidget *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent != NULL ? GTK_WINDOW(parent) : NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message);
    
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// ==============================================
// FONCTION POUR INITIALISER LES TREEVIEW
// ==============================================
void initialiser_tous_les_treeviews(GtkWidget *window) {
    g_print("=== INITIALISATION DES TREEVIEW ===\n");
    
    // TreeView de recherche admin
    GtkWidget *treeview_recherche = lookup_widget(window, "raedtreeviewrecherche");
    if (treeview_recherche) {
        g_print("1. Initialisation du TreeView recherche admin...\n");
        afficher_cours_dans_treeview(treeview_recherche);
    } else {
        g_print("ERREUR: TreeView recherche admin non trouvé\n");
    }
    
    // TreeView membre
    GtkWidget *treeview_membre = lookup_widget(window, "raedtreeview12");
    if (treeview_membre) {
        g_print("2. Initialisation du TreeView membre...\n");
        afficher_cours_dans_treeview(treeview_membre);
    } else {
        g_print("WARNING: TreeView membre non trouvé dans cette fenêtre\n");
    }
    
    // Vérifier si le fichier cours.txt existe
    FILE *f = fopen("cours.txt", "r");
    if (f) {
        g_print("INFO: Fichier cours.txt trouvé\n");
        fclose(f);
    } else {
        g_print("WARNING: Création du fichier cours.txt...\n");
        f = fopen("cours.txt", "w");
        if (f) {
            // Ajouter quelques cours d'exemple
            fprintf(f, "1001 Yoga FitnessCentre Fitness Débutant 01/01/2026\n");
            fprintf(f, "1002 Football SportsCentre SportCollectif Professionnel 01/01/2026\n");
            fprintf(f, "1003 Musculation GymCentre Musculation Débutant 01/01/2026\n");
            fprintf(f, "1004 Natation AquaCentre SportCollectif Professionnel 01/01/2026\n");
            fprintf(f, "1005 Cardio FitCenter Fitness Débutant 01/01/2026\n");
            fclose(f);
            g_print("INFO: Fichier cours.txt créé avec 5 exemples\n");
            
            // Recharger les TreeView
            if (treeview_recherche) afficher_cours_dans_treeview(treeview_recherche);
            if (treeview_membre) afficher_cours_dans_treeview(treeview_membre);
        } else {
            g_print("ERREUR: Impossible de créer cours.txt\n");
        }
    }
}

// ==============================================
// FENÊTRE RECHERCHER (Admin)
// ==============================================
void on_raedbuttonrecherche_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "MalekMhatliWAdmin");
    if (!window) {
        afficher_erreur(NULL, "Fenêtre principale non trouvée!");
        return;
    }
    
    GtkWidget *entry_recherche = lookup_widget(window, "raedentryrecherche");
    GtkWidget *treeview = lookup_widget(window, "raedtreeviewrecherche");
    
    if (!entry_recherche || !treeview) {
        afficher_erreur(window, "Widgets non trouvés!");
        return;
    }
    
    const char *texte = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    
    if (texte == NULL || strlen(texte) == 0) {
        // Afficher tous les cours si champ vide
        afficher_cours_dans_treeview(treeview);
        return;
    }
    
    g_print("Recherche avec: %s\n", texte);
    
    // Rechercher
    Cours *resultats[100];
    int count = rechercher_cours(texte, resultats, 100);
    
    if (count > 0) {
        g_print("%d cours trouvés\n", count);
        afficher_recherche_cours(treeview, resultats, count);
        
        // Libérer la mémoire
        for (int i = 0; i < count; i++) {
            if (resultats[i]) {
                free(resultats[i]);
            }
        }
    } else {
        vider_treeview(treeview);
        afficher_info(window, "Aucun cours trouvé.");
    }
}

// ==============================================
// SPIN BUTTONS (Admin)
// ==============================================
void on_raedspinbuttonday1_value_changed(GtkSpinButton *spinbutton, gpointer user_data)
{
    int jour = gtk_spin_button_get_value_as_int(spinbutton);
    if (jour < 1 || jour > 31) {
        gtk_spin_button_set_value(spinbutton, 1);
    }
}

void on_raedspinbuttonmonth1_value_changed(GtkSpinButton *spinbutton, gpointer user_data)
{
    int mois = gtk_spin_button_get_value_as_int(spinbutton);
    if (mois < 1 || mois > 12) {
        gtk_spin_button_set_value(spinbutton, 1);
    }
}

void on_raedspinbuttonyear1_value_changed(GtkSpinButton *spinbutton, gpointer user_data)
{
    int annee = gtk_spin_button_get_value_as_int(spinbutton);
    if (annee < 2020 || annee > 2030) {
        gtk_spin_button_set_value(spinbutton, 2024);
    }
}

void on_raedradiobutton10_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Géré dans la fonction d'ajout
}

void on_raedradiobutton11_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Géré dans la fonction d'ajout
}

// ==============================================
// BOUTON AJOUTER (Admin)
// ==============================================
void on_raedbutton42_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "MalekMhatliWAdmin");
    
    if (!window) {
        afficher_erreur(NULL, "Fenêtre non trouvée!");
        return;
    }
    
    // Récupérer tous les widgets
    GtkWidget *entry_id = lookup_widget(window, "raedentryid1");
    GtkWidget *entry_nom = lookup_widget(window, "raedentrynom1");
    GtkWidget *spin_day = lookup_widget(window, "raedspinbuttonday1");
    GtkWidget *spin_month = lookup_widget(window, "raedspinbuttonmonth1");
    GtkWidget *spin_year = lookup_widget(window, "raedspinbuttonyear1");
    GtkWidget *combo_specialite = lookup_widget(window, "raedcomboboxentryspecialit__1");
    GtkWidget *entry_centre = lookup_widget(window, "raedentrycentre1");
    GtkWidget *radio_debutant = lookup_widget(window, "raedradiobutton10");
    GtkWidget *radio_professionnel = lookup_widget(window, "raedradiobutton11");
    
    if (!entry_id || !entry_nom || !spin_day || !spin_month || !spin_year || 
        !combo_specialite || !entry_centre || !radio_debutant || !radio_professionnel) {
        afficher_erreur(window, "Widgets manquants!");
        return;
    }
    
    // Récupérer les valeurs
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *centre = gtk_entry_get_text(GTK_ENTRY(entry_centre));
    
    // Validation
    if (strlen(id_str) == 0) {
        afficher_erreur(window, "L'ID est obligatoire!");
        return;
    }
    
    if (strlen(nom) == 0) {
        afficher_erreur(window, "Le nom est obligatoire!");
        return;
    }
    
    if (strlen(centre) == 0) {
        afficher_erreur(window, "Le centre est obligatoire!");
        return;
    }
    
    // Convertir ID en entier
    int id = atoi(id_str);
    if (id <= 0) {
        afficher_erreur(window, "ID invalide! Doit être un nombre positif.");
        return;
    }
    
    // Vérifier ID unique
    if (existe_cours_id(id_str)) {
        afficher_erreur(window, "Cet ID existe déjà!");
        return;
    }
    
    // Date
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_day));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_month));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_year));
    
    // Validation date
    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 2020) {
        afficher_erreur(window, "Date invalide!");
        return;
    }
    
    // Spécialité
    char specialite[50];
    if (combo_specialite && GTK_IS_COMBO_BOX(combo_specialite)) {
        gint active = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_specialite));
        if (active == 0) strcpy(specialite, "Fitness");
        else if (active == 1) strcpy(specialite, "SportCollectif");
        else if (active == 2) strcpy(specialite, "Musculation");
        else strcpy(specialite, "Fitness");
    } else {
        strcpy(specialite, "Fitness");
    }
    
    // Niveau
    char niveau[20];
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_debutant))) {
        strcpy(niveau, "Débutant");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_professionnel))) {
        strcpy(niveau, "Professionnel");
    } else {
        strcpy(niveau, "Débutant");
    }
    
    // Formater date
    char date_formatee[20];
    formater_date(jour, mois, annee, date_formatee);
    
    // Créer le cours
    Cours nouveau;
    nouveau.id = id;
    strncpy(nouveau.nom, nom, sizeof(nouveau.nom)-1);
    nouveau.nom[sizeof(nouveau.nom)-1] = '\0';
    
    strncpy(nouveau.centre, centre, sizeof(nouveau.centre)-1);
    nouveau.centre[sizeof(nouveau.centre)-1] = '\0';
    
    strncpy(nouveau.specialite, specialite, sizeof(nouveau.specialite)-1);
    nouveau.specialite[sizeof(nouveau.specialite)-1] = '\0';
    
    strncpy(nouveau.niveau, niveau, sizeof(nouveau.niveau)-1);
    nouveau.niveau[sizeof(nouveau.niveau)-1] = '\0';
    
    strncpy(nouveau.date_expiration, date_formatee, sizeof(nouveau.date_expiration)-1);
    nouveau.date_expiration[sizeof(nouveau.date_expiration)-1] = '\0';
    
    // Ajouter
    if (ajouter_cours(&nouveau)) {
        afficher_info(window, "Cours ajouté avec succès!");
        
        // Réinitialiser les champs
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");
        gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_centre), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_day), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_month), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_year), 2024);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo_specialite), 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_debutant), TRUE);
        
        // Mettre à jour les TreeView
        GtkWidget *treeview_admin = lookup_widget(window, "raedtreeviewrecherche");
        if (treeview_admin) {
            afficher_cours_dans_treeview(treeview_admin);
        }
        
        // Mettre à jour TreeView membre si accessible
        GtkWidget *treeview_membre = lookup_widget(window, "raedtreeview12");
        if (treeview_membre) {
            afficher_cours_dans_treeview(treeview_membre);
        }
    } else {
        afficher_erreur(window, "Erreur lors de l'ajout!");
    }
}

// ==============================================
// BOUTON MODIFIER (Admin)
// ==============================================
void on_raedbutton45_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "MalekMhatliWAdmin");
    
    if (!window) {
        afficher_erreur(NULL, "Fenêtre non trouvée!");
        return;
    }
    
    GtkWidget *entry_id = lookup_widget(window, "raedentryid2");
    GtkWidget *entry_nom = lookup_widget(window, "raedentrynom2");
    GtkWidget *spin_day = lookup_widget(window, "raedspinbuttonday2");
    GtkWidget *spin_month = lookup_widget(window, "raedspinbuttonmonth2");
    GtkWidget *spin_year = lookup_widget(window, "raedspinbuttonyear2");
    GtkWidget *combo_specialite = lookup_widget(window, "raedcomboboxentryspecialit__2");
    GtkWidget *entry_centre = lookup_widget(window, "raedentrycour2");
    
    if (!entry_id || !entry_nom || !spin_day || !spin_month || !spin_year || 
        !combo_specialite || !entry_centre) {
        afficher_erreur(window, "Widgets manquants!");
        return;
    }
    
    // ID à modifier
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if (strlen(id_str) == 0) {
        afficher_erreur(window, "Entrez l'ID du cours à modifier!");
        return;
    }
    
    // Vérifier existence
    Cours *existant = rechercher_cours_par_id(id_str);
    if (existant == NULL) {
        afficher_erreur(window, "Cours non trouvé!");
        return;
    }
    
    // Nouveaux valeurs
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *centre = gtk_entry_get_text(GTK_ENTRY(entry_centre));
    
    if (strlen(nom) == 0 || strlen(centre) == 0) {
        free(existant);
        afficher_erreur(window, "Nom et Centre sont obligatoires!");
        return;
    }
    
    // Date
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_day));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_month));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_year));
    
    // Spécialité
    char specialite[50];
    if (combo_specialite && GTK_IS_COMBO_BOX(combo_specialite)) {
        gint active = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_specialite));
        if (active == 0) strcpy(specialite, "Fitness");
        else if (active == 1) strcpy(specialite, "SportCollectif");
        else if (active == 2) strcpy(specialite, "Musculation");
        else strcpy(specialite, existant->specialite);
    } else {
        strcpy(specialite, existant->specialite);
    }
    
    // Formater date
    char date_formatee[20];
    formater_date(jour, mois, annee, date_formatee);
    
    // Créer cours modifié
    Cours modifie;
    modifie.id = existant->id;  // Garder le même ID
    strncpy(modifie.nom, nom, sizeof(modifie.nom)-1);
    modifie.nom[sizeof(modifie.nom)-1] = '\0';
    
    strncpy(modifie.centre, centre, sizeof(modifie.centre)-1);
    modifie.centre[sizeof(modifie.centre)-1] = '\0';
    
    strncpy(modifie.specialite, specialite, sizeof(modifie.specialite)-1);
    modifie.specialite[sizeof(modifie.specialite)-1] = '\0';
    
    strncpy(modifie.niveau, existant->niveau, sizeof(modifie.niveau)-1);
    modifie.niveau[sizeof(modifie.niveau)-1] = '\0';
    
    strncpy(modifie.date_expiration, date_formatee, sizeof(modifie.date_expiration)-1);
    modifie.date_expiration[sizeof(modifie.date_expiration)-1] = '\0';
    
    // Modifier
    char id_temp[20];
    sprintf(id_temp, "%d", existant->id);
    
    if (supprimer_cours(id_temp) && ajouter_cours(&modifie)) {
        afficher_info(window, "Cours modifié avec succès!");
        
        // Réinitialiser
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");
        gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_centre), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_day), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_month), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_year), 2024);
        
        // Mettre à jour les TreeView
        GtkWidget *treeview_admin = lookup_widget(window, "raedtreeviewrecherche");
        if (treeview_admin) {
            afficher_cours_dans_treeview(treeview_admin);
        }
        
        GtkWidget *treeview_membre = lookup_widget(window, "raedtreeview12");
        if (treeview_membre) {
            afficher_cours_dans_treeview(treeview_membre);
        }
    } else {
        afficher_erreur(window, "Erreur lors de la modification!");
    }
    
    free(existant);
}

// ==============================================
// BOUTON SUPPRIMER (Admin)
// ==============================================
void on_raedbutton43_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "MalekMhatliWAdmin");
    
    if (!window) {
        afficher_erreur(NULL, "Fenêtre non trouvée!");
        return;
    }
    
    GtkWidget *entry_id = lookup_widget(window, "raedentryid3");
    
    if (!entry_id) {
        afficher_erreur(window, "Widget ID non trouvé!");
        return;
    }
    
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if (strlen(id_str) == 0) {
        afficher_erreur(window, "Entrez l'ID du cours à supprimer!");
        return;
    }
    
    // Vérifier existence
    Cours *c = rechercher_cours_par_id(id_str);
    if (c == NULL) {
        afficher_erreur(window, "Cours non trouvé!");
        return;
    }
    
    // Confirmation
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Supprimer le cours '%s' (ID: %s)?", c->nom, id_str);
    
    gint reponse = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    if (reponse == GTK_RESPONSE_YES) {
        if (supprimer_cours(id_str)) {
            afficher_info(window, "Cours supprimé avec succès!");
            
            gtk_entry_set_text(GTK_ENTRY(entry_id), "");
            
            // Mettre à jour les TreeView
            GtkWidget *treeview_admin = lookup_widget(window, "raedtreeviewrecherche");
            if (treeview_admin) {
                afficher_cours_dans_treeview(treeview_admin);
            }
            
            GtkWidget *treeview_membre = lookup_widget(window, "raedtreeview12");
            if (treeview_membre) {
                afficher_cours_dans_treeview(treeview_membre);
            }
        } else {
            afficher_erreur(window, "Erreur lors de la suppression!");
        }
    }
    
    free(c);
}

// ==============================================
// FENÊTRE MEMBRE (CheckButtons)
// ==============================================
void on_raedcheckbutton5_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        niveau_selectionne = 1;
        GtkWidget *autre = lookup_widget(GTK_WIDGET(togglebutton), "raedcheckbutton6");
        if (autre)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(autre), FALSE);
    }
}

void on_raedcheckbutton6_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        niveau_selectionne = 2;
        GtkWidget *autre = lookup_widget(GTK_WIDGET(togglebutton), "raedcheckbutton5");
        if (autre)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(autre), FALSE);
    }
}

// ==============================================
// FONCTION POUR VÉRIFIER SI UN COURS EXISTE DÉJÀ
// ==============================================
static int cours_existe_deja(const char *nom_cours, const char *centre, 
                           const char *specialite, const char *niveau) {
    FILE *f = fopen("cours.txt", "r");
    if (!f) {
        return 0; // Fichier n'existe pas, donc cours n'existe pas
    }
    
    int id;
    char nom[100], cent[100], spec[50], niv[20], date[20];
    int trouve = 0;
    
    while (fscanf(f, "%d %s %s %s %s %s", 
                  &id, nom, cent, spec, niv, date) == 6) {
        
        // Comparer tous les critères
        if (strcmp(nom, nom_cours) == 0 &&
            strcmp(cent, centre) == 0 &&
            strcmp(spec, specialite) == 0 &&
            strcmp(niv, niveau) == 0) {
            trouve = 1;
            break;
        }
    }
    
    fclose(f);
    return trouve;
}

// ==============================================
// FONCTION POUR VÉRIFIER SI L'UTILISATEUR EST DÉJÀ INSCRIT
// ==============================================
static int est_deja_inscrit(const char *nom_cours, const char *centre, 
                          const char *specialite, const char *niveau) {
    FILE *f = fopen("inscriptions.txt", "r");
    if (!f) {
        return 0; // Fichier n'existe pas
    }
    
    char ligne[512];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        // Chercher les informations dans la ligne
        if (strstr(ligne, nom_cours) && 
            strstr(ligne, centre) &&
            strstr(ligne, specialite) &&
            strstr(ligne, niveau)) {
            trouve = 1;
            break;
        }
    }
    
    fclose(f);
    return trouve;
}

// ==============================================
// BOUTON INSCRIRE (Membre) - VERSION MODIFIÉE
// ==============================================
void on_raedbuttoninscrire5_clicked(GtkWidget *button, gpointer user_data) {
    g_print("\n\n=== BOUTON INSCRIRE CLIQUE ===\n");
    
    // 1. Trouver la fenêtre
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "RaedWindowMembre");
    if (!window) {
        window = GTK_WIDGET(button);
        g_print("1. Utilisation de la fenêtre courante\n");
    } else {
        g_print("1. Fenêtre RaedWindowMembre trouvée\n");
    }
    
    // 2. Chercher les widgets
    GtkWidget *entry_centre = lookup_widget(window, "raedentrycentre5");
    GtkWidget *entry_cour = lookup_widget(window, "raedentrycour5");
    
    g_print("2. Recherche des widgets:\n");
    g_print("   entry_centre: %p\n", entry_centre);
    g_print("   entry_cour: %p\n", entry_cour);
    
    if (!entry_centre || !entry_cour) {
        g_print("ERREUR: Widgets critiques non trouvés!\n");
        afficher_erreur(window, "Champs 'centre' ou 'cours' non trouvés!");
        return;
    }
    
    // 3. Récupérer les valeurs
    const char *centre = gtk_entry_get_text(GTK_ENTRY(entry_centre));
    const char *cour = gtk_entry_get_text(GTK_ENTRY(entry_cour));
    
    g_print("3. Valeurs récupérées:\n");
    g_print("   Centre: '%s'\n", centre);
    g_print("   Cours: '%s'\n", cour);
    
    // 4. Validation BASIQUE
    if (!centre || !cour || strlen(centre) == 0 || strlen(cour) == 0) {
        g_print("ERREUR: Champs vides!\n");
        afficher_erreur(window, "Veuillez remplir le centre et le cours!");
        return;
    }
    
    // 5. Récupérer la spécialité
    char specialite[50] = "Fitness";
    GtkWidget *combobox_specialite = lookup_widget(window, "raedcomboboxxentryspecialité5");
    if (combobox_specialite && GTK_IS_COMBO_BOX(combobox_specialite)) {
        gint active = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_specialite));
        g_print("4. Spécialité index: %d\n", active);
        
        if (active == 0) strcpy(specialite, "Fitness");
        else if (active == 1) strcpy(specialite, "SportCollectif");
        else if (active == 2) strcpy(specialite, "Musculation");
    }
    
    // 6. Récupérer le niveau
    char niveau[20] = "Débutant";
    GtkWidget *check_debutant = lookup_widget(window, "raedcheckbutton5");
    GtkWidget *check_pro = lookup_widget(window, "raedcheckbutton6");
    
    gboolean debutant_actif = FALSE;
    gboolean pro_actif = FALSE;
    
    if (check_debutant) debutant_actif = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_debutant));
    if (check_pro) pro_actif = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_pro));
    
    g_print("5. États des checkbuttons:\n");
    g_print("   Débutant: %s\n", debutant_actif ? "ACTIF" : "INACTIF");
    g_print("   Professionnel: %s\n", pro_actif ? "ACTIF" : "INACTIF");
    
    if (pro_actif) {
        strcpy(niveau, "Professionnel");
        niveau_selectionne = 2;
    } else {
        strcpy(niveau, "Débutant");
        niveau_selectionne = 1;
    }
    
    // 7. VÉRIFIER SI LE COURS EXISTE DANS cours.txt
    g_print("6. Vérification si le cours existe...\n");
    
    if (!cours_existe_deja(cour, centre, specialite, niveau)) {
        char message[512];
        snprintf(message, sizeof(message),
                "❌ COURS NON TROUVÉ !\n\n"
                "Le cours spécifié n'existe pas dans notre base de données.\n"
                "Veuillez vérifier:\n"
                "• Nom du cours: %s\n"
                "• Centre: %s\n"
                "• Spécialité: %s\n"
                "• Niveau: %s\n\n"
                "Seuls les cours créés par l'administrateur sont disponibles.",
                cour, centre, specialite, niveau);
        
        afficher_erreur(window, message);
        return;
    }
    
    // 8. VÉRIFIER SI L'UTILISATEUR EST DÉJÀ INSCRIT
    g_print("7. Vérification si déjà inscrit...\n");
    
    if (est_deja_inscrit(cour, centre, specialite, niveau)) {
        char message[512];
        snprintf(message, sizeof(message),
                "⚠️  DÉJÀ INSCRIT !\n\n"
                "Vous êtes déjà inscrit à ce cours:\n"
                "• Cours: %s\n"
                "• Centre: %s\n"
                "• Spécialité: %s\n"
                "• Niveau: %s\n\n"
                "Chaque membre ne peut s'inscrire qu'une fois par cours.",
                cour, centre, specialite, niveau);
        
        afficher_erreur(window, message);
        return;
    }
    
    // 9. SAUVEGARDER L'INSCRIPTION (uniquement dans inscriptions.txt)
    g_print("8. Sauvegarde de l'inscription...\n");
    
    // Préparer la date
    time_t maintenant = time(NULL);
    struct tm *temps = localtime(&maintenant);
    char date[20];
    sprintf(date, "%02d/%02d/%04d", 
            temps->tm_mday, 
            temps->tm_mon + 1, 
            temps->tm_year + 1900);
    
    // Ouvrir le fichier d'inscriptions
    FILE *f = fopen("inscriptions.txt", "a");
    if (!f) {
        // Essayer de créer le fichier
        f = fopen("inscriptions.txt", "w");
        if (!f) {
            g_print("ERREUR: Impossible de créer inscriptions.txt\n");
            afficher_erreur(window, "Erreur de sauvegarde!");
            return;
        }
    }
    
    // Écrire l'inscription
    fprintf(f, "Date: %s | Centre: %s | Spécialité: %s | Cours: %s | Niveau: %s | Statut: Inscrit\n",
            date, centre, specialite, cour, niveau);
    
    // Forcer l'écriture
    fflush(f);
    fclose(f);
    
    g_print("9. Inscription sauvegardée avec succès!\n");
    
    // 10. NE PAS AJOUTER AU FICHIER cours.txt
    g_print("10. Le cours n'est PAS ajouté à cours.txt (il existe déjà)\n");
    
    // 11. Mettre à jour le TreeView (pour afficher les cours disponibles)
    GtkWidget *treeview = lookup_widget(window, "raedtreeview12");
    if (treeview) {
        afficher_cours_dans_treeview(treeview);
        g_print("11. TreeView mis à jour\n");
    }
    
    // 12. Réinitialiser les champs
    gtk_entry_set_text(GTK_ENTRY(entry_centre), "");
    gtk_entry_set_text(GTK_ENTRY(entry_cour), "");
    
    if (combobox_specialite) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_specialite), 0);
    }
    
    if (check_debutant) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_debutant), FALSE);
    if (check_pro) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_pro), FALSE);
    niveau_selectionne = 0;
    
    // 13. Message de confirmation
    char message[512];
    snprintf(message, sizeof(message),
            "✅ INSCRIPTION RÉUSSIE !\n\n"
            "Vous êtes maintenant inscrit au cours:\n"
            "• Centre: %s\n"
            "• Cours: %s\n"
            "• Spécialité: %s\n"
            "• Niveau: %s\n\n"
            "📅 Date d'inscription: %s\n"
            "📁 Inscription sauvegardée dans inscriptions.txt",
            centre, cour, specialite, niveau, date);
    
    afficher_info(window, message);
    
    g_print("12. Message affiché à l'utilisateur\n");
    g_print("=== FIN BOUTON INSCRIRE ===\n\n");
}

// ==============================================
// INITIALISATION FENÊTRE MEMBRE
// ==============================================
void initialiser_fenetre_membre(GtkWidget *window) {
    g_print("Initialisation fenêtre membre...\n");
    
    // Initialiser le combobox
    GtkWidget *combobox = lookup_widget(window, "raedcomboboxxentryspecialité5");
    if (combobox) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0);
        g_print("Combobox initialisé\n");
    }
    
    // Initialiser le TreeView
    GtkWidget *treeview = lookup_widget(window, "raedtreeview12");
    if (treeview) {
        afficher_cours_dans_treeview(treeview);
        g_print("TreeView initialisé\n");
    }
    
    // Créer le fichier inscriptions.txt s'il n'existe pas
    FILE *f = fopen("inscriptions.txt", "a");
    if (f) {
        fprintf(f, "=== FICHIER D'INSCRIPTIONS ===\n");
        fprintf(f, "Créé le: ");
        time_t maintenant = time(NULL);
        struct tm *temps = localtime(&maintenant);
        fprintf(f, "%02d/%02d/%04d\n\n", 
                temps->tm_mday, temps->tm_mon + 1, temps->tm_year + 1900);
        fclose(f);
        g_print("Fichier inscriptions.txt créé/initialisé\n");
    }
}

// ==============================================
// ALIGNMENT CLICKED
// ==============================================
void on_alignment33_clicked(GtkWidget *widget, gpointer user_data) {
    // Fonction vide
    g_print("Alignment cliqué\n");
}
