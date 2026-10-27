#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#define MAX_REFERENCES 1000
#define MAX_FRAMES 20
#define EMPTY_FRAME -999999

typedef struct {
    int page;
    int frames[MAX_FRAMES];
    int status;  // 1 = Hit, 0 = Fault
} Step;

typedef struct {
    Step steps[MAX_REFERENCES];
    int n;
    int capacity;
    int hits;
    int faults;
} Result;

// Algorithms implemented by your teammates
Result simulate_fifo(const int refs[], int n, int capacity);
Result simulate_lru(const int refs[], int n, int capacity);
Result simulate_optimal(const int refs[], int n, int capacity);

#endif
