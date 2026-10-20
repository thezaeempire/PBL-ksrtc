#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <gtk/gtk.h>
#include "../system_state.h"

GtkWidget *graph_view_create(SystemState *state);

#endif
