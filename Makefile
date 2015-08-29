# 		--		user config

CC ?= clang
LIBS = 
DEFS = -D_GNU_SOURCE 
INCLUDES = -I/usr/include -I./include

PROGRAM = schedsim
LIB = src/libschedsim.a
SOURCE = src/main.c
TESTS_DIR = tests/

BUILD := debug
CFLAGS := -std=gnu99 -Wall -Werror -O1 -g -DDEBUG


#			--		generation

HEADERS = $(shell find include/ -name '*.h')
SRCS = $(shell find src/ -name '*.c')
LIB_OBJS := $(patsubst %.c, %.o, $(filter-out $(SOURCE), $(SRCS)))
TESTS:= $(patsubst %.c, %.out, $(shell find $(TESTS_DIR) -name '*.c'))

all: $(PROGRAM) test 

$(PROGRAM): $(LIB) $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCE) $(DEFS) $(INCLUDES) $(LIBS) -o $@ $<

$(LIB): $(LIB_OBJS) 
	$(AR) rvs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c -o $@ $<


.PHONY: clean test 

test: $(LIB) $(SOURCES) $(TESTS) $(HEADERS)
	@$(foreach test_exec,$(TESTS),./$(test_exec);)

%.out: %.c
	$(CC) $(CFLAGS) $< $(DEFS) $(INCLUDES) $(LIBS) -o $@ $(LIB) 

print-%:
	@echo '$*=$($*)'

clean:
	find . \( -name "*.o" -o -name "*.a" -o -name "*.out" \) -type f -delete &
	find . \( -name $(PROGRAM) -o -name ".depend" \) -type f -delete

