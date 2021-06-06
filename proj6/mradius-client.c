/*
** name: mradius-client
**
** author: 
** created: 30 mar 2017
** last modified:
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
#include "mradius.h"
#include <openssl/md5.h>
#include <sys/stat.h>
struct AccReq
{
    char code ;
    char identifier;
    char length[2] ;
	char authen[AUTHEN_LEN] ;
	char type ;
	char alen ;
	char attrs[0] ;
} ;

struct AccRes
{
	char code ;
	char identifier ;
	char length[2] ;
	char authen[AUTHEN_LEN] ;
};

int mradius_client( struct Params * params ) {
struct hostent *he ;
	char randNo[16] ;
	char * dwp ;
	int sockfd ;
	struct sockaddr_in their_addr ;
	int i ;
	int numbytes ;
    int unamelen;
    int arqsize;
    he=gethostbyname(params->host);
	if(he==NULL)
    {
        perror("Failed to get host by name") ;
        exit(EXIT_FAILURE) ;
    }
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1 )
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE) ;
    }
    //Configure addres
    their_addr.sin_family = AF_INET ;
    their_addr.sin_port = htons((short)params->port) ;
    their_addr.sin_addr = *((struct in_addr *)he->h_addr) ;
    memset(&(their_addr.sin_zero), '\0', 8 ) ;
	dwp = passwd_md5(params->upass) ;

	if (params->no_randomness)
    {
		for(i=0; i<16; ++i)
        {
            randNo[i] = i+1 ;
        }
	}
    else
    {
	    for(i=0; i<16; ++i)
        {
			randNo[i] = next_random() ;
        }
	}
	size_t skeylen = strlen(params->shared_key) ;
	special_encrypt(dwp, params->shared_key, skeylen, randNo, 16) ;
	unamelen = strlen(params->uname) ;
	arqsize = unamelen + 40 ;
	struct AccReq * accessRequest = malloc(arqsize) ;
	accessRequest->code = RFC2865_ACC_REQ ;
	accessRequest->identifier = 1 ;
	*((short *)(accessRequest->length)) = htons(arqsize) ;
	memcpy(accessRequest->authen, randNo, 16) ;
    accessRequest->type = RFC2865_ATT_U_NAME ;
	accessRequest->alen = unamelen + 2;
	strcpy(accessRequest->attrs, params->uname) ;
	accessRequest->attrs[unamelen] = RFC2865_ATT_U_PASS ;
	accessRequest->attrs[unamelen+1] = RFC2865_ATT_U_REPL ;
	memcpy(&(accessRequest->attrs[unamelen+2]), dwp, 16) ;
    numbytes=sendto(sockfd, accessRequest, arqsize,0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
    if (numbytes== -1 )
    {
        perror("Failed to send to server") ;
        exit(EXIT_FAILURE) ;
    }
	struct sockaddr_in my_addr ;
    unsigned int addr_len ;
    struct AccRes accessReceiv ;
    addr_len = sizeof(struct sockaddr_in) ;
    getsockname( sockfd, (struct sockaddr *)&my_addr, &addr_len ) ;
    if (g_verbose)
    {
        printf("Sent from port %d \n", ntohs(my_addr.sin_port)) ;
    }
    addr_len = sizeof(struct sockaddr_in);
    numbytes=recvfrom(sockfd, &accessReceiv, 20,0,(struct sockaddr *)&their_addr, &addr_len); 
    if (numbytes== -1)
    {
        perror("Failed to receive from") ;
        exit(EXIT_FAILURE) ;
	}
    char stream[20+skeylen] ;
    memcpy(stream, &accessReceiv, 4) ;
    memcpy(&(stream[4]), accessRequest->authen, 16) ;
    memcpy(&(stream[20]), params->shared_key, skeylen) ;
    char * md ;
    md = (char *) MD5((unsigned char *) stream, 20+skeylen, NULL ) ;
	if ( !memcmp(accessReceiv.authen, md, 16) )
    {
		if (accessReceiv.code == RFC2865_ACC_ACC)
        {
			printf("YES\n") ;
		} 
        else if (accessReceiv.code == RFC2865_ACC_REJ)
        {
			printf("NO\n") ;
		}
        else
        {
			fprintf(stderr, "Unsupported type\n") ;
	        exit(EXIT_FAILURE) ;
		}
    }
    else
    {
		printf("Beware the imposter\n") ;
	}
	free(accessRequest) ;
return 0;
}
