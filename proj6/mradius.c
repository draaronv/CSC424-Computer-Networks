/*
** name: mradius
**
** author: bjr
** created: 30 mar 2017
** last modified: 16 apr 2017
**
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>
#include<unistd.h>
#include<openssl/md5.h>

#include "mradius.h"

int g_verbose = 0 ;
int g_debug = 0 ;

int main(int argc, char * argv[]) {
	int ch ;
	struct Params params ;

	memset( &params, 0, sizeof(struct Params)) ; 
	params.port = DEFAULT_PORT ;
	params.shared_key  = DEFAULT_SHARED_KEY ;
	
	while ((ch = getopt(argc, argv, "vRLk:p:h:D:")) != -1) {
		switch(ch) {
		case 'v':
			g_verbose ++ ;
			break ;
		case 'D':
			g_debug |= atoi(optarg) ;
			break ;
		case 'R':
			params.no_randomness ++ ;
			g_debug |= DEBUGFLAG_NORANDOM ;
			break ;
		case 'L':
			params.no_loop = 1 ;
			break ;
		case 'h':
			params.host = strdup(optarg) ;
			break ;
		case 'p':
			params.port = atoi(optarg) ;
			break ;
		case 'k':
			params.shared_key = strdup(optarg) ;
			break ;
		case '?':
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( (argc!=1 && !params.host ) ||	(argc!=2 && params.host ) ) {
		fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
		exit(0) ;
	}

    if ( !params.host ) {
    	params.pwfile = strdup(argv[0]) ;
    	mradius_server( &params ) ;	
	}
	else {
		params.uname = strdup(argv[0]) ;
		params.upass = strdup(argv[1]) ;
		mradius_client( &params ) ;
	}
    free(params.pwfile);
	return 0 ;
}

