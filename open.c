/* example of open */
#include<stdlib.h> /* for exit() */
#include<fcntl.h>


char *workfile="junk";


main()
{

  int filedes;


  if((filedes=open(workfile, O_RDWR))==-1)
    {

      printf("cannot open %s \n", workfile);
      exit(1);
    }
  close(filedes);
  exit(0);
}

/*
open : open file or create file
#include<fcntl.h>

int open(const char *pathname, int flags, [moe_tmode]};
  flag : defined in <fcntl.h>
    O_RDONLY : for read
    O_WRONLY : for write
    O_RDWR   : for read/write

  Mode_t mode
    For security
 */
