#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<math.h>

#include<stdint.h>

/*
 * quiz2 for csc424 semester 212
 * author: Aaron Valdes
 * last_update: 02/22/2021
 * see RFC 790 https://tools.ietf.org/html/rfc790
 */
 
#define CLASS_A 'A'
#define CLASS_B 'B'
#define CLASS_C 'C'
#define CLASS_D 'D'
#define CLASS_E 'E'
#define A_MASK ((uint32_t)0xff000000)
#define B_MASK ((uint32_t)0xffff0000)
#define C_MASK ((uint32_t)0xffffff00)
#define MASK_EQUAL(M,A,B) (0==((M)&((A)^(B))))
#define USAGE "usage: class-ip _ip_address_ _ip_address_"

char the_class(uint32_t ip_addr)
{
uint32_t check=((ip_addr >>24)& 0xff);
if(check <= 127)
{
    return CLASS_A;
}
else if(check<=191)
{
    return CLASS_B;
}
else if(check<=223)
{
    return CLASS_C;
}
else if(check<=239)
{
    return CLASS_D;
}
else
{
    return CLASS_E;
}	
}

uint32_t ip_to_uint(char * ip_dd)
{
int val1;
int val2;
int val3;
int val4;
uint32_t d;
sscanf(ip_dd, "%d.%d.%d.%d", &val4, &val3, &val2, &val1);
d=(val1)+(val2*pow(2,8))+(val3*pow(2,16))+(val4*pow(2,24));
return d ;
}

int is_same_net(uint32_t addr1, uint32_t addr2)
{
    char address1;
    char address2;
    address1=the_class(addr1);
    address2=the_class(addr2);
    if(address1==address2)
    {
        if(address1=='A')
        {
            if(MASK_EQUAL(A_MASK,addr1,addr2))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if(address1 =='B')
        {
        if(MASK_EQUAL(B_MASK,addr1,addr2))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if((MASK_EQUAL(C_MASK,addr1,addr2)))
            {
            return 1;
            }
            else
            {
            return 0;
            }
        }
    }
    else
    {
        return 0;
    }
}

int main(int argc, char * argv[])
{

	char * ipdd_1, * ipdd_2 ;
	uint32_t ipv4_1, addr2, ipv4_2 ;
	char ipcl_1, ipcl_2 ;
	char *f = "dotted: %s\thex: 0x%08x\tclass: %c\n" ;
	
	if (argc!=3)
    {
        printf("%s\n", USAGE );
		return 0 ;
	}
	
	ipdd_1 = strdup(argv[1]) ;
	ipdd_2 = strdup(argv[2]) ;
	
	ipv4_1 = ip_to_uint(ipdd_1) ;
	ipv4_2 = ip_to_uint(ipdd_2) ;
	
	ipcl_1 = the_class(ipv4_1) ;
	ipcl_2 = the_class(ipv4_2) ;
	
	printf(f, ipdd_1, ipv4_1, ipcl_1) ;
	printf(f, ipdd_2, ipv4_2, ipcl_2) ;
	
	if (is_same_net(ipv4_1, ipv4_2))
    {
		printf("same network\n") ;
	}
	else
    {
		printf("different networks\n") ;
	}

	return 0 ;
}
