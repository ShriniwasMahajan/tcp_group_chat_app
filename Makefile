CLIENT = socket_client.exe
SERVER = socket_server.exe

all:
	gcc -c socket_util.c -o socket_util.o
	gcc socket_client.c socket_util.o -o $(CLIENT)
	gcc socket_server.c socket_util.o -o $(SERVER)

clean:
	rm -f socket_util.o ${CLIENT} $(SERVER)