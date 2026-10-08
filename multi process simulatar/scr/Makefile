CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt -pthread

all: launcher core/core ui/ui logging/logger

launcher: scr/launcher.c
	$(CC) $(CFLAGS) scr/launcher.c -o launcher

core/core: core/core.c
	$(CC) $(CFLAGS) core/core.c -o core/core $(LDFLAGS)

ui/ui: ui/ui.c
	$(CC) $(CFLAGS) ui/ui.c -o ui/ui $(LDFLAGS)

logging/logger: logging/logger.c
	$(CC) $(CFLAGS) logging/logger.c -o logging/logger $(LDFLAGS)

clean:
	rm -f launcher core/core ui/ui logging/logger

run: all
	./launcher
