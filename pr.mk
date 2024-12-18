ATM.exe: client.o server.o admin.o
	gcc client.o server.o admin.o -o ATM.exe

client.o: client.c server.h
	gcc -c client.c

server.o: server.c server.h
	gcc -c server.c

admin.o: admin.c server.h
	gcc -c admin.c