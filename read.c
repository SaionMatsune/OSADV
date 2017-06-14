/* example of read : count the number of character in a file */
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFSIZE 512


main()
{

  char buffer[BUFSIZE];
  int filedes;


  ssize_t nread;
  long total=0;

  if((filedes=open("sample_file",O_RDONLY))==-1)
    {

      printf("ERROR\n");
      exit(1);
    }
  while((nread=read(filedes, buffer, BUFSIZE))>0)
    total += nread;


  printf("total char in sample_file:%ld\n", total);
  exit(0);
}



/*
read : copy character or byte from file to buffer
#include<unistd.h> or <sys/stat.h>

ssize_t read(int filedes, void *buffer, size_t n);
 */
