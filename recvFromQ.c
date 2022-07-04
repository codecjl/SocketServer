#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <time.h>
#include <sys/time.h>
#define MAXSIZE     128
#define BUFSIZE     128

typedef struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
} msgbuf;

void printTime(){
    time_t      curtime;
    struct tm   tp;
	struct timeval tv;
    memset(&tp, 0, sizeof(tp));
    time( &curtime );
    localtime_r( &curtime, &tp );
    gettimeofday(&tv,NULL);
    printf("%02d:%02d:%02d.%06ld\n", tp.tm_hour, tp.tm_min, tp.tm_sec, tv.tv_usec);
}

int main(int argc, char * argv[])
{
    setbuf(stdout, NULL);
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;

    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    // Socket init
    int sockfd = 0, n=0, firsttry=1;
    char sendBuff[BUFSIZE];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(sendBuff, '0',sizeof(sendBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    //if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    //{
    //   printf("\n Error : Connect Failed \n");
    //   return 1;
    //}
    while ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0 ){
        printTime();
        perror("retry connect to server 1 ");
        close(sockfd);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        usleep(500000);
    }

    puts("recv q data...");
    while (true){
        //Receive an answer of message type 1.
        if ( (n= msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0)) < 0)
        {
            printTime();
            perror("msgrcv");
            exit(1);
        }
        rcvbuffer.mtext[n]=0;
        //if( write(sockfd, sendBuff, strlen(sendBuff)) < 0 ){

        if( send(sockfd, rcvbuffer.mtext, strlen(rcvbuffer.mtext), MSG_NOSIGNAL) < 0 ){
            printTime();
            perror("write socket fail. connection may be broken ");
            close(sockfd);
            //re-connect
            if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                printf("\n Error : Could not create socket \n");
                return 1;
            } 
            while ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0 ){
                if ( firsttry == 1 ){
                    printTime();
                    perror("retry connect to server 2 ");
                    firsttry = 0;
                }
                else
                    printf(".");    
                if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    printf("\n Error : Could not create socket \n");
                    return 1;
                } 
                usleep(500000);
            }
	    printf("\n");
            printTime();
            puts("reconnect successful\n");
            firsttry = 1;
            if( send(sockfd, rcvbuffer.mtext, strlen(rcvbuffer.mtext), MSG_NOSIGNAL) < 0 ){
                printTime();
                perror("Can not send the data out. EXIT ");
                close(sockfd);
                exit(1);
            }
        }
        //printf("%s\n", rcvbuffer.mtext);
        usleep(1);
    }
    close(sockfd);
    exit(0);
}
