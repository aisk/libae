SRC = src/ae.c src/anet.c
OBJ = ${SRC:.c=.o}
CFLAGS = -D_POSIX_SOURCE -std=c99 -g -O0 -Wno-parentheses -Wno-switch-enum -Wno-unused-value

libae.a: $(OBJ)
	$(AR) -rc $@ $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

timer: example/timer.c libae.a
	$(CC) $^ -o $@

echo: example/echo.c  libae.a
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ) libae.atimer echo

.PHONY: clean
