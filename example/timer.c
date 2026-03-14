#include <stdio.h>
#include <stdlib.h>

#include "../src/ae.h"

#define EVENT_COUNT 8
typedef struct timerState {
    int count;
} timerState;

int print(struct aeEventLoop *loop, long long id, void *clientData)
{
    timerState *state = clientData;

    AE_NOTUSED(id);
    printf("event %d - Hello World %d\n", state->count, state->count);

    state->count++;
    if (state->count == EVENT_COUNT) {
        aeStop(loop);
        return AE_NOMORE;
    }

    return 1000;
}

int main(void)
{
    aeEventLoop *loop = aeCreateEventLoop(10);
    if (loop == NULL) {
        return 1;
    }

    timerState state = {0};
    if (aeCreateTimeEvent(loop, 1000, print, &state, NULL) == AE_ERR) {
        aeDeleteEventLoop(loop);
        return 1;
    }

    aeMain(loop);
    aeDeleteEventLoop(loop);
    return 0;
}
