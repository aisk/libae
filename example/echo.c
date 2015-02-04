#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "../src/ae.h"
#include "../src/anet.h"

void writeToClient(aeEventLoop *loop, int fd, void *clientdata, int mask)
{
    char *buffer = clientdata;
    //printf("%p\n", clientdata);
    printf("fd[%d] %s\n",fd, buffer);
    write(fd, buffer, strlen(buffer));
    free(buffer);
    aeDeleteFileEvent(loop, fd, AE_WRITABLE);
}

void readFromClient(aeEventLoop *loop, int fd, void *clientdata, int mask)
{
    int buffer_size = 1024;
    char *buffer = malloc(sizeof(char) * buffer_size);
    bzero(buffer, buffer_size);
    int size;
    size = read(fd, buffer, buffer_size);
    
    if(size <0)
    {
        if(errno == EAGAIN || errno == EINTR)
        {
            // receive buffer is empty, or be interrupted
            return;
        }

        aeDeleteFileEvent(loop,fd,AE_READABLE|AE_WRITABLE);
        close(fd);

        if (errno == ECONNRESET)
        {
            // RST
            printf("fd[%d] counterpart send out RST\n",fd);
        }
        else
        {
            printf("fd[%d] unrecovable error\n",fd);
        }
    }
    else if(size ==0 )
    {
        // client has closed, get a FIN
        aeDeleteFileEvent(loop,fd,AE_READABLE|AE_WRITABLE);
        close(fd);
        printf("fd[%d] counterpart has shut off\n",fd);
    }
    else
    {
        aeCreateFileEvent(loop, fd, AE_WRITABLE, writeToClient, buffer);
    }
}

void acceptTcpHandler(aeEventLoop *loop, int fd, void *clientdata, int mask)
{
    int client_port, client_fd;
    char client_ip[128];
    // create client socket
    client_fd = anetTcpAccept(NULL, fd, client_ip, 128, &client_port);
    printf("Accepted %s:%d\n", client_ip, client_port);

    // set client socket non-block
    anetNonBlock(NULL, client_fd);

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
    loop = aeCreateEventLoop(1024);

    // regist socket connect callback
    int ret;
    ret = aeCreateFileEvent(loop, ipfd, AE_READABLE, acceptTcpHandler, NULL);
    assert(ret != AE_ERR);

    // start main loop
    aeMain(loop);

    // stop loop
    aeDeleteEventLoop(loop);

    return 0;
}
