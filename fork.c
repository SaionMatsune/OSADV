/* example of fork */
#include<stdio.h>

main()
{
  
  int pid;

  pid = fork();

  if(pid==0) printf("child\n");
  else if(pid>0) printf("parent\n");
  else printf("Fork returned error code \n");

}



/*
fork : create process
#inclue<sys/types.h>
#inclue<unistd.h>

pid_t fork(void);
 */
