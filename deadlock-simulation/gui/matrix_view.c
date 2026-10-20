#include "matrix_view.h"

GtkWidget *matrix_view_create(SystemState *state)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    GtkWidget *title = gtk_label_new("Resource Matrices");

    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 5);

    int need[MAX_PROCESSES][MAX_RESOURCES];
    state_get_need(state, need);

    const char *matrix_names[] = {
        "Allocation Matrix",
        "Maximum Matrix",
        "Need Matrix"
    };

    int (*matrices[])[MAX_RESOURCES] = {
        state->allocation,
        state->maximum,
        need
    };

    for (int k = 0; k < 3; k++) {
        GtkWidget *heading = gtk_label_new(matrix_names[k]);
        gtk_widget_set_halign(heading, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(box), heading, FALSE, FALSE, 2);

        GtkWidget *grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 12);

        gtk_grid_attach(GTK_GRID(grid),
            gtk_label_new("Process"), 0, 0, 1, 1);

        for (int j = 0; j < state->resource_count; j++) {
            gtk_grid_attach(GTK_GRID(grid),
                gtk_label_new(state->resource_names[j]),
                j + 1, 0, 1, 1);
        }

        for (int i = 0; i < state->process_count; i++) {
            gtk_grid_attach(GTK_GRID(grid),
                gtk_label_new(state->process_names[i]),
                0, i + 1, 1, 1);

            for (int j = 0; j < state->resource_count; j++) {
                char value[16];

                g_snprintf(value, sizeof(value), "%d",
                           matrices[k][i][j]);

                gtk_grid_attach(GTK_GRID(grid),
                    gtk_label_new(value),
                    j + 1, i + 1, 1, 1);
            }
        }

        gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 5);
    }

    state_recalculate_available(state);

    GtkWidget *available_title =
        gtk_label_new("Available Resources");

    gtk_widget_set_halign(available_title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), available_title, FALSE, FALSE, 2);

    GtkWidget *available_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(available_grid), 12);

    for (int j = 0; j < state->resource_count; j++) {
        char value[16];

        g_snprintf(value, sizeof(value), "%d", state->available[j]);

        gtk_grid_attach(GTK_GRID(available_grid),
            gtk_label_new(state->resource_names[j]),
            j, 0, 1, 1);

        gtk_grid_attach(GTK_GRID(available_grid),
            gtk_label_new(value),
            j, 1, 1, 1);
    }

    gtk_box_pack_start(GTK_BOX(box), available_grid, FALSE, FALSE, 5);

    return box;
}
