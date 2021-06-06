/*
** name: mradius-server
**
** author: 
** created: 30 mar 2017
** last modified: 19 march 2019
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

struct AccReq
{
    char code ;
    char identifier ;
    char length[2] ;
	char authen[AUTHEN_LEN] ;
	char type ;
	char alen ;
	char attrs[0] ;
};

struct AccRes
{
	char code ;
	char identifier ;
	char length[2] ;
	char authen[AUTHEN_LEN] ;
};

int mradius_server( struct Params * params ) {

	Node * ll_users ;

	if ( g_verbose ) {
		printf("%s:%d: mradius_server, (|%s,%d|)\n", __FILE__, __LINE__, params->pwfile,params->port ) ;
	}
	
	ll_users = parse_pwfile(params->pwfile ) ;
	if ( !ll_users ) {
		perror(params->pwfile) ;
		return 0 ; 
	}
	if ( g_verbose ) {
		printf("%s:%d: user list: ", __FILE__, __LINE__ ) ;
		print_nodes(ll_users) ; 
		printf("\n") ;
	}
int sockfd_r ;
int sockfd_s ;
struct sockaddr_in my_addr ;
struct AccReq * accessRequest = malloc(MAXBUFLEN) ;
unsigned int addr_len, numbytes ;
struct sockaddr_in their_addr ;
size_t ulen ;
char dwp[16];
int binding;
sockfd_r=socket(AF_INET,SOCK_DGRAM,0);
if (sockfd_r==-1)
{
    perror("Failed to create socket") ;
    exit(EXIT_FAILURE) ;
}
my_addr.sin_family = AF_INET ;
my_addr.sin_port = htons((short)params->port) ;
my_addr.sin_addr.s_addr = INADDR_ANY ;
memset(&(my_addr.sin_zero),'\0',8) ;
binding=bind(sockfd_r, (struct sockaddr *)&my_addr,sizeof(struct sockaddr));
if (binding== -1 )
{
    perror("Failed to bind socket") ;
    exit(EXIT_FAILURE) ;
}
while (!(params->no_loop))
{
    addr_len = sizeof(struct sockaddr) ;
    numbytes=recvfrom(sockfd_r, accessRequest, MAXBUFLEN,0,(struct sockaddr *)&their_addr, &addr_len); 
    if (numbytes== -1 )
    {
        perror("Failed to recv access request from client") ;
        exit(EXIT_FAILURE) ;
    }
    if ( g_verbose )
    {
        printf("Packet size: %d bytes\n",numbytes) ;
    }
    // Create socket to send acces response
    sockfd_s=socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd_s==-1)
    {
        perror("Failed to create sending socket") ;
        exit(EXIT_FAILURE) ;
    }
    if (accessRequest->type == RFC2865_ATT_U_NAME)
    {
        ulen = (size_t)(accessRequest->alen)-2 ;
        params->uname = strndup(accessRequest->attrs, ulen) ;
        memcpy(dwp, &(accessRequest->attrs[ulen+2]), 16) ;
    }
    else if (accessRequest->type == RFC2865_ATT_U_PASS)
    {
        memcpy(dwp, accessRequest->attrs, 16) ;
        params->uname = strndup(&(accessRequest->attrs[18]), (size_t)(accessRequest->attrs[17])-2) ;
    }
    else
    {
        fprintf(stderr, "Unsupported type\n") ;
        continue ;
    }
    size_t skeylen = strlen(params->shared_key) ;
    special_encrypt(dwp, params->shared_key, skeylen, accessRequest->authen, 16) ;
    Node * mnode = find_node(ll_users, params->uname) ;
    struct AccRes accessReceiv ;
    accessReceiv.identifier = accessRequest->identifier ;
    *((short *)(accessReceiv.length)) = htons(20) ;
    char stream[20+skeylen] ;
    memcpy(&(stream[4]), accessRequest->authen, 16) ;
    memcpy(&(stream[20]), params->shared_key, skeylen) ;
    char * md ;
    char * epass ;
    if (mnode)
    {
        epass = passwd_md5(mnode->pass) ;
        if ( !memcmp(dwp,epass, 16) )
        {
            accessReceiv.code = RFC2865_ACC_ACC ;
            memcpy(stream, &accessReceiv, 4) ;
            md = (char *) MD5((unsigned char *) stream, 20+skeylen, NULL ) ;
            memcpy(accessReceiv.authen, md, 16) ;
            numbytes=sendto(sockfd_s, &accessReceiv, 20,0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (numbytes== -1 )
            {
                perror("Failed to send to client");
                exit(EXIT_FAILURE) ;
            }

        }
        else 
        {
            accessReceiv.code = RFC2865_ACC_REJ ;
            memcpy(stream, &accessReceiv, 4) ;
            md = (char *) MD5((unsigned char *) stream, 20+skeylen, NULL ) ;
            memcpy(accessReceiv.authen, md, 16) ;
            numbytes=sendto(sockfd_s, &accessReceiv, 20,0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (numbytes== -1 )
            {
                perror("Failed to sendto") ;
                exit(EXIT_FAILURE) ;
            }
        }

    }
    else
    {
        accessReceiv.code = RFC2865_ACC_REJ ;
        memcpy(stream, &accessReceiv, 4) ;
        md = (char *) MD5((unsigned char *) stream, 20+skeylen, NULL ) ;
        memcpy(accessReceiv.authen, md, 16) ;
        numbytes=sendto(sockfd_s, &accessReceiv, 20,0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
        if (numbytes == -1 )
        {
            perror("Failed to sendto") ;
            exit(EXIT_FAILURE) ;
        }
    }
    free(params->uname) ;
}
free(accessRequest) ;
free(ll_users) ;
return 0 ; 
}
