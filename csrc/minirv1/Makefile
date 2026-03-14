CFLAGS = -g -O2 -Wall -Wextra -Isrc -I. -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
TEST_SRC = $(wildcard tests/*_tests.c)
TESTS = $(patsubst %.c,%,$(TEST_SRC))

TARGET = build/minirv.a
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))
EXECUTABLE = minirv

all: $(TARGET) $(SO_TARGET) $(EXECUTABLE) tests

dev: CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(EXECUTABLE): minirv.c $(TARGET)
	$(CC) $(CFLAGS) $< $(TARGET) -o $@ $(LIBS)

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

build:
	@mkdir -p build
	@mkdir -p bin

$(TESTS): %: %.c $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(TARGET) -ldl

tests: $(TARGET) $(TESTS)
	LD_LIBRARY_PATH=./build python3 /home/sdark/cpu_project/minirv/py/runtests.py

clean:
	rm -rf build/* $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f minirv