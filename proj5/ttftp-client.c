/*
** name: ttftp-client.c
**
** author:Aaron Valdes
** created: 04/09/2021
** last modified:04/09/2021
**
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>
#include<unistd.h>
#include "ttftp.h"

#define h_addr h_addr_list[0]

int ttftp_client( char * to_host, int to_port, char * file )
{
	int block_count; 
	int sockfd;
	struct sockaddr_in their_addr;
	struct hostent *he;
	int numbytes;
	unsigned short stid;
	int filenamelen;
	struct TftpReq * rrq;
    he=gethostbyname(to_host);
	if (he==NULL)
    {
		perror("Failed to get host by name") ;
		exit(EXIT_FAILURE) ;
	}
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1 )
    {
		perror("Failed to create socket") ;
		exit(EXIT_FAILURE) ;
	}

	their_addr.sin_family = AF_INET ;
	their_addr.sin_port = htons((short)to_port) ;
	their_addr.sin_addr = *((struct in_addr *)he->h_addr) ;
	memset(&(their_addr.sin_zero), '\0', 8 ) ;

	filenamelen = strlen(file);
	rrq = malloc(sizeof(struct TftpReq) + filenamelen + 7) ;
	*((short *)(rrq->opcode)) = htons(TFTP_RRQ) ;
	strcpy(rrq->filename_and_mode, file) ;
	rrq->filename_and_mode[filenamelen] = 0 ;
	strcpy(&(rrq->filename_and_mode[filenamelen+1]), "octet") ;
	rrq->filename_and_mode[filenamelen + 6] = 0 ;
    numbytes=sendto(sockfd, rrq, filenamelen + 9,0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr)); 
	if (numbytes== -1 )
    {
		perror("Failed to send RRQ");
        close(sockfd);
		exit(EXIT_FAILURE) ;
	}
	block_count = 1 ;
    while (block_count )
    {
		struct sockaddr_in my_addr ;
		unsigned int addr_len ;
		struct TftpAck ack ;
		struct TftpData * data = malloc(TFTP_DATALEN+4) ;
		addr_len = sizeof(struct sockaddr_in) ;
		getsockname( sockfd, (struct sockaddr *)&my_addr, &addr_len ) ;
		addr_len = sizeof(struct sockaddr_in) ;
        numbytes=recvfrom(sockfd, data, TFTP_DATALEN+4,0,(struct sockaddr *)&their_addr, &addr_len); 
		if (numbytes== -1 )
        {
			perror("Failed to receive block") ;
            close(sockfd);
			exit(EXIT_FAILURE) ;
		}

		if (block_count == 1)
        {
            stid = ntohs(their_addr.sin_port) ;
        }
		while (ntohs(their_addr.sin_port) != stid)
        {
            struct TftpError * err = malloc(25) ;
            *((short *)(err->opcode)) = htons(TFTP_ERR) ;
            *((short *)(err->error_code)) = htons(5) ;
            strcpy(err->error_msg, "Unknown transfer ID") ;
            numbytes=sendto( sockfd, err, 25, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (numbytes ==-1)
            {
                perror("Failed to send Unknown Transfer ID") ;
                close(sockfd);
                exit(EXIT_FAILURE) ;
            }
            free(err);
            numbytes=recvfrom(sockfd, data, TFTP_DATALEN+4,0,(struct sockaddr *)&their_addr, &addr_len); 
            if (numbytes== -1 )
            {
                perror("Failed to receive confirmation") ;
                close(sockfd);
                exit(EXIT_FAILURE) ;
            }
        }	
		write(1, data->data, numbytes - 4) ;	
		*((short *)(ack.opcode)) = htons(TFTP_ACK) ;
		*((short *)(ack.block_num)) = htons(block_count) ;
		if (numbytes != TFTP_DATALEN + 4)
		{
			block_count = -1 ;
		}
        numbytes=sendto(sockfd, &ack, sizeof(ack),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		if (numbytes== -1 )
        {
            perror("Failed to send acknowledgement") ;
            exit(EXIT_FAILURE) ;
        }
		block_count ++ ;
		free(data) ;
	}
	close(sockfd) ;
	free(rrq) ;
	return 0 ;
}

