CC = gcc
CFLAGS = -Wall -Wextra -O3

BUILD_DIR = build

LIB_SRC = code/lib/shake128.c
MAIN_SRC = code/main.c

LIB_OBJ = $(BUILD_DIR)/shake128.o
MAIN_OBJ = $(BUILD_DIR)/main.o

TARGET_APP = shake128

all: $(TARGET_APP)

$(TARGET_APP): $(LIB_OBJ) $(MAIN_OBJ)
	@echo "Linking application..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful! Run with: ./$(TARGET_APP)"
	
$(BUILD_DIR)/shake128.o: $(LIB_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
clean:
	rm -rf $(BUILD_DIR) $(TARGET_APP) 
	@echo "Cleaned up."

.PHONY: all clean