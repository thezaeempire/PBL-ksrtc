#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <gtk/gtk.h>
#include "../system_state.h"

void dashboard_show(
    GtkApplication *application,
    SystemState *state
);

#endif
