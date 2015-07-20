LDFLAGS=-lmpsse

all: mcp9808

mcp9808:
	rm -f mcp9808
	$(CC) $(CFLAGS) main.c -o mcp9808 $(LDFLAGS)
	$(CC) $(CFLAGS) mpu6050.c -o mpu6050 $(LDFLAGS)

clean:
	rm -f *.dSYM
	rm -f mcp9808
	rm -f mpu6050

distclean: clean