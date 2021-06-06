#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>


/*
 * sign-me-up.c
 * quize for csc424-212
 * orginal: bjr
 * last-update:Aaron Valdes
 *   16 march 2021
 *
 */


int main(int argc, char * argv[]) {
    int i ; 
	unsigned char c ;
    printf("%lu\n",sizeof(char));
    printf("%lu\n",sizeof(int));
	i = 255 ;
	c = i ;
	i = c ;
	printf("%d\n",i) ;
	return 0 ; 
}


