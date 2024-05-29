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
	int serverFD;
	struct sockaddr_un serverAddr;
	char buffer[BUFFER_SIZE + 1];

	serverFD = socket(AF_UNIX, SOCK_STREAM, 0);
	if (serverFD < 0) {
		printf("serverFDet() error\n");
		return 1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, SOCKET_PATH);

	if (connect(serverFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("%s%s\n", "connect() error: ", strerror(errno));
		close(serverFD);
		return 1;
	}

	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
		if (write(serverFD, buffer, strlen(buffer)) < 0) {
			printf("%s%s\n", "write() error: ", strerror(errno));
			sleep(1);
			continue;
		}

		ssize_t numRead = read(serverFD, buffer, BUFFER_SIZE);
		if (numRead < 0) {
			printf("%s%s\n", "read() error: ", strerror(errno));
			sleep(1);
			continue;
		}

		buffer[numRead] = '\0';
		printf("response: %s", buffer);
	}

	close(serverFD);
	return 0;
}
