#include <stdlib.h>
#include <stdio.h>

#include "../ae.c"

int print(struct aeEventLoop *loop, long long id, void *clientData)

{
    printf("%lld - Hellow, World\n", id);
    return -1;
}

int main(void)
{
    printf("we are using %s\n", aeApiName());
    aeEventLoop *loop = aeCreateEventLoop();
    int i;
    for (i=0;i<10;i++) {
        aeCreateTimeEvent(loop, i*1000, print, NULL, NULL);
    }
    aeMain(loop);
    aeDeleteEventLoop(loop);
    return 0;
}
