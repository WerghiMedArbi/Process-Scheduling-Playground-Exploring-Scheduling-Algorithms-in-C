#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include "process_generator.h" 

void on_destroy(GtkWidget *widget, gpointer data);
void on_process_entry_changed(GtkEditable *editable, gpointer user_data);
void on_save_button_clicked(GtkWidget *button, gpointer data);
void on_see_results_button_clicked(GtkWidget *button, gpointer data);
void on_choose_file_button_clicked(GtkWidget *button, gpointer data);
void on_update_button_clicked(GtkWidget *button, gpointer data);
void on_text_view_changed(GtkTextBuffer *buffer, gpointer data);
void on_entry_changed(GtkEditable *editable, gpointer user_data);
struct Process* fifoScheduling(struct Process processes[], int n);
struct Process* srtfScheduling(struct Process processes[], int n);
struct Process* priorityScheduling(struct Process processes[], int n);
struct Process* roundRobinScheduling(struct Process processes[], int n, int quantum);
struct Process* multilevelScheduling(struct Process processes[], int n, int quantum);
void on_scheduling_button_clicked(GtkWidget *widget, gpointer data);
void print_results(struct Process* rs,const gchar *selected_item,struct Process* processes, int n);
void on_made_button_clicked(GtkButton *button, gpointer user_data) ;

#endif

