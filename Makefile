CC := gcc
CFLAGS ?=

BUILD_DIR := build
SRC_DIR := src

BASM := basm
BEMU := bemu

ARGS ?=

all: basm bemu

basm: $(BUILD_DIR)/$(BASM)/$(BASM)

bemu: $(BUILD_DIR)/$(BEMU)/$(BEMU)

$(BUILD_DIR)/$(BASM)/$(BASM): $(SRC_DIR)/$(BASM)/main.c $(SRC_DIR)/$(BASM)/cpu.c
	mkdir -p $(BUILD_DIR)/$(BASM)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(BASM)/$(BASM) $(SRC_DIR)/$(BASM)/main.c $(SRC_DIR)/$(BASM)/cpu.c

$(BUILD_DIR)/$(BEMU)/$(BEMU): $(SRC_DIR)/$(BEMU)/main.c
	mkdir -p $(BUILD_DIR)/$(BEMU)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(BEMU)/$(BEMU) $(SRC_DIR)/$(BEMU)/main.c

run-basm: basm
	$(BUILD_DIR)/$(BASM)/$(BASM) $(ARGS)

run-bemu: bemu
	$(BUILD_DIR)/$(BEMU)/$(BEMU) $(ARGS)
