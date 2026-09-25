CC = gcc
LD = gcc

concc: $(COM_BIN)
conci: $(INT_BIN)

CFLAGS = -g -c -Wall
LDFLAGS = -lm

COMMON_SRC = $(wildcard src/common/*.c)
COMMON_HDR = $(wildcard src/common/*.h)
COMMON_OBJ = $(COMMON_SRC:src/common/%.c=obj/common_%.o)

COM_SRC = $(wildcard src/comp/*.c)
COM_HDR = $(wildcard src/comp/*.h)
COM_OBJ = $(COM_SRC:src/comp/%.c=obj/comp_%.o)

INT_SRC = $(wildcard src/int/*.c)
INT_HDR = $(wildcard src/int/*.h)
INT_OBJ = $(INT_SRC:src/int/%.c=obj/int_%.o)

COM_BIN = concc
INT_BIN = conci

$(INT_BIN): $(INT_OBJ) $(COMMON_OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(COM_BIN): $(COM_OBJ) $(COMMON_OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

obj/common_%.o: src/common/%.c $(COMMON_HDR)
	$(CC) -o $@ $< $(CFLAGS)
obj/comp_%.o: src/comp/%.c $(COMMON_HDR) $(COMP_HDR)
	$(CC) -o $@ $< $(CFLAGS)
obj/int_%.o: src/int/%.c $(COMMON_HDR) $(INT_HDR)
	$(CC) -o $@ $< $(CFLAGS)
