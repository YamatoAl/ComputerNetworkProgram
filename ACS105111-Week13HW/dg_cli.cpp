#include        "unp.h"
#include       <time.h>
#include       <stdio.h>
void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
        int     n;
        char    sendline[MAXLINE], recvline[MAXLINE + 1];
        clock_t t1,t2;
        double charBit;
        double total;
        while (Fgets(sendline, MAXLINE, fp) != NULL) {
                 t1 = clock();
                Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

                n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
                t2 = clock();
                charBit = (n-1)*8;
                total = (t2-t1)/(double)(CLOCKS_PER_SEC);
                total = charBit/(total*2);
                recvline[n] = 0;        /* null terminate */
                printf("bytes number: %lf\n",(n-1));
                printf("average throughout:%.0lf bps\n",charBit);
        }
}

