#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main (int argc, char * argv[]) {
	if (argc != 3) {
		printf ("Syntax Error\n");
		return -3;
	}

	int sockfd = -1;
	char buff[255];

	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf ("Could not create socket\n");
		return -1;
	}

	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons (atoi(argv[2]));
	sock_addr.sin_addr.s_addr = inet_addr (argv[1]);

	if (connect (sockfd, (struct sockaddr *)&sock_addr, sizeof (sock_addr)) == 0) {
		read (sockfd, &buff, 255);
		printf ("%s", buff);
		close (sockfd); 
	} else {
		printf ("Could not connect to server\n");
		return -2;
	}
	return 0;
}
