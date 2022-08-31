#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <stdbool.h> 
#include <sys/time.h>
#include <errno.h>
#define BUFSIZE     128
#define TIMESIZE    20
void printTime(void){
    time_t      curtime;
    struct tm   tp;
	struct timeval tv;
    memset(&tp, 0, sizeof(tp));
    time( &curtime );
    localtime_r( &curtime, &tp );
    gettimeofday(&tv,NULL);
    printf("%02d:%02d:%02d.%06ld\n", tp.tm_hour, tp.tm_min, tp.tm_sec, tv.tv_usec);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    setbuf(stdout, NULL);
    int listenfd = 0, connfd = 0, reuse = 1;
    long n = 0;
    struct sockaddr_in serv_addr; 
    FILE * fd = fopen( "OrderQuote.dat" , "w");
    setbuf(fd, NULL);

    char recvBuff[BUFSIZE];
    char recvTime[TIMESIZE];
    time_t      curtime;
    struct tm   tp;
	struct timeval tv;
    memset(&tp, 0, sizeof(tp));
    //time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(recvBuff, '0', sizeof(recvBuff)); 

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    if ( bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 ){
        perror("bind fail ");
        return false;
    }
    if ( listen(listenfd, 10) < 0 ){
        perror("listen fail ");
        return false;
    }

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        //ticks = time(NULL);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        //write(connfd, sendBuff, strlen(sendBuff)); 
        puts("wait data...");
        //while ( (n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0)
        while ( true )
        {
            if( (n = recv(connfd, recvBuff, 100, 0 )) > 0 ){
                recvBuff[n] = '\n';
                recvBuff[n+1] = 0;
                //printf("(%d)\n%s\n", n, recvBuff);
                //fflush(stdout);
                printf(".");

                time( &curtime );
                localtime_r( &curtime, &tp );
                gettimeofday(&tv,NULL);
                snprintf(recvTime, sizeof(recvTime), " [%02d:%02d:%02d.%06ld]\n", tp.tm_hour, tp.tm_min, tp.tm_sec, tv.tv_usec);
                fwrite(recvTime, strlen(recvTime), 1, fd);
                fwrite(recvBuff, strlen(recvBuff), 1, fd);
            }
            else if ( n==0 ) {
                printTime();
                puts("client leave.");
                break;
            }
            else{
                printTime();
                perror("recv error: ");
                break;
            }
        }
        close(connfd);
        usleep(1);
     }
}
