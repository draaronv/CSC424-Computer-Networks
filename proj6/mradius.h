/*
** name: mradius.h
**
** author: bjr
** created: 30 mar 2017
** last modified: 19 mar 2019
**
*/

#define USAGE_MESSAGE "usage: mradius [-vLR -k key -p port -D debugflags] ( -h host user pwd | pwd-file )" 
#define DEFAULT_PORT  1812
#define DEFAULT_SHARED_KEY "pa55word0" 
#define DEBUGFLAG_NOCRYPTO  01
#define DEBUGFLAG_NORANDOM  02
#define MAXBUFLEN 1024

extern int g_verbose ;
extern int g_debug ;

struct Params {
	char * host ;
	char * pwfile ;
	int port ;
	char * shared_key ;
	int no_randomness ;
	int no_loop ;
	char * upass ;
	char * uname ;
	int is_hashchain ;
	int hashchain_len ;
	/* add more parameters here, if needed */
} ;

typedef struct {
        char * first;
        char * rest ;
} StringPair ;

typedef struct Node {
	char * user ;
	char * pass ;
	struct Node * next ;
} Node ;

typedef struct Pair {
	char * dat ;
	int len ;
} Pair ;

#define AUTHEN_LEN 16

#define RFC2865_ACC_REQ 1
#define RFC2865_ACC_ACC 2
#define RFC2865_ACC_REJ 3
#define RFC2865_ACC_CHA 11
#define RFC2865_ATT_U_NAME 1
#define RFC2865_ATT_U_PASS 2
#define RFC2865_ATT_U_REPL 18

StringPair pa_parse(char * str, int sep) ; // Is in the utils.c

Node * new_node( char * user, char * pass , Node * next ) ; 
Node * find_node( Node * root, char * user ) ;
Node * parse_pwfile( char * filename ) ;
void print_nodes( Node * n ) ;

char next_random(void) ; // Is in the utils.c

void special_encrypt( char * buf, char * secret, int secret_len, char * ra, int ra_len ) ;
char * passwd_md5(char * pwd) ;

int mradius_client( struct Params * params ) ;
int mradius_server( struct Params * params ) ;

