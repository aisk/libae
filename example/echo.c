#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../src/ae.h"
#include "../src/anet.h"

#define BUFFER_SIZE 1024

void closeClient(aeEventLoop *loop, int fd)
{
    aeDeleteFileEvent(loop, fd, AE_READABLE);
    close(fd);
}

void readFromClient(aeEventLoop *loop, int fd, void *clientData, int mask)
{
    char buffer[BUFFER_SIZE];
    ssize_t size;

    AE_NOTUSED(clientData);
    AE_NOTUSED(mask);

    size = read(fd, buffer, sizeof(buffer));
    if (size <= 0) {
        printf("Client disconnected\n");
        closeClient(loop, fd);
        return;
    }

    printf("recv client [%d] data: ", fd);
    fwrite(buffer, 1, (size_t)size, stdout);
    if (buffer[size - 1] != '\n') {
        putchar('\n');
    }
    write(fd, buffer, (size_t)size);
}

void acceptTcpHandler(aeEventLoop *loop, int fd, void *clientData, int mask)
{
    int client_port, client_fd;
    char client_ip[128];

    AE_NOTUSED(clientData);
    AE_NOTUSED(mask);

    client_fd = anetTcpAccept(NULL, fd, client_ip, sizeof(client_ip), &client_port);
    if (client_fd == ANET_ERR) {
        return;
    }

    printf("Accepted %s:%d\n", client_ip, client_port);
    assert(aeCreateFileEvent(loop, client_fd, AE_READABLE, readFromClient, NULL) != AE_ERR);
}

int main(void)
{
    int server_fd;
    aeEventLoop *loop;

    server_fd = anetTcpServer(NULL, 8000, "localhost", 0);
    assert(server_fd != ANET_ERR);

    loop = aeCreateEventLoop(1024);
    assert(loop != NULL);

    assert(aeCreateFileEvent(loop, server_fd, AE_READABLE, acceptTcpHandler, NULL) != AE_ERR);
    aeMain(loop);
    aeDeleteEventLoop(loop);
    return 0;
}
