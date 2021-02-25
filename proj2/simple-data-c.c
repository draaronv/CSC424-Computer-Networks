#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include "simple-data.h"

/*
 * simple-data-c.c
 * name:Aaron Valdes
 * date:02/19/2021
 *
 * template file for csc424 the simple-datagram project
 */


extern int g_verbose ;

int simple_data_c(char * host, int port)
{
    char buffer[MAXBUFLEN];
    unsigned int num_char_read;
    struct sockaddr_in servaddr={0};

    //Reading the message
    num_char_read=fread(buffer+1,sizeof(char),sizeof(buffer),stdin);
    buffer[0]=num_char_read;

    // Create socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    //Socket Errors
    if(sockfd==-1)
    {
        perror("Failed to create the socket");
        exit(EXIT_FAILURE);
    }
    // Configuring connection
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET, host, &(servaddr.sin_addr));
    memset(&(servaddr.sin_zero),'\0',8);

    // Sending Package
    int len=sendto(sockfd,(const char*)buffer,strlen(buffer)+1,0,(const struct sockaddr*)&servaddr,sizeof(servaddr));
    if(len==-1)
    {
        perror("Failed to Send");
        exit(EXIT_FAILURE);
    }

    if(g_verbose)
    {
        printf("Send %d bytes to %s\n",(int)sizeof(buffer),host);
    }
    close(sockfd);
    return 0;
}


