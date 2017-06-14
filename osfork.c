#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(void){
  int pid;        // forkを生成する変数

  pid = fork();
  while(1){
    if(pid == 0){
      printf("a");
    }
    else{
      printf("1");
    }
  }
  
  return 0;
}
