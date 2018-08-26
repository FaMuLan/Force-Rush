BUILD_PATH	:= ../build/libsamplerate
CFLAGS	:= -c -O3
OBJ	:= \
	$(BUILD_PATH)/samplerate.o \
	$(BUILD_PATH)/src_linear.o \
	$(BUILD_PATH)/src_sinc.o \
	$(BUILD_PATH)/src_zoh.o

.PHONY: clear prepare

all: prepare $(BUILD_PATH)/libsamplerate.a

clear:
	rm -rf ../build/libsamplerate

prepare:
	-mkdir ../build
	-mkdir ../build/libsamplerate

$(BUILD_PATH)/libsamplerate.a: $(OBJ)
	$(AR) cr ../build/libsamplerate.a $(OBJ)

$(BUILD_PATH)/samplerate.o: samplerate.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/src_linear.o: src_linear.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/src_sinc.o: src_sinc.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/src_zoh.o: src_zoh.c
	$(CC) $< $(CFLAGS) -o $@