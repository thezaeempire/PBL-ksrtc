#include "dashboard.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    SystemState *state;
    GtkWidget *process_count;
    GtkWidget *resource_count;
    GtkWidget *available;
    GtkWidget *status;
    GtkWidget *output;
} DashboardData;

static void show_text(DashboardData *d, const char *text)
{
    GtkTextBuffer *buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(d->output));

    gtk_text_buffer_set_text(buffer, text, -1);
}

static void refresh(DashboardData *d)
{
    char text[512] = "";
    char item[64];
    char number[32];

    state_recalculate_available(d->state);

    snprintf(number, sizeof(number), "%d",
             d->state->process_count);
    gtk_label_set_text(GTK_LABEL(d->process_count), number);

    snprintf(number, sizeof(number), "%d",
             d->state->resource_count);
    gtk_label_set_text(GTK_LABEL(d->resource_count), number);

    for (int j = 0; j < d->state->resource_count; j++) {
        snprintf(item, sizeof(item), "%s: %d   ",
                 d->state->resource_names[j],
                 d->state->available[j]);

        strncat(text, item, sizeof(text) - strlen(text) - 1);
    }

    gtk_label_set_text(GTK_LABEL(d->available), text);

    gtk_label_set_text(
        GTK_LABEL(d->status),
        state_validate(d->state) ? "Input valid" : "Invalid input");
}

static void show_matrices(GtkButton *button, gpointer data)
{
    DashboardData *d = data;
    char text[8192] = "";
    char line[256];
    int need[MAX_PROCESSES][MAX_RESOURCES];

    state_get_need(d->state, need);

    strncat(text, "ALLOCATION MATRIX\n",
            sizeof(text) - strlen(text) - 1);

    for (int i = 0; i < d->state->process_count; i++) {
        snprintf(line, sizeof(line), "%s: ",
                 d->state->process_names[i]);

        for (int j = 0; j < d->state->resource_count; j++) {
            char value[16];
            snprintf(value, sizeof(value), "%d ",
                     d->state->allocation[i][j]);
            strncat(line, value, sizeof(line) - strlen(line) - 1);
        }

        strncat(text, line, sizeof(text) - strlen(text) - 1);
        strncat(text, "\n", sizeof(text) - strlen(text) - 1);
    }

    strncat(text, "\nMAXIMUM MATRIX\n",
            sizeof(text) - strlen(text) - 1);

    for (int i = 0; i < d->state->process_count; i++) {
        snprintf(line, sizeof(line), "%s: ",
                 d->state->process_names[i]);

        for (int j = 0; j < d->state->resource_count; j++) {
            char value[16];
            snprintf(value, sizeof(value), "%d ",
                     d->state->maximum[i][j]);
            strncat(line, value, sizeof(line) - strlen(line) - 1);
        }

        strncat(text, line, sizeof(text) - strlen(text) - 1);
        strncat(text, "\n", sizeof(text) - strlen(text) - 1);
    }

    strncat(text, "\nNEED MATRIX\n",
            sizeof(text) - strlen(text) - 1);

    for (int i = 0; i < d->state->process_count; i++) {
        snprintf(line, sizeof(line), "%s: ",
                 d->state->process_names[i]);

        for (int j = 0; j < d->state->resource_count; j++) {
            char value[16];
            snprintf(value, sizeof(value), "%d ", need[i][j]);
            strncat(line, value, sizeof(line) - strlen(line) - 1);
        }

        strncat(text, line, sizeof(text) - strlen(text) - 1);
        strncat(text, "\n", sizeof(text) - strlen(text) - 1);
    }

    show_text(d, text);
}

static void run_bankers(GtkButton *button, gpointer data)
{
    DashboardData *d = data;

    if (!state_validate(d->state)) {
        show_text(d, "Invalid system data.");
        return;
    }

    show_text(d,
        "Banker's Algorithm\n\n"
        "Connect the implementation from Member 2 here.");
}

static void detect_deadlock(GtkButton *button, gpointer data)
{
    DashboardData *d = data;

    if (!state_validate(d->state)) {
        show_text(d, "Invalid system data.");
        return;
    }

    show_text(d,
        "Deadlock Detection\n\n"
        "Connect the implementation from Member 4 here.");
}

static void recover_deadlock(GtkButton *button, gpointer data)
{
    DashboardData *d = data;

    show_text(d,
        "Deadlock Recovery\n\n"
        "Connect the recovery implementation from Member 4 here.");
}

static void refresh_clicked(GtkButton *button, gpointer data)
{
    DashboardData *d = data;

    refresh(d);
    show_text(d, "Dashboard refreshed.");
}

static GtkWidget *make_card(const char *title, GtkWidget **value)
{
    GtkWidget *frame = gtk_frame_new(title);
    GtkWidget *label = gtk_label_new("-");

    gtk_widget_set_margin_start(label, 12);
    gtk_widget_set_margin_end(label, 12);
    gtk_widget_set_margin_top(label, 18);
    gtk_widget_set_margin_bottom(label, 18);

    gtk_container_add(GTK_CONTAINER(frame), label);
    *value = label;

    return frame;
}

static GtkWidget *make_button(
    const char *label,
    GCallback callback,
    DashboardData *d)
{
    GtkWidget *button = gtk_button_new_with_label(label);

    gtk_widget_set_size_request(button, -1, 42);
    g_signal_connect(button, "clicked", callback, d);

    return button;
}

void dashboard_show(
    GtkApplication *application,
    SystemState *state)
{
    DashboardData *d = g_new0(DashboardData, 1);
    d->state = state;

    GtkWidget *window =
        gtk_application_window_new(application);

    gtk_window_set_title(
        GTK_WINDOW(window), "Deadlock Simulation System");

    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 720);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), root);

    /* Sidebar */
    GtkWidget *sidebar =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_widget_set_size_request(sidebar, 220, -1);
    gtk_widget_set_margin_start(sidebar, 16);
    gtk_widget_set_margin_end(sidebar, 16);
    gtk_widget_set_margin_top(sidebar, 25);

    gtk_box_pack_start(GTK_BOX(root), sidebar, FALSE, FALSE, 0);

    GtkWidget *brand =
        gtk_label_new("DEADLOCK\nSIMULATOR");

    gtk_label_set_justify(GTK_LABEL(brand), GTK_JUSTIFY_LEFT);
    gtk_widget_set_halign(brand, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(sidebar), brand, FALSE, FALSE, 15);

    gtk_box_pack_start(GTK_BOX(sidebar),
        make_button("Dashboard", G_CALLBACK(refresh_clicked), d),
        FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sidebar),
        make_button("Display Matrices", G_CALLBACK(show_matrices), d),
        FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sidebar),
        make_button("Banker's Algorithm", G_CALLBACK(run_bankers), d),
        FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sidebar),
        make_button("Detect Deadlock", G_CALLBACK(detect_deadlock), d),
        FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sidebar),
        make_button("Recovery", G_CALLBACK(recover_deadlock), d),
        FALSE, FALSE, 0);

    /* Main area */
    GtkWidget *content =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);

    gtk_widget_set_margin_start(content, 22);
    gtk_widget_set_margin_end(content, 22);
    gtk_widget_set_margin_top(content, 25);
    gtk_widget_set_margin_bottom(content, 20);

    gtk_box_pack_start(GTK_BOX(root), content, TRUE, TRUE, 0);

    GtkWidget *heading = gtk_label_new("System Overview");
    gtk_widget_set_halign(heading, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(content), heading, FALSE, FALSE, 0);

    GtkWidget *subtitle =
        gtk_label_new("Operating Systems | Resource Management");

    gtk_widget_set_halign(subtitle, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(content), subtitle, FALSE, FALSE, 0);

    /* Statistics */
    GtkWidget *cards =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

    gtk_box_pack_start(GTK_BOX(content), cards, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cards),
        make_card("Processes", &d->process_count),
        TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(cards),
        make_card("Resource Types", &d->resource_count),
        TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(cards),
        make_card("Input Status", &d->status),
        TRUE, TRUE, 0);

    /* Available resources */
    GtkWidget *available_frame =
        gtk_frame_new("Available Resources");

    d->available = gtk_label_new("-");

    gtk_widget_set_margin_start(d->available, 12);
    gtk_widget_set_margin_top(d->available, 14);
    gtk_widget_set_margin_bottom(d->available, 14);
    gtk_widget_set_halign(d->available, GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(available_frame), d->available);

    gtk_box_pack_start(GTK_BOX(content),
        available_frame, FALSE, FALSE, 0);

    /* Actions */
    GtkWidget *actions =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    gtk_box_pack_start(GTK_BOX(content), actions, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(actions),
        make_button("Matrices", G_CALLBACK(show_matrices), d),
        TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(actions),
        make_button("Run Banker", G_CALLBACK(run_bankers), d),
        TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(actions),
        make_button("Detect", G_CALLBACK(detect_deadlock), d),
        TRUE, TRUE, 0);

    /* Results */
    GtkWidget *results_frame = gtk_frame_new("Activity and Results");
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    d->output = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(d->output), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(d->output), GTK_WRAP_WORD);

    gtk_container_add(GTK_CONTAINER(scroll), d->output);
    gtk_container_add(GTK_CONTAINER(results_frame), scroll);

    gtk_box_pack_start(GTK_BOX(content),
        results_frame, TRUE, TRUE, 0);

    /* Dark theme */
    GtkCssProvider *css = gtk_css_provider_new();

    const char *style =
        "window { background: #101827; }"
        "label { color: #f3f4f6; font-size: 14px; }"
        "frame { border: 1px solid #344256; }"
        "button { background: #243449; color: white; padding: 8px; }"
        "button:hover { background: #2563eb; }"
        "textview, textview text { background: #111827; color: #e5e7eb; }";

    gtk_css_provider_load_from_data(css, style, -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(css);

    refresh(d);

    show_text(d,
        "Welcome to the Deadlock Simulation System.\n\n"
        "Select Display Matrices to inspect the system.\n"
        "Algorithm buttons need integration with teammates' modules.");

    gtk_widget_show_all(window);
}
