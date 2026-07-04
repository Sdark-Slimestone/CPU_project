# rv32e-csr ISA 模拟器
MAIN_SRC = src/main.c
EXECUTABLE = rv32e-csr
TARGET_LIB = rv32e-csr.a
SO_TARGET_LIB = $(TARGET_LIB:.a=.so)

OPTFLAGS ?=
CFLAGS = -g -O2 -Wall -Wextra -Isrc -I. -rdynamic -DNDEBUG -fPIC $(OPTFLAGS)
LIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

ALL_SRCS = $(shell find src -name "*.c")
SOURCES = $(filter-out $(MAIN_SRC), $(ALL_SRCS))
OBJECTS = $(patsubst src/%.c, build/%.o, $(SOURCES))

TARGET = build/$(TARGET_LIB)
SO_TARGET = build/$(SO_TARGET_LIB)

all: $(TARGET) $(SO_TARGET) $(EXECUTABLE)

dev: CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

build:
	@mkdir -p build

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

$(EXECUTABLE): $(MAIN_SRC) $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(TARGET) $(LIBS)

# 构建 diff-test 动态库（给 NPC 的 diff 功能调用）
.PHONY: so
so: $(SO_TARGET)

clean:
	rm -rf build $(OBJECTS)
	rm -f $(EXECUTABLE)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf $(shell find . -name "*.dSYM" -print)