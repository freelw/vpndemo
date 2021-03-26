#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/if_tun.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define max(a,b) ((a)>(b) ? (a):(b))

#include "tun_alloc.h"


int main()
{
    int tun_fd;
    char buf[1500];
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *        IFF_NO_PI - Do not provide packet information
     */
    tun_fd = tun_alloc(IFF_TUN | IFF_NO_PI);
    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }
    printf("i'm [server].\n");
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_port = htons(1234);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 20);
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("vpn client is coming.\n");
    ioctl(clnt_sock, TUNSETNOCSUM, 1); 
    ioctl(tun_fd, TUNSETNOCSUM, 1); 
    fd_set fds;
    int fm = max(clnt_sock, tun_fd) + 1;
    int l = 0;
    while(1){
        FD_ZERO(&fds);
        FD_SET(clnt_sock, &fds);
        FD_SET(tun_fd, &fds);
        select(fm, &fds, NULL, NULL, NULL);
        if(FD_ISSET(clnt_sock, &fds)) {
            l = read(clnt_sock,buf,sizeof(buf));
            printf("recieved %d bytes from vpn client.\n", l);
            write(tun_fd,buf,l);
        }
        if(FD_ISSET(tun_fd, &fds)) {
            l = read(tun_fd, buf, sizeof(buf));
            printf("sending %d bytes to vpn client.\n", l);
            write(clnt_sock, buf, l);
        }
    }
    return 0;
}
