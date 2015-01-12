SRC = src/ae.c src/anet.c
OBJ = ${SRC:.c=.o}
CFLAGS = -std=c99 -g -O0 -Wno-parentheses -Wno-switch-enum -Wno-unused-value

all: $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) example/timer.o timer example/echo.o echo

timer: $(OBJ) example/timer.o
	$(CC) $^ -o $@

echo: $(OBJ) example/echo.o
	$(CC) $^ -o $@

.PHONY: clean
