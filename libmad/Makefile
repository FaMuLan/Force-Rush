BUILD_PATH	:= ../build/libmad
CFLAGS	:= -c -O3 -DFPM_DEFAULT
OBJ	:= \
	$(BUILD_PATH)/bit.o \
	$(BUILD_PATH)/decoder.o \
	$(BUILD_PATH)/fixed.o \
	$(BUILD_PATH)/frame.o \
	$(BUILD_PATH)/huffman.o \
	$(BUILD_PATH)/layer12.o \
	$(BUILD_PATH)/layer3.o \
	$(BUILD_PATH)/stream.o \
	$(BUILD_PATH)/synth.o \
	$(BUILD_PATH)/timer.o \
	$(BUILD_PATH)/version.o

.PHONY: clear prepare

all: prepare $(BUILD_PATH)/libmad.a

clear:
	rm -rf ../build/libmad

prepare:
	-mkdir ../build
	-mkdir ../build/libmad

$(BUILD_PATH)/libmad.a: $(OBJ)
	$(AR) cr ../build/libmad.a $(OBJ)

$(BUILD_PATH)/bit.o: bit.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/decoder.o: decoder.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/fixed.o: fixed.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/frame.o: frame.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/huffman.o: huffman.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/layer12.o: layer12.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/layer3.o: layer3.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/stream.o: stream.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/synth.o: synth.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/timer.o: timer.c
	$(CC) $< $(CFLAGS) -o $@
$(BUILD_PATH)/version.o: version.c
	$(CC) $< $(CFLAGS) -o $@