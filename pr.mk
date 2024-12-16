atm.exe: client.o server.o
	gcc client.o server.o -o atm.exe

client.o: client.c server.h
	gcc -c client.c

server.o: server.c server.h
	gcc -c server.c