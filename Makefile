SRC = src/ae.c src/anet.c
OBJ = ${SRC:.c=.o}
CFLAGS = -std=c99 -g -O0 -Wno-parentheses -Wno-switch-enum -Wno-unused-value

libae.a: $(OBJ)
	$(AR) -rc $@ $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

timer: libae.a example/timer.o
	$(CC) $^ -o $@

echo: libae.a example/echo.o
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ) libae.a example/timer.o timer example/echo.o echo

.PHONY: clean
