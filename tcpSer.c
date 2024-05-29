#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(void) {
	int serverFD, clientFD;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t client_len;
	pid_t pid;

	serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFD < 0) {
		printf("socket() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	if (bind(serverFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("bind() error\n");
		return 1;
	}

	if (listen(serverFD, 5) < 0) {
		printf("listen() error\n");
		close(serverFD);
		return 1;
	}

	while (1) {
		clientFD = accept(serverFD, (struct sockaddr*)&clientAddr, &sizeof(clientAddr));
		if (clientFD < 0) {
			printf("accept() error\n");
			continue;
		}

		pid = fork();
		if (pid == 0) {
			char buffer[BUFFER_SIZE];
			ssize_t numRead;

			while ((numRead = read(clientFD, buffer, BUFFER_SIZE)) > 0) {
				printf("received: %s\n", buffer);
				write(clientFD, buffer, numRead);
			}
			close(serverFD);
			return 0;
		}
		else {
			close(clientFD);
		}
	}

	return 0;
}
