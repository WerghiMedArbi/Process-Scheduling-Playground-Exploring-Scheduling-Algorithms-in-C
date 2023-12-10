#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include "callbacks.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Scheduling Playground");
    gtk_window_set_default_size(GTK_WINDOW(window), 870, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrolled_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 15);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);

    

    
    GtkWidget *process_label = gtk_label_new("Number of Processes:");
    GtkWidget *process_entry = gtk_entry_new();// Add an empty label as a spacer in the first column
GtkWidget *spacer1_label = gtk_label_new("                                                   ");
gtk_grid_attach(GTK_GRID(grid), spacer1_label, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), process_label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), process_entry, 2, 0, 2, 1);
    g_signal_connect(process_entry, "changed", G_CALLBACK(on_process_entry_changed), NULL);
    
    
    
GtkWidget *algorithm_label = gtk_label_new("Scheduling Algorithm:");
GtkWidget *algorithm_combo = gtk_combo_box_text_new();
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "PRIORITY");
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "SRT");
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "MULTILEVEL");
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "ROUND-ROBIN");
GtkWidget *spacer2_label = gtk_label_new("                                                   ");
gtk_grid_attach(GTK_GRID(grid), spacer2_label, 0, 10, 1, 1);

gtk_grid_attach(GTK_GRID(grid), algorithm_label, 1, 10, 1, 1);
gtk_grid_attach(GTK_GRID(grid), algorithm_combo, 2, 10,1, 1);

GtkWidget *scheduling_button = gtk_button_new_with_label("Let's start!");
gtk_grid_attach(GTK_GRID(grid), scheduling_button, 3, 10, 1, 1);

g_signal_connect(scheduling_button, "clicked", G_CALLBACK(on_scheduling_button_clicked), window);

g_object_set_data(G_OBJECT(scheduling_button), "algorithm_combo", algorithm_combo);



    
    GtkWidget *arrival_label = gtk_label_new("Arrival Time:");
    GtkWidget *min_arrival_entry = gtk_entry_new();
    GtkWidget *max_arrival_entry = gtk_entry_new();
GtkWidget *spacer3_label = gtk_label_new("                                                ");
gtk_grid_attach(GTK_GRID(grid), spacer3_label, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), arrival_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), min_arrival_entry, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_arrival_entry, 3, 1, 1, 1);

    GtkWidget *execution_label = gtk_label_new("Execution Time:");
    GtkWidget *min_execution_entry = gtk_entry_new();
    GtkWidget *max_execution_entry = gtk_entry_new();
    
GtkWidget *spacer4_label = gtk_label_new("                                                 ");
gtk_grid_attach(GTK_GRID(grid), spacer4_label, 0, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), execution_label, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), min_execution_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_execution_entry, 3, 2, 1, 1);

    GtkWidget *priority_label = gtk_label_new("Priority:");
    GtkWidget *min_priority_entry = gtk_entry_new();
    GtkWidget *max_priority_entry = gtk_entry_new();
GtkWidget *spacer5_label = gtk_label_new("                                               ");
gtk_grid_attach(GTK_GRID(grid), spacer5_label, 0, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), priority_label, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), min_priority_entry, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_priority_entry, 3, 3, 1, 1);

    // Create a label and entry for the file name
    GtkWidget *file_label = gtk_label_new("File Name:");
    GtkWidget *file_entry = gtk_entry_new();
GtkWidget *spacer6_label = gtk_label_new("                          ");
gtk_grid_attach(GTK_GRID(grid), spacer6_label, 0, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), file_label, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), file_entry, 2, 4, 2, 1);

    GtkWidget *save_button = gtk_button_new_with_label("Save Processes");  

GtkWidget *spacer7_label = gtk_label_new("                                                   ");
gtk_grid_attach(GTK_GRID(grid), spacer7_label, 0, 8, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), save_button, 1, 8, 3, 1);

    g_object_set_data(G_OBJECT(save_button), "file_entry", file_entry);
    g_object_set_data(G_OBJECT(save_button), "process_entry", process_entry);
    g_object_set_data(G_OBJECT(save_button), "min_arrival_entry", min_arrival_entry);
    g_object_set_data(G_OBJECT(save_button), "max_arrival_entry", max_arrival_entry);
    g_object_set_data(G_OBJECT(save_button), "min_execution_entry", min_execution_entry);
    g_object_set_data(G_OBJECT(save_button), "max_execution_entry", max_execution_entry);
    g_object_set_data(G_OBJECT(save_button), "min_priority_entry", min_priority_entry);
    g_object_set_data(G_OBJECT(save_button), "max_priority_entry", max_priority_entry);

    
    g_signal_connect(min_arrival_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(max_arrival_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(min_execution_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(max_execution_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(min_priority_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(max_priority_entry, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD); // Wrap words
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    
    GtkWidget *see_results_button = gtk_button_new_with_label("See Results");
    
    g_object_set_data(G_OBJECT(see_results_button), "file_entry", file_entry);
    g_signal_connect(see_results_button, "clicked", G_CALLBACK(on_see_results_button_clicked), text_view);
    
GtkWidget *spacer9_label = gtk_label_new("                                                   ");
gtk_grid_attach(GTK_GRID(grid), spacer9_label, 0, 9, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), see_results_button, 1, 9, 1, 1);
    
    GtkWidget *update_button = gtk_button_new_with_label("Update");

    g_object_set_data(G_OBJECT(update_button), "file_entry", file_entry);
    g_signal_connect(update_button, "clicked", G_CALLBACK(on_update_button_clicked), text_view);
    gtk_widget_set_visible(update_button, FALSE); 
      
    gtk_grid_attach(GTK_GRID(grid), update_button, 2, 9, 1, 1);

    GtkTextBuffer *text_view_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    g_signal_connect(text_view_buffer, "changed", G_CALLBACK(on_text_view_changed), update_button);


    GtkWidget *choose_file_button = gtk_button_new_with_label("Choose File");

    g_signal_connect(choose_file_button, "clicked", G_CALLBACK(on_choose_file_button_clicked), file_entry);


    gtk_grid_attach(GTK_GRID(grid), choose_file_button, 3, 9, 1, 1);
      // Create a button to choose a file
    GtkWidget *made_button = gtk_button_new_with_label("Made with ❤️");
  

    gtk_grid_attach(GTK_GRID(grid), made_button, 1, 19, 3, 1);
    g_signal_connect(made_button, "clicked", G_CALLBACK(on_made_button_clicked), NULL);


    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

