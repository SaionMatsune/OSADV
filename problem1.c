#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#define MSGSIZE 16

char *msg = "Change";

main(){
  char inbuf[MSGSIZE];
  int p[2], c[2], j;
  pid_t pid;

  pipe(p);
  pipe(c);
  pid = fork();


    switch(pid) {
      case -1:
        perror("fork call");
        exit(1);
      case 0:
      while(1) {
        read(p[0], inbuf, MSGSIZE);
        for(j=0;j<10;j++) {
          printf("a");
        }
        printf("\n");
        write(c[1], msg, MSGSIZE);
      }
        break;
      default:
      write(c[1], msg, MSGSIZE);
      while(1) {
        read(c[0], inbuf, MSGSIZE);
        for(j=0;j<10;j++) {
          printf("b");
        }
        printf("\n");
        write(p[1], msg, MSGSIZE);
      }
        break;
    }

    exit(0);
}




/*
Synchronization
 create 2 Process using fork()
 Each process : Print one character infinitely
 Synchronize two process using pipe()
  Each process print 10 times one character alternately
 */
