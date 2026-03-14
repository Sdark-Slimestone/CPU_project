# 编译参数：包含src目录，开启调试、优化、警告
CFLAGS=-g -O2 -Wall -Wextra -Isrc -I. -rdynamic -DNDEBUG $(OPTFLAGS)
# 链接库：dl库（动态加载）
LIBS=-ldl $(OPTLIBS)
# 安装前缀
PREFIX?=/usr/local

# 自动查找src下所有.c文件（包括子目录）
SOURCES=$(wildcard src/**/*.c src/*.c)
# 把.c文件转为.o目标文件
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

# 自动查找tests下所有*_tests.c测试文件
TEST_SRC=$(wildcard tests/*_tests.c)
# 把测试.c文件转为可执行文件
TESTS=$(patsubst %.c,%,$(TEST_SRC))

# 目标库名：静态库+动态库
TARGET=build/minirv.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

EXECUTABLE=minirv

# 默认目标：编译静态库、动态库、测试程序
all: $(TARGET) $(SO_TARGET) $(EXECUTABLE) tests

# 开发模式：关闭优化，开启更多警告
dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

# 编译最后的可执行文件
$(EXECUTABLE): minirv.c $(TARGET)
	$(CC) $(CFLAGS) $< $(TARGET) -o $@ $(LIBS) 
#展开等效：gcc -g -O2 -Wall -Wextra -Isrc -I. -rdynamic -DNDEBUG minirv.c build/minirv.a -o minirv -ldl


# 编译静态库：添加-fPIC（位置无关代码），打包成.a
$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

# 编译动态库：基于静态库和.o文件，生成.so
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

# 创建build/bin目录（通用规则）
.PHONY: build
build:
	@mkdir -p build
	@mkdir -p bin

# 核心修正1：显式定义测试文件编译规则（解决链接顺序问题）
$(TESTS): %: %.c $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(TARGET) -ldl
#-l  + minirv 

# 核心修正2：测试目标添加LD_LIBRARY_PATH（解决运行时找不到动态库）
.PHONY: tests
tests: $(TARGET) $(TESTS)
	LD_LIBRARY_PATH=./build python3 /home/sdark/cpu_project/minirv/py/runtests.py

# valgrind内存检测（可选）
valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# 清理规则：删除所有编译产物
clean:
	rm -rf build/* $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f minirv

# 安装规则：把库安装到系统目录（需sudo）
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	install $(SO_TARGET) $(DESTDIR)/$(PREFIX)/lib/

# 代码检查：查找危险函数（可选）
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true