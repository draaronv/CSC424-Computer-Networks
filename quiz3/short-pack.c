#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>


/*
 * short-pack.c
 * quize for csc424-212
 * orginal: bjr
 * last-update:Aaron Valdes
 *   16 march 2021
 *
 */


void print_buf(char * name, unsigned char * buf, int size) {
	int i ;
	printf("%s: ",name) ;
	for (i=0;i<size;i++) {
		printf("%02x ",buf[i]) ;
	}
	printf("\n") ;
	return ;
}


int main(int argc, char * argv[]) {

	unsigned short i, j ;
	unsigned char buf[4] ;
	assert(sizeof(short)==2) ;
	assert(argc==3) ; 

	i = atoi(argv[1]) ;
	j = atoi(argv[2]) ;

	print_buf("i",(unsigned char*)&i,sizeof(short)) ;	
	print_buf("j",(unsigned char*)&j,sizeof(short)) ;	
	
	// fill in the buffer so that it is correctly shows i then j in big endian
	{
		// student code here!
        unsigned char * temp;
        //Conv
        temp =(unsigned char *) &i;
        buf[0]=*(temp+1);
        buf[1]=*(temp+0);
        temp=(unsigned char *) &j;
        buf[2]=*(temp+1);
        buf[3]=*(temp+0);
	}

	print_buf("buf",buf,sizeof(buf)) ;	

	return 0 ;
}

