#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define max(a,b) ((a)>(b) ? (a):(b))

void bridge(int fd1, int fd2)
{
    char buf[1500];
    fd_set fds;
    int fm = max(fd1, fd2) + 1;
    int l = 0;
    while(1){
        FD_ZERO(&fds);
        FD_SET(fd1, &fds);
        FD_SET(fd2, &fds);
        select(fm, &fds, NULL, NULL, NULL);
        if(FD_ISSET(fd1, &fds)) {
            l = read(fd1,buf,sizeof(buf));
            if (l == 0) {
                printf("read fd1 failed.");
                break;
            }
            printf("recieved %d bytes.\n", l);
            write(fd2,buf,l);
        }
        if(FD_ISSET(fd2, &fds)) {
            l = read(fd2, buf, sizeof(buf));
            if (l == 0) {
                printf("read fd2 failed.");
                break;
            }
            printf("sending %d bytes.\n", l);
            write(fd1, buf, l);
        }
    }
}