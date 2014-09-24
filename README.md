# libae

Redis's async event library, you can use it in your projects.

## supported event multiplexing model

* `epoll`
* `kqueue`
* `ev_port`
* `select`

## Sample

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
