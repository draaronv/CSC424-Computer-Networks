/*
 * simple-data.h
 * name:Aaron Valdes
 * date:02/16/2021
 *
 * template file for csc424 the simple-datagram project
 */
 
#define MAXBUFLEN 256

extern int g_verbose ;

int simple_data_c(char * host, int port) ;
int simple_data_s(int port) ;

