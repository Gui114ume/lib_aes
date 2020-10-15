CC=gcc

LDFLAGS1=-L./ -lmyaes
LDFLAGS2=-L./ -lmyrevaes
HEADER_DIR=./header/
SOURCE_DIR=./source/
CFLAGS=-g -O3 -Wall -I$(HEADER_DIR)


all: libmyaes.so my_aes my_rev_aes
	rm main.o main2.o

libmyaes.so: $(SOURCE_DIR)library.c 
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS)

libmyrevaes.so: $(SOURCE_DIR)library2.c 
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS)

my_aes: main.o libmyaes.so
	$(CC) $^ -o $@ $(LDFLAGS1)

my_rev_aes: main2.o libmyrevaes.so
	$(CC) $^ -o $@ $(LDFLAGS2)

main.o: $(SOURCE_DIR)main.c
	$(CC) -c $^ -o $@ $(CFLAGS)

main2.o: $(SOURCE_DIR)main2.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm libmyaes.so  libmyrevaes.so my_aes my_rev_aes
