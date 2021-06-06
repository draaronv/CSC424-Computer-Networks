/*
** name: ttftp-server.c
**
** author: Aaron Valdes
** created: 04/09/2021
** last modified:04/09/2021
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
#include "ttftp.h"
#include <strings.h>
#include <sys/stat.h>
int  ttftp_server( int listen_port, int is_noloop )
{

	int sockfd_1;
	int sockfd_2 ;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int block_count ;
	unsigned int addr_len, numbytes;
	int datalen ;
	int data_size ;
	int ack_size ;
	int bytes_sent ;
	struct TftpReq * rrq = malloc(263) ;
	int rrqf_len ;
	char * octet = OCTET_STRING ;
	int f_read ;
	int readbytes ;
	int i ;
	char * data = malloc(512) ;
	struct stat st ;
	unsigned short oPort ;
	int lop ;

	sockfd_1=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd_1==-1)
    {
		perror("Failed to create Socket") ;
		exit(EXIT_FAILURE) ;
	}

	my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons((short)listen_port) ;
    my_addr.sin_addr.s_addr = INADDR_ANY ;
    memset(&(my_addr.sin_zero),'\0',8) ;
    if(bind(sockfd_1, (struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("Failed to Bind");
        close(sockfd_1);
        exit(EXIT_FAILURE) ;
    }
	do {
		addr_len = sizeof(struct sockaddr) ;
        numbytes=recvfrom(sockfd_1, rrq, 263,0,(struct sockaddr *)&their_addr, &addr_len); 
		if (numbytes== -1 ) 
        {
		    perror("Failed to Receive") ;
            close(sockfd_1);
			exit(EXIT_FAILURE) ;
		}

		oPort = ntohs(their_addr.sin_port) ;
        sockfd_2=socket(AF_INET,SOCK_DGRAM,0);
	    if (sockfd_2==-1)
        {
            perror("Failed to create sending socket");
            close(sockfd_1);
            exit(EXIT_FAILURE) ;
        }

		if (rrq->opcode[1]!= 1)
        {
			struct TftpError * err = malloc(27) ;
            *((short *)(err->opcode)) = htons(TFTP_ERR) ;
            *((short *)(err->error_code)) = htons(0) ;
            strcpy(err->error_msg, "Only RRQ is supported.") ;
            bytes_sent=sendto( sockfd_2, err, 27, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (bytes_sent ==-1 )
            {
                perror("Failed to send support RRQ") ;
                close(sockfd_1);
                close(sockfd_2);
                exit(EXIT_FAILURE) ;
            }

			close(sockfd_2) ;
			free(err) ;
            continue ;
		}

		rrqf_len = strlen(rrq->filename_and_mode) ;

		if(rrqf_len > 256)
        {
			struct TftpError * err = malloc(47) ;
            *((short *)(err->opcode)) = htons(TFTP_ERR) ;
            *((short *)(err->error_code)) = htons(0) ;
            strcpy(err->error_msg, "Maximum filename length is 256 characters.") ;
            bytes_sent=sendto( sockfd_2, err, 47, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (bytes_sent ==-1 )
            {
                perror("Failed to send Maximum Filename length");
                close(sockfd_1);
                close(sockfd_2);
                exit(EXIT_FAILURE) ;
            }
            close(sockfd_2) ;
            free(err) ;
            continue ;
		}
		for( i = 0; i < rrqf_len; i = i + 1 )
        {
			if ( rrq->filename_and_mode[i] == '/' )
            {
				struct TftpError * err = malloc(22);
                *((short *)(err->opcode)) = htons(TFTP_ERR) ;
                *((short *)(err->error_code)) = htons(2) ;
                strcpy(err->error_msg, "Access violation.") ;
                bytes_sent=sendto(sockfd_2, err, 22, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr)); 
                if (bytes_sent ==-1)
                {
                    perror("Failed to send Access Violation");
                    close(sockfd_1);
                    close(sockfd_2);
                    exit(EXIT_FAILURE) ;
                }
                close(sockfd_2) ;
                free(err) ;
				lop = 1;
                break ;
			}
		}

		if (lop)
        {
			lop = 0;
			continue ;
		}
		f_read = open(rrq->filename_and_mode, O_RDONLY);
		readbytes = read(f_read, data, 0) ;
        //Errors
        //File not found
		if (readbytes == -1)
        {
			struct TftpError * err = malloc(20) ;
            *((short *)(err->opcode)) = htons(TFTP_ERR) ;
            *((short *)(err->error_code)) = htons(1) ;
            strcpy(err->error_msg, "File not found") ;
            bytes_sent=sendto( sockfd_2, err, 20, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (bytes_sent ==-1 )
            {
                    perror("Failed to send File not Found") ;
                    close(sockfd_1);
                    close(sockfd_2);
                    exit(EXIT_FAILURE) ;
            }
            close(sockfd_2) ;
            free(err) ;
            continue ;
		}
        //Octet error
		if (strcasecmp(rrq->filename_and_mode + rrqf_len + 1, OCTET_STRING) != 0) 
        {
            struct TftpError * err = malloc(34) ;
			*((short *)(err->opcode)) = htons(TFTP_ERR) ;
			*((short *)(err->error_code)) = htons(0) ;
			strcpy(err->error_msg, "Only octet data is supported.") ;
            bytes_sent=sendto( sockfd_2, err, 34, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
			if (bytes_sent ==-1 )
            {
                perror("Failed to send only octet") ;
                close(sockfd_1);
                close(sockfd_2);
                exit(EXIT_FAILURE) ;
            }
			close(sockfd_2) ;
            free(err) ;
            continue ;
		}
		stat(rrq->filename_and_mode, &st ) ;

        //Access violation

        if (!((st.st_mode & S_IRUSR) && (st.st_mode & S_IRGRP) && (st.st_mode & S_IROTH)))
        {
            struct TftpError * err = malloc(22) ;
            *((short *)(err->opcode)) = htons(TFTP_ERR) ;
            *((short *)(err->error_code)) = htons(2) ;
            strcpy(err->error_msg, "Access violation.") ;
            bytes_sent=sendto( sockfd_2, err, 22, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (bytes_sent ==-1 ) 
            {
                perror("Failed to send Access violation") ;
                close(sockfd_1);
                close(sockfd_2);
                exit(EXIT_FAILURE) ;
            }
            close(sockfd_2) ;
            free(err) ;
            continue ;
		}
		block_count = 1 ;
        //Sending blocks
		while (block_count)
        {
			readbytes = read(f_read, data, TFTP_DATALEN) ;
			data_size = sizeof (struct TftpData) + readbytes ;
			struct TftpData * datapack = malloc(data_size) ;
			*((short *)(datapack->opcode)) = htons(TFTP_DATA) ;
			*((short *)(datapack->block_num)) = htons(block_count) ;
			memcpy(datapack->data, data, readbytes) ;
            bytes_sent= sendto( sockfd_2, datapack, data_size, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
			if (bytes_sent ==-1 )
            {
				perror("Failed to send block") ;
                close(sockfd_1);
                close(sockfd_2);
				exit(EXIT_FAILURE) ;
			}
			ack_size = sizeof(struct TftpAck) ;
			struct TftpAck * ack = malloc(ack_size) ;
			addr_len = sizeof(struct sockaddr) ;
            numbytes=recvfrom(sockfd_2, ack, ack_size,0,(struct sockaddr *)&their_addr, &addr_len); 
            if (numbytes== -1 )
            {
                perror("Failed to received acknowledgement") ;
                close(sockfd_1);
                close(sockfd_2);
                exit(EXIT_FAILURE) ;
            }
            //We can't remove this
			if (g_verbose )
            {
				printf("ntohs(their_addr.sin_port): %d \n ntohs(their_addr.sin_port) != oPort: %d\n", 
                        ntohs(their_addr.sin_port), ntohs(their_addr.sin_port)) ;
			}

			while (ntohs(their_addr.sin_port) != oPort)
            {
				struct TftpError * err = malloc(25) ;
                *((short *)(err->opcode)) = htons(TFTP_ERR) ;
                *((short *)(err->error_code)) = htons(5) ;
                strcpy(err->error_msg, "Unknown transfer ID.") ;
                bytes_sent=sendto( sockfd_2, err, 25, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
                if (bytes_sent ==-1 )
                {
                    perror("Failed to send Unknown Transfer ID") ;
                    close(sockfd_1);
                    close(sockfd_2);
                    exit(EXIT_FAILURE) ;
                }
				free(err) ;
                numbytes=recvfrom(sockfd_2, ack, ack_size,0,(struct sockaddr *)&their_addr, &addr_len); 
				if (numbytes== -1 )
                {
                    perror("Failed to receive error acknowledgement") ;
                    close(sockfd_1);
                    close(sockfd_2);
                    exit(EXIT_FAILURE) ;
                }
			}
			block_count++ ;
			if (readbytes != 512)
            {
				block_count = 0 ;
			}
			free(datapack) ;
			free(ack) ;
		}
		close(sockfd_2) ;

	} while (!is_noloop) ;
	close(sockfd_1) ;
	free(rrq) ;
	free(data) ;

	return 0 ;
}

