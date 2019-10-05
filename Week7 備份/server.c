#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h> // import waitpid()
#include <signal.h> // import signal()
#include <time.h>

// 印出error message
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// 處理子進程接收message的事件
void NP_AcceptMessage(int newsockfd){
	int n;
	char buffer[256]; // 儲存接收訊息
	
	bzero(buffer, 256);
	n = read(newsockfd, &buffer, 255);
	
	// 錯誤讀取
	if(n < 0)
		error("ERROR ON READ");
	
	// 將成功讀取資訊寫回給client
	n = write(newsockfd, "Server got your message!!", 25);

	// 寫回錯誤
	if(n < 0)
		error("ERROR ON WRITE");
	
	// 印出接收資訊
	printf("Your message is: %s", buffer);
}

// 等待捕捉子程序的結束，呼叫該函式，處理zombie process問題
void sigchld_handler(){ 
	pid_t PID;
	int status;
	
	// 使用waitpid解決zombie process
	while(PID = waitpid(-1, &status, WNOHANG) > 0)
		printf("pid end, status %d\n", PID);
	
	return;
}


// 傳送時間資訊
void sendTime(int newsockfd){
	int n;
	char buffer[256]; // 儲存接收訊息
	time_t ticks; // 建立時間資訊
	struct tm * timeinfo; // 時間結構	
	
	// 重複執行直到沒有訊息傳過來
	bzero(buffer, 256); 
	while(read(newsockfd, &buffer, 255) > 0){
		// 當地時間取得
		time(&ticks);
		timeinfo = localtime(&ticks);
		
		// 錯誤讀取
		if(n < 0)
			error("ERROR ON READ");

		// 印出接收資訊
		printf("pid %d send message : %s\n", getpid(), buffer);
		
		// 回傳伺服器當地時間給server
		n = write(newsockfd, asctime(timeinfo), strlen(asctime(timeinfo)));

		// 寫回錯誤
		if(n < 0)
			error("ERROR ON WRITE");
		
		bzero(buffer, 256); 
	}
	
	return;
}

/* 
 * argc為argv的長度
 * 當使用者輸入 ./執行檔 portNumber時
 * argv就會存入 "執行檔" "portNumber"兩個字串
 */
int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen; 
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	time_t nowTime;

	// 處理信號結束事件
	signal(SIGCHLD, sigchld_handler); 

	// 如果輸入字串數小於2(表示沒輸入portNumber)
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	// int socket(int domain, int type, int protocol) 回傳sockfd檔案描述符，建立socket失敗回傳-1
	// 		domain: 決定socket的溝通方式，AF_INET為用IPv4溝通
	// 		type: 決定socket的傳輸方式，SOCK_STREAM為用TCP方式傳輸
	// 		protocol: socket的協定標準，通常設為0讓kernel選擇默認的協定
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
	error("ERROR opening socket");

	// 將serv_addr內的參數全部轉為0
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// 將輸入的字串port number轉為int給portno
	portno = atoi(argv[1]);

	// 為server賦予數值
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY; // 將s_addr的IP地址設為0.0.0.0(接收所有地址)

	// bind綁定，讓server_addr資訊與socket綁定在一起
	// 雖然socket有命名空間，但是沒有賦予其伺服器名稱
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		  sizeof(serv_addr)) < 0) 
		  error("ERROR on binding");
		  
	// 聽取並等待連線，queue最多存放5個封包要求
	listen(sockfd,5);

	//fork程式碼
	clilen = sizeof(cli_addr); // 取得client的sock_addr大小
	
	// 重複執行接收封包的程式
	while(1){
		// 得到連線要求，呼叫accept函式
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
		// 呼叫fork()，fork分兩段執行
		pid = fork();
		
		// 分段錯誤
		if(pid < 0){
			error("ERROR ON FORK");
		}
		// 第一段：子程序
		else if(pid == 0){
			close(sockfd);
			//NP_AcceptMessage(newsockfd);
			sendTime(newsockfd);
			exit(0);
		}
		// 第二段：父程序
		else{
			close(newsockfd);
		}
	}
}
