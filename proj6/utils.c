/*
** name: utils
**
** author: 
** created: 30 mar 2017
** last modified: 19 mar 2019
**
*/

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<arpa/inet.h>
#include<openssl/md5.h>

#include "mradius.h"


char next_random()
{
	static FILE * fr = NULL ;
	static int r = 0 ;
	if ( g_debug & DEBUGFLAG_NORANDOM  ) {
		return ++r ;
	}

	if ( ! fr ) {
		if ( g_verbose )  printf("%s:%d: opening dev-urandom\n", __FILE__, __LINE__ ) ;
		fr = fopen("/dev/urandom","r") ;
	}
	return getc(fr) ;
}

StringPair pa_parse(char * str, int sep)
{
        StringPair sp ;
        char * p ;
        sp.first = str ;
        sp.rest = NULL ;
        p = strchr(str,sep) ;
        if (p) {
                *p = '\0' ;
                sp.rest = p+1 ;
        }
        return sp ;
}

Node * new_node( char * user, char * pass, Node * next ) {
	Node * n = (Node *) malloc(sizeof(Node)) ;
	n->user = strdup(user) ;
	n->pass = strdup(pass) ;
	n->next = next ;
	return n ;
}

void print_nodes( Node * n ) {
	int i ;
	while (n) {
		printf("(%s,%s)->", n->user, n->pass ) ;
		n = n->next ;
	}
	printf("NULL") ;
}

void print_hex( char * b, int n ) {
	int i ;
	for (i=0;i<n;i++) printf("%02hhx", b[i]) ;
}

Node * find_node( Node * root, char * user ) {
	// find user
	while ( root ) {
		if ( !strcmp(root->user,user) ) return root ;
		root = root->next ;
	}
	return NULL ;
}

#define SEP ": \n\t"
#define COMMENT_CHAR '#' 

Node * parse_pwfile( char * filename ) {
	Node * n = NULL ;
	FILE * f ; 
	char s[1024] ;
	char * u ;
	char * p ;

	if (! (f = fopen(filename, "r" )) ) {
		return NULL ;
	}
	
	while ( fgets( s, sizeof(s), f) ) {
	
		u = strtok(s,SEP) ;
		if ( !u ) continue ;
		if ( *u == COMMENT_CHAR ) continue ;
		p = strtok(NULL,SEP) ;
		if ( !p ) continue ;
		n = new_node( u, p, n ) ;
		if (g_verbose) {
			printf("%s:%d: adding (|%s|,|%s|) to linked list\n",__FILE__, __LINE__, u,p) ;
		}
	}
	
    fclose(f) ; 
    return n ; 
}

char * passwd_md5(char * pwd)
{
	int i ;
	static char md[MD5_DIGEST_LENGTH] ;
	MD5( (unsigned char *) pwd, strlen(pwd), md ) ;
	if ( g_verbose ) {
		printf("%s:%d: pwd=%s, md5=", __FILE__, __LINE__, pwd ) ;
		for (i=0; i< MD5_DIGEST_LENGTH; i++ )  {
			printf("%02hhx", md[i]) ;
			if ( (i%4)==3 ) printf(" ") ;
		}
		printf("\n") ;
	}
	return md ;
}

void special_encrypt( char * buf, char * secret, int secret_len, char * ra, int ra_len )
{
	char * md ;
	int i ;
	char * tb = (char *) malloc( secret_len+ra_len) ;
	memcpy(tb, secret, secret_len) ;
	memcpy(tb+secret_len, ra, ra_len ) ;
	md = (char *) MD5((unsigned char *) tb, secret_len+ra_len, NULL ) ;
	free(tb) ;
	for (i=0;i<AUTHEN_LEN;i++) {
		buf[i] ^= md[i] ;
	}
	return ;
}



