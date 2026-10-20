
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -I.
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

TARGET = deadlock_simulator

CORE = \
	core/system_state.c \
	core/resource_manager.c

ALGORITHMS = \
	algorithms/bankers.c \
	algorithms/detection.c \
	algorithms/recovery.c

GUI = \
	gui/dashboard.c \
	gui/matrix_view.c \
	gui/graph_view.c

SOURCES = main.c $(CORE) $(ALGORITHMS) $(GUI)

TEST_BINS = \
	test_system_state \
	test_resource_manager \
	test_bankers \
	test_detection \
	test_recovery

.PHONY: all run test clean clean_tests

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(SOURCES) -o $(TARGET) $(GTK_LIBS)

run: $(TARGET)
	./$(TARGET)

test_system_state: tests/test_system_state core/system_state.c
	$(CC) $(CFLAGS) -x c $^ -o $@

test_resource_manager: tests/test_resource_manager $(CORE) $(ALGORITHMS)
	$(CC) $(CFLAGS) -x c $^ -o $@

test_bankers: tests/test_bankers.c core/system_state.c algorithms/bankers.c
	$(CC) $(CFLAGS) -x c $^ -o $@

test_detection: tests/test_detection.c core/system_state.c algorithms/detection.c
	$(CC) $(CFLAGS) -x c $^ -o $@

test_recovery: tests/test_recovery.c core/system_state.c algorithms/recovery.c
	$(CC) $(CFLAGS) -x c $^ -o $@

test: $(TEST_BINS)
	./test_system_state
	./test_resource_manager
	./test_bankers
	./test_detection
	./test_recovery

clean_tests:
	rm -f $(TEST_BINS)

clean: clean_tests
	rm -f $(TARGET)
