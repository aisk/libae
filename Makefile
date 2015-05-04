SRC = src/ae.c src/anet.c
OBJ = ${SRC:.c=.o}
CFLAGS = -D_POSIX_SOURCE -std=c99 -g -O0 -Wno-parentheses -Wno-switch-enum -Wno-unused-value

libae.a: $(OBJ)
	$(AR) -rc $@ $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

timer: example/timer.o libae.a
	$(CC) $^ -o $@

echo: example/echo.o libae.a
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ) libae.a example/timer.o timer example/echo.o echo

.PHONY: clean
