all: server

clean:
	@rm -rf *.o
	@rm -rf server

server: main.o httpd.o
	$(CC) -o server $^ -I $(FORKFUZZ_DIR)/libhfnetdriver/libhfnetdriver.a -lpthread

main.o: main.c httpd.h
	$(CC) -c -o main.o main.c 

httpd.o: httpd.c httpd.h
	$(CC) -c -o httpd.o httpd.c

