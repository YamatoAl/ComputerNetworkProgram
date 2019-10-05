/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// 處理子進程接收message的事件
void NP_AcceptMessage(int newsockfd){
	int n;
	char buffer[256];
	
	bzero(buffer, 256);
	n = read(newsockfd, &buffer, 255);
	
	if(n < 0)
		error("ERROR ON READ");
	
	n = write(newsockfd, "Server got your message!!", 25);

	if(n < 0)
		error("ERROR ON WRITE");
	
	printf("Your message is: %s", buffer);
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
	//char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	// 如果輸入字串數小於2(表示沒輸入portNumber)
	if (argc < 2) {
	 fprintf(stderr,"ERROR, no port provided\n");
	 exit(1);
	}

	// int socket(int domain, int type, int protocol) 回傳sockfd檔案描述符，建立socket失敗回傳-1
	// 	domain: 決定socket的溝通方式，AF_INET為用IPv4溝通
	//		type: 決定socket的傳輸方式，SOCK_STREAM為用TCP方式傳輸
	//		protocol: socket的協定標準，通常設為0讓kernel選擇默認的協定
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

	// fork程式碼
	clilen = sizeof(cli_addr);
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		pid = fork();
		if(pid < 0){
			error("ERROR ON FORK");
		}
		else if(pid == 0){
			close(sockfd);
			NP_AcceptMessage(newsockfd);
			exit(0);
		}
		else{
			close(newsockfd);
		}
	}
}