#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "../ae.h"
#include "../anet.h"

void readFromClient(aeEventLoop *loop, int fd, void *privdata, int mask) {
    int buffer_size = 1024;
    char buffer[1024];
    int size;
    size = read(fd, buffer, buffer_size);
    write(fd, buffer, size);
}

void acceptTcpHandler(aeEventLoop *loop, int fd, void *privdata, int mask) {
    int client_port, client_fd;
    char client_ip[128];
    // create client socket
    client_fd = anetTcpAccept(NULL, fd, client_ip, 128, &client_port);
    printf("Accepted %s:%d\n", client_ip, client_port);

    // set client socket non-block
    anetNonBlock(NULL, client_fd);
    // anetTcpNoDelay(NULL, client_fd);

    // regist on message callback
    int ret;
    ret = aeCreateFileEvent(loop, client_fd, AE_READABLE, readFromClient, NULL);
    assert(ret != AE_ERR);
}

int main()
{
    int ipfd;
    // create server socket
    ipfd = anetTcpServer(NULL, 8000, "0.0.0.0", 0);
    assert(ipfd != ANET_ERR);

    // create main event loop
    aeEventLoop *loop;
    loop = aeCreateEventLoop(10);

    // regist socket connect callback
    int ret;
    ret = aeCreateFileEvent(loop, ipfd, AE_READABLE, acceptTcpHandler, NULL);
    assert(ret != AE_ERR);

    // start main loop
    aeMain(loop);

    return 0;
}
