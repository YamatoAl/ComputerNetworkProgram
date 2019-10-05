/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void sigchld_handler(){ 
	pid_t Pid;
	int status;
	
	while((Pid = waitpid(-1, &status, WNOHANG)) > 0)
		printf("pid %d end\n", Pid);
	
	return;
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen; 
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	signal(SIGCHLD, sigchld_handler);

	if (argc < 2) {
	 fprintf(stderr,"ERROR, no port provided\n");
	 exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
	error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		  sizeof(serv_addr)) < 0) 
		  error("ERROR on binding");
		  
	listen(sockfd,5);

	clilen = sizeof(cli_addr);
	for(;;){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		pid = fork();
		if(pid < 0) error("ERROR on fork");
		else if(pid == 0){

			char buffer[256]; 
			time_t tick; 
			struct tm * ltime; 
			
			close(sockfd);
			bzero(buffer, 256); 
			for(;read(newsockfd, &buffer, 255)>0;){
				time(&tick);
				ltime = localtime(&tick);
		
				if(n < 0) error("ERROR on read");

				printf("Get message : \"%s\" from pid: %d\n", buffer, getpid());
		
				n = write(newsockfd, asctime(ltime), strlen(asctime(ltime)));

				if(n < 0) error("ERROR ON WRITE");
		
				bzero(buffer, 256); 
			}

			exit(0);
		}
		else close(newsockfd);
	}
}