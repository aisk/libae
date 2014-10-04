# libae

Redis's async event library, you can use it in your projects.

## supported event multiplexing model

* `epoll`
* `kqueue`
* `ev_port`
* `select`

## Example

### Timer

Print `Hello, World` on screen every 10 seconds:

```C
int print(struct aeEventLoop *loop, long long id, void *clientData)
{
    printf("%lld - Hello, World\n", id);
    return -1;
}

int main(void)
{
    aeEventLoop *loop = aeCreateEventLoop(0);
    int i;
    for (i = 0; i < 10; i ++) {
        aeCreateTimeEvent(loop, i*1000, print, NULL, NULL);
    }
    aeMain(loop);
    aeDeleteEventLoop(loop);
    return 0;
}
```

### Echo server

Start an echo server on 8000:

```C
void writeToClient(aeEventLoop *loop, int fd, void *clientdata, int mask)
{
    char *buffer = clientdata;
    printf("%p\n", clientdata);
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
    aeCreateFileEvent(loop, fd, AE_WRITABLE, writeToClient, buffer);
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
```

[original document](http://redis.io/topics/internals-rediseventlib)
