BUILD_DIR = build
OUTPUT_DIR = output

TARGET = $(BUILD_DIR)/cpu_emulator.elf

FLAGS = -Wall -O3 -g -Iinclude
UNIT_FLAGS = -Isrc/cpu

OBJECTS = main.o cpu.o mem.o dss.o fetch.o decode.o execute.o alu.o lsu.o jump.o

all: $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OUTPUT_DIR)
	gcc $(OBJECTS) -o $(TARGET)
	mv $(OBJECTS) $(BUILD_DIR)

main.o: src/main.c
	gcc $(FLAGS) -c src/main.c -o main.o

cpu.o: src/cpu/cpu.c
	gcc $(FLAGS) -c src/cpu/cpu.c -o cpu.o

mem.o: src/mem.c
	gcc $(FLAGS) -c src/mem.c -o mem.o

dss.o: src/disassembler.c
	gcc $(FLAGS) -c src/disassembler.c -o dss.o

fetch.o: src/cpu/fetch.c
	gcc $(FLAGS) -c src/cpu/fetch.c -o fetch.o

decode.o: src/cpu/decode.c
	gcc $(FLAGS) -c src/cpu/decode.c -o decode.o

execute.o: src/cpu/execute.c
	gcc $(FLAGS) -c src/cpu/execute.c -o execute.o

alu.o: src/cpu/units/alu.c
	gcc $(FLAGS) $(UNIT_FLAGS) -c src/cpu/units/alu.c -o alu.o

lsu.o: src/cpu/units/lsu.c
	gcc $(FLAGS) $(UNIT_FLAGS) -c src/cpu/units/lsu.c -o lsu.o

jump.o: src/cpu/units/jump.c
	gcc $(FLAGS) $(UNIT_FLAGS) -c src/cpu/units/jump.c -o jump.o

clean:
	rm -rf $(BUILD_DIR) $(OBJECTS)

run:
	./$(TARGET) roms/input.txt $(OUTPUT_DIR)/output.txt

