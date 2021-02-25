#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<assert.h>

#include "simple-data.h"

/*
 * simple-data-s.c
 * name:Aaron Valdes
 * date:02/16/2021
 *
 * template file for csc424 the simple-datagram project
 */


extern int g_verbose ;

int simple_data_s(int port)
{
    char buffer[MAXBUFLEN];
	struct sockaddr_in servaddr = {0};
    struct sockaddr_in clienaddr={0};
    unsigned int addr_len;
    //Created Socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("Failed to create socket");
        close(sockfd);
		exit(EXIT_FAILURE);
	}
	//Configuring socket
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons((short)port);
	servaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(servaddr.sin_zero),'\0',8);
	//Binding socket
	int rc = bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr));
	if(rc == -1)
	{
		perror("failed to bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	addr_len=sizeof(struct sockaddr);
	int n = recvfrom(sockfd, (char *)buffer, MAXBUFLEN,0,(struct sockaddr*)&clienaddr,&addr_len);
    fwrite(buffer+1,sizeof(char),buffer[0],stdout);
    if(n==-1)
    {
        perror("Failed to received package");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if(g_verbose)
    {
        printf("Received package from %s , port %d\n",inet_ntoa(clienaddr.sin_addr),ntohs(clienaddr.sin_port));
        printf("Packet is %d bytes long \n",n);
    }
	close(sockfd);
    return 0;
}


