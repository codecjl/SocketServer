#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE     128

typedef struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
}msgbuf;

int main(int argc, char * argv[])
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = 1234;

    if(argc != 2)
    {
        printf("\n Usage: %s <number of message> \n",argv[0]);
        return 1;
    } 

    if ((msqid = msgget(key, msgflg )) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    //Message Type
    sbuf.mtype = 1;

    time_t      curtime;
    struct tm   tp;
	struct timeval tv;
    memset(&tp, 0, sizeof(tp));

	long loops = strtol(argv[1], NULL, 10);
	long i;
    for ( i = 0; i < loops; i++){
		time( &curtime );
		localtime_r( &curtime, &tp );
        gettimeofday(&tv,NULL);

		if( tv.tv_usec & 1 )
			snprintf( sbuf.mtext, MAXSIZE, "00%02d:%02d:%02d.%06ld%083ld",
				tp.tm_hour, tp.tm_min, tp.tm_sec,
				tv.tv_usec, i);
		else
			snprintf( sbuf.mtext, MAXSIZE, "01%02d:%02d:%02d.%06ld%083ld",
				tp.tm_hour, tp.tm_min, tp.tm_sec,
				tv.tv_usec, i);

		buflen = strlen(sbuf.mtext) + 1 ;

		if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0){
			printf ("Error while seding a message");
			perror("msgsnd");
			exit(1);
		}
    }
	printf("Message All Sent\n");

    exit(0);
}
