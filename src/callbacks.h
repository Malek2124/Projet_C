#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

extern char id_login[20];

// Prototypes des fonctions de callback
void on_raedbuttonrecherche_clicked(GtkButton *button, gpointer user_data);
void on_raedspinbuttonday1_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_raedspinbuttonmonth1_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_raedspinbuttonyear1_value_changed(GtkSpinButton *spinbutton, gpointer user_data);
void on_raedradiobutton10_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_raedradiobutton11_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_raedbutton42_clicked(GtkButton *button, gpointer user_data);
void on_raedbutton45_clicked(GtkButton *button, gpointer user_data);
void on_raedbutton43_clicked(GtkButton *button, gpointer user_data);
void on_raedbuttoninscrire5_clicked(GtkWidget *button, gpointer user_data);
void on_raedcheckbutton5_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_raedcheckbutton6_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_raedtreeviewrecherche_row_activated(GtkTreeView *treeview, 
                                           GtkTreePath *path, 
                                           GtkTreeViewColumn *column, 
                                           gpointer user_data);

// Fonction d'initialisation
void initialiser_fenetre_membre(GtkWidget *window);
void on_alignment33_clicked(GtkWidget *widget, gpointer user_data);

#endif /* CALLBACKS_H */


