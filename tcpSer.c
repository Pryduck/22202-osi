#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./socket"
#define BUFFER_SIZE 1024

int main(void) {
	int serverFD, clientFD;
	struct sockaddr_un serverAddr;
	char buffer[BUFFER_SIZE];

	serverFD = socket(AF_UNIX, SOCK_STREAM, 0);
	if (serverFD < 0) {
		printf("socket() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, SOCKET_PATH);

	unlink(SOCKET_PATH);
	if (bind(serverFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("bind() error\n");
		close(serverFD);
		return 1;
	}

	if (listen(serverFD, 5) < 0) {
		printf("listen() error\n");
		close(serverFD);
		return 1;
	}

	pid_t pid;
	do {
		clientFD = accept(serverFD, NULL, NULL);
		if (clientFD < 0) {
			printf("accept() error\n");
			close(serverFD);
			return 1;
		}
		pid = fork();
	} while (pid != 0);

	printf("connected\n");

	ssize_t numRead;
	while ((numRead = read(clientFD, buffer, BUFFER_SIZE)) > 0) {
		write(clientFD, buffer, numRead);
	}

	printf("disconnected\n");

	close(clientFD);

	close(serverFD);
	unlink(SOCKET_PATH);

	return 0;
}
