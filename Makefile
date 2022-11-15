CC = g++
mochi = /home/users/liudy/software/spack-develop/var/spack/environments/rtai/.spack-env/view/
CFLAGS = -g -Wall --std=c++17 -D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200809L -I. -Icommon -Icommon/thirdparty -I$(mochi)/include -L$(mochi)/lib

TEST_CFLAGS = -DPLASMA_TEST=1 -Isrc
BUILD = build

all: $(BUILD)/dimes_server  $(BUILD)/dimes_client.so  $(BUILD)/libdimes_client.a common
#all: $(BUILD)/dimes_server  $(BUILD)/libdimes_client.a

debug: FORCE
debug: CFLAGS += -DRAY_COMMON_DEBUG=1
debug: all

clean:
	rm src/*.o
	rm $(BUILD)/*
	cd common; make clean

$(BUILD)/dimes_server: src/dimes_server.c src/share_string.c src/plasma.h src/fling.h src/fling.c src/malloc.c src/malloc.h thirdparty/dlmalloc.c common
	$(CC) $(CFLAGS) src/dimes_server.c src/share_string.c src/fling.c src/malloc.c common/build/libcommon.a -labt -lmargo -lmercury -lpthread -o $(BUILD)/dimes_server

$(BUILD)/dimes_client.so: src/dimes_client.c src/share_string.c src/plasma_client.c src/fling.c src/fling.c common
	$(CC) $(CFLAGS) src/dimes_client.c src/share_string.c src/plasma_client.c src/fling.c common/build/libcommon.a  -fPIC -shared -o $(BUILD)/dimes_client.so

$(BUILD)/libdimes_client.a: src/dimes_client.o src/share_string.o  src/plasma_client.o src/fling.o common/io.o common/common.o
	ar rcs $@ $^

common: FORCE
	cd common; make all

valgrind: test
	valgrind --leak-check=full --error-exitcode=1 ./build/manager_tests

FORCE:
