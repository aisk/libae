#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../ae.h"

int print(struct aeEventLoop *loop, long long id, void *clientData)

{
    printf("%lld - Hellow, World\n", id);
    return -1;
}

int main(void)
{
    aeEventLoop *loop = aeCreateEventLoop(10);
    int i;
    for (i=0;i<10;i++) {
        aeCreateTimeEvent(loop, i*1000, print, NULL, NULL);
    }
    aeMain(loop);
    aeDeleteEventLoop(loop);
    return 0;
}
