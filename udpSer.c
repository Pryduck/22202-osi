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
	struct sockaddr_in serverAddr, clientAddr; 
	socklen_t clientAddrLen;
	ssize_t receivedSize;

	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr)); 
	memset(&clientAddr, 0, sizeof(clientAddr));

	serverAddr.sin_family = AF_INET; // IPv4 
	serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons(PORT); 

	if (bind(socketFD, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { 
		printf("bind() error\n");
		close(socketFD);
		return 1;
	}

	while (1) {
		clientAddrLen = sizeof(clientAddr);
		receivedSize = recvfrom(socketFD, (char*)buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (receivedSize == -1) {
			printf("recvfrom() error\n");
			close(socketFD);
			return 1;
		}
		
		buffer[receivedSize] = '\0';
		printf("client : %s\n", buffer);
		if (sendto(socketFD, (const char*)buffer, receivedSize, 0, (const struct sockaddr*)&clientAddr, clientAddrLen) < 0) {
			printf("sendto() error\n");
			strerror(errno);
			close(socketFD);
			return 1;
		}
	}

	return 0;
}
