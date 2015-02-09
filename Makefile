LDFLAGS=-lmpsse

all: mcp9808

mcp9808:
	$(CC) $(CFLAGS) main.c -o mcp9808 $(LDFLAGS)

clean:
	rm -f *.dSYM
	rm -f mcp9808

distclean: clean