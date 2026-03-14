SRC := src/ae.c src/anet.c src/monotonic.c
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)
EXAMPLES := timer echo
EXAMPLE_OBJ := $(EXAMPLES:%=example/%.o)
EXAMPLE_DEP := $(EXAMPLE_OBJ:.o=.d)
WARNINGS := -Wno-parentheses -Wno-switch-enum -Wno-unused-value

CFLAGS ?=
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?=
ARFLAGS ?= -rc

all: libae.a $(EXAMPLES)

libae.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

timer: example/timer.o libae.a
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

echo: example/echo.o libae.a
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJ) $(DEP) libae.a $(EXAMPLE_OBJ) $(EXAMPLE_DEP) $(EXAMPLES)

.PHONY: all clean

-include $(DEP) $(EXAMPLE_DEP)
