CC = gcc
CFLAGS = -std=c99 -Wall -fsanitize=address,undefined

SRCS = mymalloc.c memgrind.c
EXE = memgrind

all: $(EXE)

$(EXE): $(SRCS:.c=.o)
	$(CC) -o $(EXE) $(SRCS:.c=.o) $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

permissions:
	chmod +x $(EXE)

clean:
	rm -f $(EXE) $(SRCS:.c=.o)

.PHONY: all clean permissions
