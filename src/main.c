#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "support.h"
#include "callbacks.h"
#include "cour.h"

char id_login[20] = "0000";

int main(int argc, char *argv[]) {
#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif
    
    gtk_set_locale ();
    gtk_init(&argc, &argv);
    
    add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
    
    /* Créer les deux fenêtres */
    GtkWidget *window_admin = create_MalekMhatliWAdmin();
    GtkWidget *window_membre = create_RaedWindowMembre();
    
    /* Initialiser la fenêtre admin */
    if (window_admin) {
        GtkWidget *treeview = lookup_widget(window_admin, "raedtreeviewrecherche");
        if (treeview) {
            afficher_tous_cours(treeview);
        }
        gtk_widget_show_all(window_admin);
    }
    
    /* Initialiser la fenêtre membre */
    if (window_membre) {
        initialiser_fenetre_membre(window_membre);
        gtk_widget_show_all(window_membre);
    }
    
    if (window_admin == NULL && window_membre == NULL) {
        g_print("Erreur: Aucune fenêtre créée\n");
        return 1;
    }
    
    gtk_main();
    
    return 0;
}
