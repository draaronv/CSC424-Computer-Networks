/*
** netbounce-client.c
** from Beej's Guide to Network Programming: Using Unix Sockets, 
** by Brian "Beej" Hall.
**
** modified by Burt Rosenberg, 
** Created: Feb 8, 2009
** Last modified: Jan 31, 2015 - minor changes -bjr
**    Jan 12, 2015 - added getopt
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>

#define MAXBUFLEN 100

#define USAGE_MESSAGE "usage: %s [-v] -p port -h host message\n" 
#define PROG_NAME "netbounce-client" 

int main( int argc, char * argv[] ) {

	int sockfd ;
	struct sockaddr_in their_addr ;
	struct hostent *he ;
	int numbytes ;
	int port = 0 ;
	char * host = NULL ;
	char * msg = NULL ;
	int ch ;
	int is_verbose = 0 ;
	
	while ((ch = getopt(argc, argv, "vp:h:")) != -1) {
		switch(ch) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'h':
				host = strdup(optarg) ;
				break ;
			case 'v':
				is_verbose = 1 ;
				break ;
			default:
				printf(USAGE_MESSAGE, PROG_NAME) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	if ( !host || !port || ! argc ) {
		printf(USAGE_MESSAGE, PROG_NAME) ;
		return 0 ;
	}
	msg = strdup(*argv) ;

	/* example of an assertion */
	assert(host) ;
	assert(port) ;

	if ((he=gethostbyname(host))==NULL) {
		perror("gethostbyname") ;
		exit(1) ;
	}
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
		perror("socket") ;
		exit(1) ;
	}
	their_addr.sin_family = AF_INET ;
	their_addr.sin_port = htons((short)port) ;
	their_addr.sin_addr = *((struct in_addr *)he->h_addr) ;
	memset(&(their_addr.sin_zero), '\0', 8 ) ;
	
	if ((numbytes=sendto(sockfd, msg, strlen(msg),0,
			(struct sockaddr *)&their_addr, sizeof(struct sockaddr)) ) == -1 ) {
		perror("sendto") ;
		exit(1) ;
	}
	if (is_verbose) {
		printf("send %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr)) ;
	}
	
	{
		// if you must introduce new variables, make a block
		
		struct sockaddr_in my_addr ;
		unsigned int addr_len ;
		char buf[MAXBUFLEN];
		
		addr_len = sizeof(struct sockaddr_in) ;
		getsockname( sockfd, (struct sockaddr *)&my_addr, &addr_len ) ;
		if ( is_verbose ) {
			printf("sent from port %d\n", ntohs(my_addr.sin_port)) ;
			printf("calling for return packet\n") ;
		}

		addr_len = sizeof(struct sockaddr_in) ;
		if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN-1,0,
				(struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
			perror("recvfrom") ;
			exit(1) ;
		}
		
		if ( is_verbose ) {
			printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr), 
				ntohs(their_addr.sin_port)) ;
			printf("packet is %d bytes long\n", numbytes ) ;
		}

		buf[numbytes] = '\0' ;
		printf("%s\n", buf ) ;
	}
	
	close(sockfd) ;
	return 0 ;
}

