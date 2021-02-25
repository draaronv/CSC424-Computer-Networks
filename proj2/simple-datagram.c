#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#include "simple-data.h"

#define DEFAULT_PORT 3333


/*
 * simple-datagram.c
 * name:Aaron Valdes
 * date:02/16/2021
 *
 * template file for csc424 the simple-datagram  project
 */

#define USAGE_MESSAGE "usage: simple-datagram [-v] [-p port] [host]"

int g_verbose = 0 ;

int main(int argc, char * argv[]) {

	int ch ;
	int port = DEFAULT_PORT ;
	char * host ;
	int is_client = 0 ;

			
	while ((ch = getopt(argc, argv, "vp:l")) != -1) {
		switch(ch) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'v':
				g_verbose += 1 ;
				break ;
			default:
				printf("%s\n", USAGE_MESSAGE) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	if ( argc > 1 ) {
		fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
		exit(0) ;
	}
	assert(port>0) ; 

	if (argc==1) {
		is_client = 1 ;
		host = strdup(argv[0]) ;
	}
	
	if (g_verbose) {
		// sample of using g_verbose
		if (is_client) {
			printf( "%s#%d: client to send to %s:%d \n", __FILE__,__LINE__, host, port ) ;
		}
		else {
			printf( "%s#%d: server to list on %d \n", __FILE__,__LINE__, port ) ;
		}
	}

	if ( is_client)
		simple_data_c(host,port) ;
	else
		simple_data_s(port) ;

	return 0 ;
}

	
