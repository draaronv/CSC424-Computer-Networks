/*
** name: passaround.c
**
** author:Aaron Valdes
** date:03/04/2020
** last modified:
**		15 feb 2021 bjr: added comment line
**
** from template created 31 jan 2015 by bjr
**
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
#include "passaround.h"
#define LOCALHOST "localhost"
#define MAXMSGLEN 2048
#define N_REPEAT_DEFAULT 1

#define USAGE_MESSAGE "usage: passaround [-v] [-n num] [-m message] port"

int g_verbose = 0 ;


int main(int argc, char * argv[]) {
	int ch ;
	int the_port = 0 ;
	int n_repeat = N_REPEAT_DEFAULT ;
	char * msg = NULL ;
	int is_forever = 0 ;
	
	assert(sizeof(short)==2) ; 
	
	while ((ch = getopt(argc, argv, "vm:n:")) != -1) {
		switch(ch) {
		case 'n':
			n_repeat = atoi(optarg) ;
			break ;
		case 'v':
			g_verbose = 1 ;
			break ;
		case 'm':
			msg = strdup(optarg) ;
			break ;
		case '?':
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc!= 1 ) {
		printf("%s\n",USAGE_MESSAGE) ;
		exit(0) ;
	}

	the_port = atoi(*argv) ;
	assert(the_port) ;

	is_forever = (n_repeat == 0);

    //Initializing
    char buffer[MAXMSGLEN];
    char* rest_message;
    char* server_address;
    int sockfd;
    struct sockaddr_in myaddr={0};
    struct sockaddr_in theiraddr;
    int sending;
    int receiving;
    int binding;
    unsigned int addr_len;
    char *client_address;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1)
    {
        perror("Failed to create the Socket");
        exit(EXIT_FAILURE);
    }
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons((short)the_port);
    myaddr.sin_addr.s_addr=INADDR_ANY;
    memset(&(myaddr.sin_zero),'\0',8);
    binding=bind(sockfd,(const struct sockaddr*)&myaddr,sizeof(struct sockaddr));
    if(binding==-1)
    {
        perror("Failed to bind socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (msg)
    {
        // Parse
        if(strchr(msg,':')==NULL)
        {
            server_address=strtok(msg,"");
            rest_message="";
            assert(server_address);
        }
        else
        {
            server_address=strtok(msg,":");
            rest_message=strtok(NULL," ");
            strcpy(buffer,rest_message);
            assert(server_address);
        }
        // Send
        //Configuring addresses
        theiraddr.sin_family=AF_INET;
        theiraddr.sin_port=htons((short)the_port);
        if(strcmp(server_address,"localhost")==0)
        {
            server_address="127.0.0.1";
        }
        inet_pton(AF_INET,server_address,&(theiraddr.sin_addr));
        memset(&(theiraddr.sin_zero),'\0',8);
        //Sending Package
        sending=sendto(sockfd,(const char*)buffer,strlen(buffer),0,(const struct sockaddr*)&theiraddr,sizeof(theiraddr));
        if(sending==-1)
        {
            perror("Failed to send package");
            exit(EXIT_FAILURE);
            close(sockfd);
        }
        // and print S: host:port |message|
        printf("S: %s:%i |%s|\n",inet_ntoa(theiraddr.sin_addr),ntohs(theiraddr.sin_port),rest_message);
        free(msg);
		n_repeat-- ; // a packet sent
	}
	while(is_forever || n_repeat )
    {
        memset(buffer,'\0',MAXMSGLEN);
        //Receiving sockets
        addr_len=sizeof(struct sockaddr);
        receiving=recvfrom(sockfd,(char*)buffer,MAXMSGLEN,0,(struct sockaddr*)&theiraddr,&addr_len);
        if(receiving==-1)
        {
            perror("Failed to Received Package");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        client_address=inet_ntoa(theiraddr.sin_addr);
		// print R: host:port |message|
        printf("R: %s:%i |%s|\n",client_address,ntohs(theiraddr.sin_port),buffer);
        //Parse
        if(strcmp(buffer,"")!=0)
        {
        if(strchr(buffer,':')==NULL)
        {
            server_address=strtok(buffer,"");
            rest_message="";
            inet_pton(AF_INET,server_address,&(theiraddr.sin_addr));
            sending=sendto(sockfd,(const char*)rest_message,strlen(rest_message),0,(const struct sockaddr*)&theiraddr,sizeof(theiraddr));
            if(sending==-1)
            {
                perror("Failed to send package");
                exit(EXIT_FAILURE);
                close(sockfd);
            } 
            printf("S: %s:%i |%s|\n",inet_ntoa(theiraddr.sin_addr),ntohs(theiraddr.sin_port),rest_message);
            break;
        }
        else
        {
            server_address=strtok(buffer,":");
            rest_message=strtok(NULL," ");
            inet_pton(AF_INET,server_address,&(theiraddr.sin_addr));
            strcpy(buffer,rest_message);
            sending=sendto(sockfd,(const char*)buffer,strlen(buffer),0,(const struct sockaddr*)&theiraddr,sizeof(theiraddr));
            if(sending==-1)
            {
                perror("Failed to send package");
                exit(EXIT_FAILURE);
                close(sockfd);
            }
            printf("S: %s:%i |%s|\n",inet_ntoa(theiraddr.sin_addr),ntohs(theiraddr.sin_port),buffer);
        }
        }
        else
        {
            break;
        }
		n_repeat-- ;
	}
    close(sockfd);
	return 0 ;
}

