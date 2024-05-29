#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <errno.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
	int socketFD;
	char buffer[BUFFER_SIZE];
	char message[BUFFER_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLength;
	ssize_t lengthRecieved;

	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT); //узловой порядок расположения байтов -> сетевой
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	serverAddrLength = sizeof(serverAddr);

	while (1) {
		fgets(message, BUFFER_SIZE, stdin);

		if (sendto(socketFD, (const char*)message, strlen(message), 0, (struct sockaddr*)&serverAddr, serverAddrLength) < 0) {
			printf("recvfrom() error");
			printf("%s\n", strerror(errno));
		}

		lengthRecieved = recvfrom(socketFD, (char*)buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLength);
		if (lengthRecieved < 0) {
			printf("recvfrom() error\n");
			close(socketFD);
			return 1;
		}
		buffer[lengthRecieved] = '\0';
		printf("received : %s\n", buffer);

	}

	close(socketFD);
	return 0;
}
