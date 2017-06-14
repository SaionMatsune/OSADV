#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include<stdlib.h>

#define MSGSIZE 16

char *msg1 = "1:Thinking";  // プロセス1の状態がHungryへ
char *msg2 = "1:Hungry";
char *msg3 = "1:Eating";
char *msg4 = "2:Thinking";  // プロセス2の状態がHungryへ
char *msg5 = "2:Hungry";
char *msg6 = "2:Eating";
char *msg7 = "1:Waiting";
char *msg8 = "2:Waiting";

int processA[2], processB[2], monitor[2];  // パイプ用配列
char buf[MSGSIZE];  // メッセージを入れる配列

void moni();  // モニター処理の関数
void process(int);  // プロセス処理の関数

main()
{
	int pid, pid2;  // fork用変数
	
	srand((unsigned)time(NULL));  // 乱数のシードを変える

	if(pipe(processA) < 0) {
		perror("pipe call");
		exit(1);
	}
    
	if(pipe(processB) < 0) {
		perror("pipe call");
		exit(1);
	}
    
	if(pipe(monitor) < 0) {
		perror("pipe call");
		exit(1);
	}
  
	if((pid = fork()) < 0) {
		perror("fork call");
		exit(2);
	}

	if(pid > 0) {
		moni();  // モニター処理の関数へ
	} else {
		if((pid2 = fork()) < 0) {
			perror("fork call");
			exit(2);
		}
	
		process(pid2);	
	}
	exit(0);
}

void moni(){

	for(;;) {
		read(monitor[0], buf, MSGSIZE);
		printf("%s\n", buf);
	}

}

void process(int pid2){
	int j = 0;      // 一つのプロセスが長い間占有するのでそうしないための変数
	int x;          // プロセスの状態遷移をさせるか決めるためのランダム変数
	int state = 0;  // 自分の状態を把握するための変数。0:Thinking、1:Hungry、2:Eating

    
    if(pid2 > 0) {  // プロセス1の処理開始
       write(monitor[1], msg1, MSGSIZE);  // 初期値の状態であるThinkingをモニターへ
       read(processA[0], buf, MSGSIZE);   // プロセス2の初期値も表示させたいため、プロセス2にCPUを譲る
       for(;;) {
			x = rand() % 2; // 状態遷移をランダムで決める。0ならそのまま状態を維持、1なら次の状態へ遷移する
	
			if(x == 1) {  // 状態が遷移するなら
				if(state == 0) {  // 自分の状態がThinkingの場合
					write(monitor[1], msg2, MSGSIZE);  // Hungryに遷移したことをモニターに伝える
					state = 1;  // stateの更新
				} else if(state == 2){  // 自分の状態がEatingの場合
					write(monitor[1], msg1, MSGSIZE);  //Thinkingに遷移したことをモニターに伝える
					state = 0;  // stateの更新
				}  
			}
			
			if(state == 1) {  // 自分の状態がHungryの場合
				if(*buf != *msg6) {  //　相手の状態がEatingでないならフォークは空いているのでEatingに遷移する
					write(monitor[1], msg3, MSGSIZE);  // Eatingに遷移したことをモニターに伝える
					state = 2;  // stateの更新
				} else {  // 相手がEating状態ならフォークが空くまで待っていないといけない
					write(monitor[1], msg7, MSGSIZE);  // Waiting（フォークを待っている状態）になっていることをモニターに伝える
					write(processB[1], msg7, MSGSIZE); // 相手のプロセスに自分が待ち状態であることを知らせる
					read(processA[0], buf, MSGSIZE);  // 相手にCPUを渡す
					j = 0;  // 占有の変数初期化
				}
			}
	
			if( j == 5 || *buf == *msg8) {
			// jはどのくらいの頻度でプロセスを切り替えるかを表す。今回は5回同じプロセスを実行し続けるか、相手が待ち状態になっていたら相手にCPUを渡すようにする
				if(state == 0) {  // Thinkingという状態だということを相手に伝える
					write(processB[1], msg1, MSGSIZE);
				} else if(state == 1) {  // Hungryという状態だということを相手に伝える
					write(processB[1], msg2, MSGSIZE);
				} else {  // Eatingという状態だということを相手に伝える
					write(processB[1], msg3, MSGSIZE); 
				}
				read(processA[0], buf, MSGSIZE);  // 相手にCPUを渡す
				j = 0;  // 占有の変数初期化
			}
	
			j++;

		}
    }  // プロセス1の処理終了
    else {  // プロセス2の処理開始
		write(monitor[1], msg4, MSGSIZE);   // 初期値の状態であるThinkingをモニターへ
		write(processA[1], msg4, MSGSIZE);  // 相手のプロセスに自分の状態を伝える
		read(processB[0], buf, MSGSIZE);    // 相手にCPUを渡す
		for(;;) {
			x = rand() % 2;  // 状態遷移をランダムで決める。0ならそのまま状態を維持、1なら次の状態へ遷移する
	
			if(x == 1) {  // 状態が遷移するなら
				if(state == 0) {  // 自分の状態がThinkingの場合
					write(monitor[1], msg5, MSGSIZE);  // Hungryに遷移したことをモニターに伝える
					state = 1;  // stateの更新
				} else if(state == 2) {  // 自分の状態がEatingの場合
					write(monitor[1], msg4, MSGSIZE);  //Thinkingに遷移したことをモニターに伝える
					state = 0;  // stateの更新
				}
			}
	
			if(state == 1) {  // 自分の状態がHungryの場合
				if(*buf != *msg3) {  //　相手の状態がEatingでないならフォークは空いているのでEatingに遷移する
					write(monitor[1], msg6, MSGSIZE);  // Eatingに遷移したことをモニターに伝える
					state = 2;  // stateの更新
				} else {  // 相手がEating状態ならフォークが空くまで待っていないといけない
					write(monitor[1], msg8, MSGSIZE);  // Waiting（フォークを待っている状態）になっていることをモニターに伝える
					write(processA[1], msg8, MSGSIZE); // 相手のプロセスに自分が待ち状態であることを知らせる
					read(processB[0], buf, MSGSIZE);   // 相手にCPUを渡す
					j = 0;  // 占有の変数初期化
				}
			}
	
			if( j == 5 || *buf == *msg7) {
			// jはどのくらいの頻度でプロセスを切り替えるかを表す。今回は5回同じプロセスを実行し続けるか、相手が待ち状態になっていたら相手にCPUを渡すようにする
				if(state == 0) {  // Thinkingという状態だということを相手に伝える
					write(processA[1], msg4, MSGSIZE);
				} else if(state == 1) {  // Hungryという状態だということを相手に伝える
					write(processA[1], msg5, MSGSIZE);
				} else {  // Eatingという状態だということを相手に伝える
					write(processA[1], msg6, MSGSIZE); 
				}
				read(processB[0], buf, MSGSIZE);  // 相手にCPUを渡す
				j = 0;  // 占有の変数初期化
			}
	
			j++;
		}

    }
}

/*
Mini Philosopher
 Moniter process
  Receive message about thier states from 2philosopher process and print them
 2 Philosopher process have 3 states
  Thinking, hungry -> eating
 2 philiosopher process share 1 stick
       monitor
       ^     ^
       |     |
 process0<->process1
*/
