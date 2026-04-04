# 用户配置
MAIN_SRC = src/lcthw/scpu.c     # 主程序源文件路径
EXECUTABLE = scpu                # 可执行文件名
TARGET_LIB = scpu.a              # 静态库名
SO_TARGET_LIB = $(TARGET_LIB:.a=.so) # 动态库名

OPTFLAGS ?=
CFLAGS = -g -O2 -Wall -Wextra -Isrc -I. -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

# 自动查找所有源文件（排除主程序）
ALL_SRCS = $(shell find src -name "*.c")
SOURCES = $(filter-out $(MAIN_SRC), $(ALL_SRCS))
OBJECTS = $(patsubst src/%.c, build/%.o, $(SOURCES))

# 测试文件
TEST_SRC = $(wildcard tests/*_tests.c)
TESTS = $(TEST_SRC:.c=)

TARGET = build/$(TARGET_LIB)
SO_TARGET = build/$(SO_TARGET_LIB)

all: $(TARGET) $(SO_TARGET) $(EXECUTABLE) tests

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

$(TESTS): %: %.c $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(TARGET) -ldl

tests: $(TARGET) $(TESTS)
	@echo "No tests for scpu"

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log $(EXECUTABLE)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf $(shell find . -name "*.dSYM" -print)

install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	install $(SO_TARGET) $(DESTDIR)/$(PREFIX)/lib/

check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true