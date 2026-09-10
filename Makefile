CC := gcc
CFLAGS ?=

BUILD_DIR := build
SRC_DIR := src
TEST_DIR := test

BASM := basm
BEMU := bemu

ARGS ?=

all: basm bemu

basm: $(BUILD_DIR)/$(BASM)/$(BASM)

bemu: $(BUILD_DIR)/$(BEMU)/$(BEMU)

$(BUILD_DIR)/$(BASM)/$(BASM): $(SRC_DIR)/$(BASM)/main.c
	mkdir -p $(BUILD_DIR)/$(BASM)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(BASM)/$(BASM) $(SRC_DIR)/$(BASM)/main.c

$(BUILD_DIR)/$(BEMU)/$(BEMU): $(SRC_DIR)/$(BEMU)/main.c $(SRC_DIR)/$(BEMU)/computer.c
	mkdir -p $(BUILD_DIR)/$(BEMU)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(BEMU)/$(BEMU) $(SRC_DIR)/$(BEMU)/main.c \
		$(SRC_DIR)/$(BEMU)/computer.c

run-basm: basm
	$(BUILD_DIR)/$(BASM)/$(BASM) $(ARGS)

run-bemu: bemu
	$(BUILD_DIR)/$(BEMU)/$(BEMU) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

test: test-basm test-bemu

test-basm: $(BUILD_DIR)/$(TEST_DIR)/$(BASM)/test_basm
	@echo =================BASM TEST RESULTS=================
	-@$(BUILD_DIR)/$(TEST_DIR)/$(BASM)/test_basm
	@echo ==============BASM TEST RESULTS ENDED==============

test-bemu: $(BUILD_DIR)/$(TEST_DIR)/$(BEMU)/test_bemu
	@echo =================BEMU TEST RESULTS=================
	-@$(BUILD_DIR)/$(TEST_DIR)/$(BEMU)/test_bemu
	@echo ==============BEMU TEST RESULTS ENDED==============

$(BUILD_DIR)/$(TEST_DIR)/$(BASM)/test_basm: $(TEST_DIR)/$(BASM)/main.c
	mkdir -p $(BUILD_DIR)/$(TEST_DIR)/$(BASM)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TEST_DIR)/$(BASM)/test_basm $(TEST_DIR)/$(BASM)/main.c

$(BUILD_DIR)/$(TEST_DIR)/$(BEMU)/test_bemu: $(TEST_DIR)/$(BEMU)/main.c $(SRC_DIR)/$(BEMU)/computer.c
	mkdir -p $(BUILD_DIR)/$(TEST_DIR)/$(BEMU)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TEST_DIR)/$(BEMU)/test_bemu $(TEST_DIR)/$(BEMU)/main.c \
		$(SRC_DIR)/$(BEMU)/computer.c

.PHONY: clean basm bemu run-basm run-bemu test-basm test-bemu test
