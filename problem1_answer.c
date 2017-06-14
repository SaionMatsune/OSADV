/* example of synchronization with fork and pipe */
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

#define msg "*"
#define MSGSIZE 3

main()
{
  int pid, i, j;
  int p_chil[2], p_par[2];
  char buf[3];

  if( pipe(p_chil) < 0 || pipe(p_par) < 0) {
    perror("pipe call");
    exit(1);
  }
  if((pid=fork()) < 0) {
    perror("fork call");
    exit(2);
  }

  if(pid>0) {
    write(p_chil[1], msg, MSGSIZE);
    for(i=0;i<100000;i++){
      read(p_par[0], buf, MSGSIZE);
      for(j=0;j<10;j++) printf("a");
      printf("\n");
      write(p_chil[1], msg, MSGSIZE);
    }
  }
  else {
    for(i=0;i<100000;i++){
      read(p_chil[0], buf, MSGSIZE);
      for(j=0;j<10;j++) printf("b");
      printf("\n");
      write(p_par[1], msg, MSGSIZE);
    }
  }
}
