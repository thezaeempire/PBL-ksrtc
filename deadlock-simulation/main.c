
#include <gtk/gtk.h>
#include <stdio.h>

#include "core/system_state.h"
#include "gui/dashboard.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    SystemState state;
    state_init(&state);

    if (!state_validate(&state)) {
        fprintf(stderr, "Error: Invalid initial system state.\n");
        return 1;
    }

    /*
     * The dashboard receives the shared state so that
     * the GUI can display and operate on the simulation.
     */
    GtkWidget *window = create_dashboard(&state);

    if (window == NULL) {
        fprintf(stderr, "Error: Could not create dashboard.\n");
        return 1;
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
