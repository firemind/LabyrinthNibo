target=proj
CC=gcc 
LDFLAGS = -Wl,--hash-style=both
LIBS = -lcunit
main: main.c
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@ && ./main
