/* example of write : copy file */
#include<unistd.h>
#include<fcntl.h>

#define BUFSIZE 512
#define PERM 0644

main(int argc, char *argv[])
{
  int infile, outfile;
  char buffer[BUFSIZE];

  ssize_t nread;
  
  if(argc!=3)
    {
      printf("ERROR : input file name\n");
      exit(1);
    }

  if((infile=open(argv[1], O_RDONLY))==-1)
  {
    printf("cannot open file %s\n", argv[1]);
    exit(-1);
  }

  if((outfile=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, PERM))==-1)
  {
    printf("cannot create file %s\n", argv[2]);
    close(infile);
    exit(-1);
  }

  while((nread=read(infile, buffer, BUFSIZE))>0)
    {
      if(write(outfile, buffer, nread)<nread)
      {
	close(infile);
	close(outfile);
	exit(-1);
      }
    }
  close(infile);
  close(outfile);
  exit(0);
}


/*
write : copy character or byte from buffer to a file
#include<uniste.h> or <sys/stat.h>

ssize_t write(int filedes, const void *buffer, size_t n);
 */
