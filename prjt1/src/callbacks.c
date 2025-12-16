#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "callbacks.h"
#include "support.h"

int choix_radio = 0; // 0 = non choisi, 10 = débutant, 11 = professionnel

void on_raedbutton42_clicked(GtkWidget *objet_graphique, gpointer user_data) {
    GtkWidget *entry_id, *entry_nom, *combo_specialite, *entry_centre;
    GtkWidget *spin_day, *spin_month, *spin_year;
    
    char id[50], nom[100], specialite[100], centre[100];
    int jour, mois, annee;
    char niveau[50];

    // Récupérer les widgets par leur nom
    entry_id = lookup_widget(objet_graphique, "raedentryid1");
    entry_nom = lookup_widget(objet_graphique, "raedentrynom1");
    combo_specialite = lookup_widget(objet_graphique, "raedcomboboxentryspecialité1");
    entry_centre = lookup_widget(objet_graphique, "raedentrycentre1");
    spin_day = lookup_widget(objet_graphique, "raedspinbuttonday1");
    spin_month = lookup_widget(objet_graphique, "raedspinbuttonmonth1");
    spin_year = lookup_widget(objet_graphique, "raedspinbuttonyear1");

    // Récupérer les valeurs
    strcpy(id, gtk_entry_get_text(GTK_ENTRY(entry_id)));
    strcpy(nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    
    // Pour ComboBox
    gchar *temp_specialite = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_specialite));
    if (temp_specialite) {
        strcpy(specialite, temp_specialite);
        g_free(temp_specialite);
    } else {
        strcpy(specialite, "Non sélectionné");
    }
    
    strcpy(centre, gtk_entry_get_text(GTK_ENTRY(entry_centre)));

    jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_day));
    mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_month));
    annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_year));

    // Déterminer le niveau selon le choix radio
    if (choix_radio == 10) {
        strcpy(niveau, "Débutant");
    } else if (choix_radio == 11) {
        strcpy(niveau, "Professionnel");
    } else {
        strcpy(niveau, "Non défini");
    }

    // Afficher dans la console
    printf("=== Données saisies ===\n");
    printf("ID: %s\n", id);
    printf("Nom: %s\n", nom);
    printf("Spécialité: %s\n", specialite);
    printf("Centre: %s\n", centre);
    printf("Date: %02d/%02d/%04d\n", jour, mois, annee);
    printf("Niveau: %s\n", niveau);
    printf("=======================\n");
}

void on_raedradiobutton10_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        choix_radio = 10;
        printf("Niveau sélectionné : Débutant\n");
    }
}

void on_raedradiobutton11_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        choix_radio = 11;
        printf("Niveau sélectionné : Professionnel\n");
    }
}
