CC     = gcc
CFLAGS = --std=gnu89 -O3 -Wno-format -Wno-unused-result

all: extract

extract: extract.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) extract
