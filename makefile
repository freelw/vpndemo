all:
	gcc client.c tun_alloc.c bridge.c -o client
	gcc server.c tun_alloc.c bridge.c -o server
clean:
	rm client server