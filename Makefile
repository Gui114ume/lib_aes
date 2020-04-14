CC=gcc
CFLAGS=-g -O0 -Wall
LDFLAGS1=-L./ -lmyaes
LDFLAGS2=-L./ -lmyrevaes

EXEC=

all: libmyaes.so my_aes128 my_rev_aes128

libmyaes.so: library.c
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS)

libmyrevaes.so: library2.c
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS)

my_aes128: main.o libmyaes.so
	$(CC) $^ -o $@ $(LDFLAGS1)

my_rev_aes128: main2.o libmyrevaes.so
	$(CC) $^ -o $@ $(LDFLAGS2)

main.o: main.c
	$(CC) -c $^ -o $@ $(CFLAGS)

main2.o: main2.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -rf *.o
