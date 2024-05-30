#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(void) {
	int socketFD;
	struct sockaddr_in serverAddr;
	char buffer[BUFFER_SIZE];

	socketFD = socket(AF_INET, SOCK_STREAM, 0);//TCP
	if (socketFD < 0) {
		printf("%s", "socket() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
		perror("inet_pton");
		close(socketFD);
		exit(EXIT_FAILURE);
	}

	if (connect(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("%s", "connect() error\n");
		return 1;
	}

	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
		write(socketFD, buffer, strlen(buffer));

		ssize_t numRead = read(socketFD, buffer, BUFFER_SIZE);
		if (numRead < 0) {
			printf("%s", "read() error\n");
			close(socketFD);
			return 1;
		}

		buffer[numRead] = '\0';
		printf("received: %s\n", buffer);
	}

	close(socketFD);
	return 0;
}
