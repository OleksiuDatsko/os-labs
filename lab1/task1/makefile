INPUTFLAG ?= single

CC = gcc
LIBFLAG = 
TARGET = main
DYNAMIC_LIB_NAME = libfunctions_dynamic.so
CFLAGS ?=

ifeq ($(INPUTFLAG),static)
    REQO = main.o libfunctions.a
    LIBFLAG = -L. -lfunctions
else ifeq ($(INPUTFLAG),single)
    REQO = main.o power.o factorial.o
else ifeq ($(INPUTFLAG),dynamic)
    REQO = main.o $(DYNAMIC_LIB_NAME)
    LIBFLAG = -L. -lfunctions_dynamic
	CFLAGS += -DLIBNAME="$(DYNAMIC_LIB_NAME)/"
else
    $(error Invalid INPUTFLAG value. Use 'static' or 'single'.)
endif

$(TARGET): $(REQO)
	$(CC) -o $@ $^ $(LIBFLAG)

main.o: main.c
	@echo "$(INPUTFLAG) compiling"
	$(CC) -o $@ -c $<

power.o: power.c
	$(CC) -o $@ -c $<

factorial.o: factorial.c
	$(CC) -o $@ -c $<

libfunctions.a: power.o factorial.o
	@echo "Creating static library $@"
	ar rcs $@ $^

libfunctions_dynamic.so: power.o factorial.o
	@echo "Creating dynamic library $@"
	@export LD_LIBRARY_PATH=.
	$(CC) -shared -o $(DYNAMIC_LIB_NAME) $^

clean:
	rm -f $(TARGET) *.o *.a *.so

.PHONY: clean
