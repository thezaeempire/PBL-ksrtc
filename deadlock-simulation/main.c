#include <gtk/gtk.h>
#include "system_state.h"
#include "gui/dashboard.h"

static SystemState state;

static void activate(GtkApplication *application, gpointer data)
{
    dashboard_show(application, &state);
}

int main(int argc, char **argv)
{
    state_init(&state);

    GtkApplication *application = gtk_application_new(
        "org.college.deadlocksimulator",
        G_APPLICATION_FLAGS_NONE);

    g_signal_connect(application, "activate",
                     G_CALLBACK(activate), NULL);

    int status = g_application_run(
        G_APPLICATION(application), argc, argv);

    g_object_unref(application);
    return status;
}