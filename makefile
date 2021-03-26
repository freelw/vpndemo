all:
	gcc client.c tun_alloc.c -o client
	gcc server.c tun_alloc.c -o server
clean:
	rm client server