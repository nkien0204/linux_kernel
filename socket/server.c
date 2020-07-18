#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main (int argc, char * argv[]) {
	if (argc != 2) {
		printf ("Syntax Error\n");
		return -3;
	}

	int sockfd = -1;
	int newsockfd = -1;
	char buff[255] = "Hello from server\n";

	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf ("Could not create socket\n");
		return -1;
	}

	struct sockaddr_in serv_addr, client_addr;
	int client_len = sizeof (client_addr);
	int serv_len = sizeof (serv_addr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (atoi(argv[1]));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind (sockfd, (const struct sockaddr *)&serv_addr, (socklen_t)serv_len) != 0) {
		printf ("Could not bind name to a socket\n");
		close(sockfd);
		return -2;
	}

	if (listen (sockfd, 1) != 0) {
		printf ("Listening error\n");
		close (sockfd);
		return -4;
	}

	newsockfd =	accept (sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
	if (newsockfd < 0) {
		printf ("Accepting failed\n");
		close (sockfd);
		close (newsockfd);
		return -5;
	}

  write (newsockfd, buff, 255);
	close (newsockfd);
	close (sockfd);
	return 0;
}
