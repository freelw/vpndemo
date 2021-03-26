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
#include "tun_alloc.h"
#include "bridge.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please input server address.\n");
        return -1;
    }
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *        IFF_NO_PI - Do not provide packet information
     */
    int tun_fd = tun_alloc(IFF_TUN | IFF_NO_PI);
    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }
    struct sockaddr_in server;
    int clnt_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (clnt_sock == -1) {
        printf("Could not create socket.\n");
        exit(1);
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( 1234 );
    if (connect(clnt_sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        printf("connect error.\n");
        exit(1);
    }
    printf("server [%s]conneted.\n", argv[1]);
    bridge(clnt_sock, tun_fd);
    return 0;
}
