CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-L./ -lmyaes
EXEC=

all: libmyaes.so my_aes128


libmyaes.so: library.c
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS)

my_aes128: main.o libmyaes.so
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -rf *.o
