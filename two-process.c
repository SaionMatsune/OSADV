#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSIZE 16

char *msg1 = "hello, world#1";
char *msg2 = "hello, world#2";
char *msg3 = "hello, world#3";

int main( int argc, char* argv[] ) {
	
	char inbuf[MSGSIZE];
	int p[2], c[2], j;
	pid_t pid;

	pipe( p );
	pipe( c );
	pid = fork();

	//if( pid == 0 ) {
	//	pipe( c );
	//} else {
	//	pipe( p );
	//}

	switch( pid ) {
		case -1:
			perror( "fork call" );
			exit( 1 );
		
		case 0: 
			write( p[1], msg1, MSGSIZE );
			write( p[1], msg2, MSGSIZE );
			write( p[1], msg3, MSGSIZE );
			for( j = 0; j < 3; j++ ) {
				read( c[0], inbuf, MSGSIZE );
				printf( "0 %s\n", inbuf );
			}
			break;
		
		default:
			write( c[1], msg1, MSGSIZE );
			write( c[1], msg2, MSGSIZE );
			write( c[1], msg3, MSGSIZE );
			for( j = 0; j < 3; j++ ) {
				read( p[0], inbuf, MSGSIZE );
				printf( "1 %s\n", inbuf );
			}
			break;
	}

	exit( 0 );
}
