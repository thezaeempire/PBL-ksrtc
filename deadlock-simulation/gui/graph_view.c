#include "graph_view.h"
#include <cairo.h>
#include <math.h>
#include <string.h>

static void draw_node(cairo_t *cr, double x, double y,
                      const char *text, int resource)
{
    if (resource)
        cairo_set_source_rgb(cr, 0.10, 0.48, 0.30);
    else
        cairo_set_source_rgb(cr, 0.12, 0.30, 0.65);

    cairo_rectangle(cr, x - 38, y - 20, 76, 40);
    cairo_fill_preserve(cr);

    cairo_set_source_rgb(cr, 0.85, 0.90, 1.0);
    cairo_set_line_width(cr, 1.5);
    cairo_stroke(cr);

    cairo_select_font_face(cr, "Sans",
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 13);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, text, &extents);

    cairo_move_to(cr,
        x - extents.width / 2,
        y + extents.height / 2);

    cairo_show_text(cr, text);
}

static void draw_arrow(cairo_t *cr, double x1, double y1,
                       double x2, double y2)
{
    double angle = atan2(y2 - y1, x2 - x1);
    double head = 9;

    cairo_set_source_rgb(cr, 0.95, 0.72, 0.25);
    cairo_set_line_width(cr, 2);

    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);

    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr,
        x2 - head * cos(angle - 0.45),
        y2 - head * sin(angle - 0.45));

    cairo_line_to(cr,
        x2 - head * cos(angle + 0.45),
        y2 - head * sin(angle + 0.45));

    cairo_close_path(cr);
    cairo_fill(cr);
}

static gboolean draw_graph(GtkWidget *widget,
                           cairo_t *cr, gpointer data)
{
    SystemState *state = data;

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    double width = allocation.width;
    double height = allocation.height;

    cairo_set_source_rgb(cr, 0.06, 0.09, 0.15);
    cairo_paint(cr);

    double process_x = width * 0.25;
    double resource_x = width * 0.75;

    double process_step = height /
        (state->process_count + 1.0);

    double resource_step = height /
        (state->resource_count + 1.0);

    /* Allocation edges: resource -> process */
    for (int i = 0; i < state->process_count; i++) {
        double py = process_step * (i + 1);

        for (int j = 0; j < state->resource_count; j++) {
            if (state->allocation[i][j] > 0) {
                double ry = resource_step * (j + 1);

                draw_arrow(cr,
                    resource_x - 40, ry,
                    process_x + 40, py);
            }
        }
    }

    /* Request edges can be added when request data is available. */

    for (int i = 0; i < state->process_count; i++) {
        double py = process_step * (i + 1);

        draw_node(cr, process_x, py,
                  state->process_names[i], 0);
    }

    for (int j = 0; j < state->resource_count; j++) {
        double ry = resource_step * (j + 1);

        draw_node(cr, resource_x, ry,
                  state->resource_names[j], 1);
    }

    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
    cairo_select_font_face(cr, "Sans",
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);

    cairo_move_to(cr, process_x - 35, 22);
    cairo_show_text(cr, "Processes");

    cairo_move_to(cr, resource_x - 35, 22);
    cairo_show_text(cr, "Resources");

    return FALSE;
}

GtkWidget *graph_view_create(SystemState *state)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget *title = gtk_label_new("Resource Allocation Graph");

    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 5);

    GtkWidget *area = gtk_drawing_area_new();

    gtk_widget_set_size_request(area, 600, 420);

    g_signal_connect(area, "draw",
        G_CALLBACK(draw_graph), state);

    gtk_box_pack_start(GTK_BOX(box), area, TRUE, TRUE, 0);

    GtkWidget *legend = gtk_label_new(
        "Blue: Processes    Green: Resources    Yellow: Allocation edges");

    gtk_box_pack_start(GTK_BOX(box), legend, FALSE, FALSE, 5);

    return box;
}
