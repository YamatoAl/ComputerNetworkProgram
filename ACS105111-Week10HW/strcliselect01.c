#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
	int maxfdp1,rv;
	fd_set rset;
	char sendline[MAXLINE],recvline[MAXLINE];
	struct timeval tv;
	
	tv.tv_sec = 60;
	tv.tv_usec = 500000;
	
	FD_ZERO(&rset);
	
	for(;;){
		FD_SET(fileno(fp),&rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		rv = Select(maxfdp1, &rset, NULL, NULL, &tv);

		if(rv == -1) perror("select");
		
		else if(rv == 0){
			printf("Time out\n");
			Writen(sockfd,"idle message\n",15);
			return;
		}
		
		else{
			if(FD_ISSET(sockfd,&rset)){	/* socket is readable */
				if (Readline(sockfd, recvline, MAXLINE) == 0) err_quit("str_cli: server terminated prematurely");
				Fputs(recvline, stdout);
			}

			if(FD_ISSET(fileno(fp),&rset)){  /* input is readable */
				if (Fgets(sendline, MAXLINE, fp) == NULL) return;	/* all done */
				Writen(sockfd, sendline, strlen(sendline));
			}
		}	
	}
}
