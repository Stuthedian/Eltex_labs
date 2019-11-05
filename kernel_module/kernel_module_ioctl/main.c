#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>


#define ERR(...) fprintf( stderr, "\7" __VA_ARGS__ ), exit( EXIT_FAILURE )
typedef struct _RETURN_STRING 
{   char buf[ 160 ]; } RETURN_STRING;

#define TEST_IOC_MAGIC    'h'
#define IOCTL_GET_STRING _IOR(TEST_IOC_MAGIC, 1, RETURN_STRING)

int main( int argc, char *argv[] ) {
   int dfd;                  
   if( ( dfd = open( "/dev/test", O_RDWR ) ) < 0 ) ERR( "Open device error: %m\n" );
   RETURN_STRING buf;
   if( ioctl( dfd, IOCTL_GET_STRING, &buf ) ) ERR( "IOCTL_GET_STRING error: %m\n" );
   fprintf( stdout, (char*)&buf );
   close( dfd );
   return EXIT_SUCCESS;
};


