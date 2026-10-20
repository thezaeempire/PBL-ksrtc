#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10
#define NAME_LENGTH 32

typedef struct {
    int process_count;
    int resource_count;

    char process_names[MAX_PROCESSES][NAME_LENGTH];
    char resource_names[MAX_RESOURCES][NAME_LENGTH];

    int total[MAX_RESOURCES];
    int available[MAX_RESOURCES];

    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];
    int request[MAX_PROCESSES][MAX_RESOURCES];

    int terminated[MAX_PROCESSES];
} SystemState;

void state_init(SystemState *state);
void state_recalculate_available(SystemState *state);
int state_get_need(
    const SystemState *state,
    int process_id,
    int resource_id
);
int state_validate(const SystemState *state);

#endif
