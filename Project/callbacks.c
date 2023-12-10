#include <gtk/gtk.h>
#include "process_generator.h" 
#include "callbacks.h"
#include "process_generator.h"

void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_process_entry_changed(GtkEditable *editable, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(editable));
    gchar *new_text = g_strdup(text);

    for (int i = 0; i < strlen(text); ++i) {
        if (!g_ascii_isdigit(text[i])) {
            new_text[i] = ' ';
        }
    }

    gtk_entry_set_text(GTK_ENTRY(editable), new_text);
    g_free(new_text);
}

void on_save_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *file_entry = g_object_get_data(G_OBJECT(button), "file_entry");
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(file_entry));

    GtkWidget *process_entry = g_object_get_data(G_OBJECT(button), "process_entry");
    const gchar *num_processes_str = gtk_entry_get_text(GTK_ENTRY(process_entry));

    GtkWidget *min_arrival_entry = g_object_get_data(G_OBJECT(button), "min_arrival_entry");
    GtkWidget *max_arrival_entry = g_object_get_data(G_OBJECT(button), "max_arrival_entry");
    GtkWidget *min_execution_entry = g_object_get_data(G_OBJECT(button), "min_execution_entry");
    GtkWidget *max_execution_entry = g_object_get_data(G_OBJECT(button), "max_execution_entry");
    GtkWidget *min_priority_entry = g_object_get_data(G_OBJECT(button), "min_priority_entry");
    GtkWidget *max_priority_entry = g_object_get_data(G_OBJECT(button), "max_priority_entry");

    const gchar *min_arrival_str = gtk_entry_get_text(GTK_ENTRY(min_arrival_entry));
    const gchar *max_arrival_str = gtk_entry_get_text(GTK_ENTRY(max_arrival_entry));
    const gchar *min_execution_str = gtk_entry_get_text(GTK_ENTRY(min_execution_entry));
    const gchar *max_execution_str = gtk_entry_get_text(GTK_ENTRY(max_execution_entry));
    const gchar *min_priority_str = gtk_entry_get_text(GTK_ENTRY(min_priority_entry));
    const gchar *max_priority_str = gtk_entry_get_text(GTK_ENTRY(max_priority_entry));

  if (g_strcmp0(filename, "") == 0 || g_strcmp0(num_processes_str, "") == 0 ||
        g_strcmp0(min_arrival_str, "") == 0 || g_strcmp0(max_arrival_str, "") == 0 ||
        g_strcmp0(min_execution_str, "") == 0 || g_strcmp0(max_execution_str, "") == 0 ||
        g_strcmp0(min_priority_str, "") == 0 || g_strcmp0(max_priority_str, "") == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                         "Please enter all the required information.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    int numProcesses = atoi(num_processes_str);
    int minArrival = atoi(min_arrival_str);
    int maxArrival = atoi(max_arrival_str);
    int minExecution = atoi(min_execution_str);
    int maxExecution = atoi(max_execution_str);
    int minPriority = atoi(min_priority_str);
    int maxPriority = atoi(max_priority_str);

    
    if (minArrival >= maxArrival || minExecution >= maxExecution || minPriority >= maxPriority) {
    
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                         "Invalid parameter values. Make sure min < max for arrival, execution, and priority.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    
    struct Process *processes = (struct Process *)malloc(numProcesses * sizeof(struct Process));
    if (processes == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    generateParameterizedProcesses(processes, numProcesses, minArrival, maxArrival,
                                   minExecution, maxExecution, minPriority, maxPriority);

    saveToFile(processes, numProcesses, filename);

    free(processes);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                              "✨Processes generated and saved successfully✨");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


const char *selected_filename = NULL;
void on_see_results_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *file_entry = g_object_get_data(G_OBJECT(button), "file_entry");
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(file_entry));

    GtkWidget *text_view = GTK_WIDGET(data);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        perror("Memory allocation error");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    selected_filename = filename;
    
    
    
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';
    fclose(file);

    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), TRUE);

    
    gtk_text_buffer_set_text(buffer, file_content, -1);
    
    free(file_content);
}




void on_choose_file_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *file_entry = GTK_WIDGET(data);

    
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose a .txt file", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    
    if (response == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_entry_set_text(GTK_ENTRY(file_entry), filename);
        g_free(filename);
    }

    
    gtk_widget_destroy(dialog);
}

void on_update_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *file_entry = g_object_get_data(G_OBJECT(button), "file_entry");
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(file_entry));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gchar *content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        g_free(content);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", content);

    fclose(file);

    g_free(content);

    
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                              "File updated successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_text_view_changed(GtkTextBuffer *buffer, gpointer data) {
    GtkWidget *update_button = GTK_WIDGET(data);
    gtk_widget_set_visible(update_button, TRUE);
}

void on_entry_changed(GtkEditable *editable, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(editable));
    gchar *new_text = g_strdup(text);

    for (int i = 0; i < strlen(text); ++i) {
        if (!g_ascii_isdigit(text[i])) {
            new_text[i] = ' ';
        }
    }

    gtk_entry_set_text(GTK_ENTRY(editable), new_text);
    g_free(new_text);
}

void print_results(struct Process* rs,const gchar *selected_item,struct Process* processes, int n){
       
        int s=0;
	for (int i = 0; i < n; ++i)
	  {s += processes[i].dureeExecution;}
        

           GtkWidget *resultsWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gchar *window_title = g_strdup_printf("Selected scheduling algorithm: %s", selected_item);

            gtk_window_set_title(GTK_WINDOW(resultsWindow), window_title );
            gtk_window_set_default_size(GTK_WINDOW(resultsWindow), 800, 400);
          
            
            GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
            gtk_container_set_border_width(GTK_CONTAINER(scrolledWindow), 10);
            gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);


            GtkWidget *resultBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
            gchar *scheduling_order_text = g_strdup_printf(" Gantt Chart for Scheduled Processes using: %s",selected_item);
            GtkWidget *schedulingOrderLabel = gtk_label_new(scheduling_order_text);

            gtk_widget_set_halign(schedulingOrderLabel, GTK_ALIGN_START);
            gtk_box_pack_start(GTK_BOX(resultBox), schedulingOrderLabel, FALSE, FALSE, 0);
          

            GtkWidget *grid = gtk_grid_new();
            gtk_grid_set_column_spacing(GTK_GRID(grid),2);
            gtk_grid_set_row_spacing(GTK_GRID(grid), 10); 
            gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);



  	const char *Colors[] = {
   	 "#B0926A", "#E1C78F", "#cea96c", "#A37455", "#E48F45", "#A9B388", "#c1c1c1", "#CA965C",
   	 "#EEC373", "#739072", "#E2C799", "#F2aC7E", "#87805E", "#d8cca0"
	};

	
	int numColors = sizeof(Colors) / sizeof(Colors[0]);
	int i=0;
	int duration =0;
	int starTime = rs[0].dateArrivee;
	char str[20]="";


         while (i<s){

	    GtkWidget *eventBox = gtk_event_box_new();
	    GtkWidget *frame = gtk_frame_new(NULL);
	    gtk_container_add(GTK_CONTAINER(frame), eventBox);
	  

 	 int matchingIndex = -1;
        for (int j = 0; j < i; ++j) {
            if (strcmp(rs[j].nom, rs[i].nom) == 0) {
                matchingIndex = j;
                break;
            }
        }

        if (matchingIndex != -1) {
            
            sprintf(rs[i].color, "%s", rs[matchingIndex].color);
        } else {
            
            int randomColorIndex;
            int j=0;
            randomColorIndex = rand() % numColors;
            do {
              if(strcmp(rs[j].color, Colors[randomColorIndex]) == 0)
                randomColorIndex = rand() % numColors;
              j++;
            } while ( j<i);

            sprintf(rs[i].color, "%s", Colors[randomColorIndex]);
        }



            GdkRGBA color;
	    gdk_rgba_parse(&color, rs[i].color); 
	    gtk_widget_override_background_color(eventBox, GTK_STATE_FLAG_NORMAL, &color);


           gtk_box_pack_start(GTK_BOX(resultBox), grid, FALSE, FALSE, 0);

	    gchar *process_label_text = g_strdup_printf("%s",rs[i].nom);
	    GtkWidget *processLabel = gtk_label_new(process_label_text);



	    gtk_container_add(GTK_CONTAINER(eventBox), processLabel);

	   

	    if(rs[i].dateArrivee>(i+ starTime)){
	       gchar *time_label = g_strdup_printf("%d", i+ starTime);
	       GtkWidget *timeLabel1 = gtk_label_new(time_label);
	       g_free(time_label);
	       gtk_grid_attach(GTK_GRID(grid), timeLabel1, (duration * 25) - 1, 14, 1, 1);
	       
	       starTime += rs[i].dateArrivee - (i+ starTime);
	    }
	      
	    gchar *time_label_text = g_strdup_printf("%d", i+ starTime);
	    GtkWidget *timeLabel = gtk_label_new(time_label_text);
	    g_free(time_label_text);

	    
	    
	    const char *currentproc =rs[i].nom;
	    while(strcmp(rs[i].nom,currentproc)==0)
	      i++;
	     
	    gtk_grid_attach(GTK_GRID(grid), frame, duration * 25, 9, (i - duration)*10, 5);
	    
	    gtk_grid_attach(GTK_GRID(grid), timeLabel, duration * 25, 14, 1, 1);
	    duration = i;

      }
	  
	    gchar *time_label_text = g_strdup_printf("%d", s+ starTime);
	    GtkWidget *timeLabel = gtk_label_new(time_label_text);
	    g_free(time_label_text);
	
	    gtk_grid_attach(GTK_GRID(grid), timeLabel, s * 25, 14, 1, 1);
	    

	    GtkWidget *tableGrid = gtk_grid_new();
	    gtk_container_add(GTK_CONTAINER(resultBox), tableGrid);
	    gtk_grid_set_column_spacing(GTK_GRID(tableGrid), 10);
	    gtk_grid_set_column_homogeneous(GTK_GRID(tableGrid), FALSE);
            gtk_grid_set_column_spacing(GTK_GRID(tableGrid), 20);
            

	    GtkWidget *header_label1 = gtk_label_new("Processus");
	    GtkWidget *header_label2 = gtk_label_new("Waiting time");
	    GtkWidget *header_label3 = gtk_label_new("Turn around time");

	    gtk_grid_attach(GTK_GRID(tableGrid), header_label1, 0, 0, 1, 1);
	    gtk_grid_attach(GTK_GRID(tableGrid), header_label2, 1, 0, 1, 1);
	    gtk_grid_attach(GTK_GRID(tableGrid), header_label3, 2, 0, 1, 1);

	    for (int i = 0; i < n; i++) {
	
		GtkWidget *label1 = gtk_label_new(g_strdup_printf("%s", processes[i].nom));
		GtkWidget *label2 = gtk_label_new(g_strdup_printf("%d", processes[i].waitingTime));
		GtkWidget *label3 = gtk_label_new(g_strdup_printf("%d", processes[i].turnAroundTime));

	
		GtkWidget *frame1 = gtk_frame_new(NULL);
		GtkWidget *frame2 = gtk_frame_new(NULL);
		GtkWidget *frame3 = gtk_frame_new(NULL);

		gtk_container_add(GTK_CONTAINER(frame1), label1);
		gtk_container_add(GTK_CONTAINER(frame2), label2);
		gtk_container_add(GTK_CONTAINER(frame3), label3);

		gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);
		gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_IN);
		gtk_frame_set_shadow_type(GTK_FRAME(frame3), GTK_SHADOW_IN);

		gtk_grid_attach(GTK_GRID(tableGrid), frame1, 0, i+1, 1, 1);
		gtk_grid_attach(GTK_GRID(tableGrid), frame2, 1, i+1, 1, 1);
		gtk_grid_attach(GTK_GRID(tableGrid), frame3, 2, i+1, 1, 1);
	    }
	    
            double avgWaitingTime = 0.0;
            double avgTurnAroundTime = 0.0;
            for (int i = 0; i < n; ++i) {
                avgWaitingTime += processes[i].waitingTime;
                avgTurnAroundTime += processes[i].turnAroundTime;
            }
            avgWaitingTime /= n;
            avgTurnAroundTime /= n;
            
            gchar *waiting_time_text = g_strdup_printf("Average Waiting Time (TAM): %.2f", avgWaitingTime);
            GtkWidget *waitingTimeLabel = gtk_label_new(waiting_time_text);
            
            gchar *turn_around_time_text = g_strdup_printf("Average Turn Around Time (TRM): %.2f", avgTurnAroundTime);
            GtkWidget *turnAroundTimeLabel = gtk_label_new(turn_around_time_text);




            gtk_widget_set_halign(turnAroundTimeLabel, GTK_ALIGN_START);
            gtk_widget_set_halign(waitingTimeLabel, GTK_ALIGN_START);
            gtk_box_pack_start(GTK_BOX(resultBox), turnAroundTimeLabel, FALSE, FALSE, 0);
          
            gtk_box_pack_start(GTK_BOX(resultBox), waitingTimeLabel, FALSE, FALSE, 0);

            
            
            gtk_container_add(GTK_CONTAINER(scrolledWindow), resultBox);

            gtk_container_add(GTK_CONTAINER(resultsWindow), scrolledWindow);

            gtk_widget_show_all(resultsWindow);
    
  }


void on_response(GtkWidget *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
      
    gtk_widget_destroy(dialog);
    }

}
void on_scheduling_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entries[3];
    entries[0] = g_object_get_data(G_OBJECT(widget), "fil_chooser");
    entries[1] = g_object_get_data(G_OBJECT(widget), "algorithm_combo");
    entries[2] = g_object_get_data(G_OBJECT(widget), "results_box");

    if (!GTK_IS_COMBO_BOX(entries[1])) {
        g_warning("Algorithm combo box is not valid.");
        return;
    }

    GtkComboBox *combo_box = GTK_COMBO_BOX(entries[1]);
    GtkTreeIter iter;
    gboolean valid = gtk_combo_box_get_active_iter(combo_box, &iter);

    if (valid) {
        gchar *selected_item;
        gtk_tree_model_get(gtk_combo_box_get_model(combo_box), &iter, 0, &selected_item, -1);

        if (selected_item != NULL) {
            if (selected_filename != NULL) {
                const gchar *filename = selected_filename;

                FILE *file = fopen(filename, "r");
                if (file == NULL) {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }

                char line[256];
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);

                struct Process processes[100];
                size_t numProcesses = 0;

                while (fgets(line, sizeof(line), file) != NULL && numProcesses < sizeof(processes) / sizeof(processes[0])) {
                    int result = sscanf(line, " %99[^|]| %d | %d | %d",
                                        processes[numProcesses].nom, &processes[numProcesses].dateArrivee,
                                        &processes[numProcesses].dureeExecution, &processes[numProcesses].priorite);

                    if (result == 4) {
                        numProcesses++;
                    } else {
                        fprintf(stderr, "Error parsing line: %s", line);
                    }
                }

                fclose(file);

                const gchar *selected_item = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[1]));

                  struct Process* results; 
	    if (selected_item != NULL) {
	       if (strcmp(selected_item, "FIFO") == 0) {
		results = fifoScheduling(processes, numProcesses);
		print_results(results,selected_item,processes, numProcesses);

	       }
	       else if(strcmp(selected_item, "SRT") == 0){
	      
	       results= srtfScheduling(processes, numProcesses) ;
	       print_results(results, selected_item,processes, numProcesses);

	       }else if(strcmp(selected_item, "PRIORITY") == 0){
	      
	       results= priorityScheduling(processes, numProcesses) ;
	       print_results(results, selected_item,processes, numProcesses);

	       }
	       else if(strcmp(selected_item, "ROUND-ROBIN") == 0 || strcmp(selected_item, "MULTILEVEL") == 0){
	           int quantum;
		     // Create a dialog
		    GtkWidget *dialogQuantum = gtk_dialog_new_with_buttons("Enter Quantum :",
				                                    GTK_WINDOW(data),
				                                    GTK_DIALOG_MODAL,
				                                    GTK_STOCK_SAVE,
				                                    GTK_RESPONSE_ACCEPT,
				                                    NULL);
				                                    
  		
                   GtkWidget *spin_button = gtk_spin_button_new_with_range(1, 100, 1);  // Set the range and step
                   gtk_entry_set_placeholder_text(GTK_ENTRY(spin_button), "Quantum");


		    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialogQuantum))), spin_button, TRUE, TRUE, 0);
		    gtk_widget_show_all(dialogQuantum);

		    gint result = gtk_dialog_run(GTK_DIALOG(dialogQuantum));
		    if (result == GTK_RESPONSE_ACCEPT) {
		  
			gdouble user_input = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
    			quantum = (int)user_input;
		        gtk_widget_destroy(dialogQuantum);
		        if(strcmp(selected_item, "ROUND-ROBIN") == 0 ){
		       	   results= roundRobinScheduling(processes, numProcesses,quantum) ;
		       	   
	               	   print_results(results, selected_item,processes, numProcesses);
	               }
	               else{
	                   results= multilevelScheduling(processes, numProcesses,quantum) ;
	               	   print_results(results, selected_item,processes, numProcesses);
	               }
		    }else {
                     gtk_widget_destroy(dialogQuantum);
                   }

	      }
	    } else {
	       GtkWidget *dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"No algorithm has been selected");
	       gtk_dialog_run(GTK_DIALOG(dialog));
	       gtk_widget_destroy(dialog);
	    }
	 g_free((gpointer)selected_item);
            } else {
                GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Unlock scheduling magic ✨ peek at your file! Scheduling secrets revealed!");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
            }
        } else {
            g_warning("No algorithm selected.");
        }
    } else {
        g_warning("No algorithm selected.");
    }
}


void on_made_button_clicked(GtkButton *button, gpointer user_data) {
    const gchar *url = "https://github.com/WerghiMedArbi/Process-Scheduling-Playground-Exploring-Scheduling-Algorithms-in-C";
    GError *error = NULL;

    if (!gtk_show_uri_on_window(NULL, url, GDK_CURRENT_TIME, &error)) {
        g_warning("Failed to open URL: %s", error->message);
        g_error_free(error);
    }
}


