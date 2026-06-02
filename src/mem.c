#include "mem.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

/**
 * @brief Emulator main memory object
 * 
 * Memory is stored as a contigous array of bytes and
 * may be accessed by helper functions.
 */
typedef struct Memory {
    uint8_t* mem8; /** < Memory buffer  */
    uint32_t size; /** < Memory size in bytes */
    bool loaded;   /** < Program loaded flag */
} Memory;

Memory* mem_create(uint32_t size) {
    Memory* mem = (Memory*)calloc(1, sizeof(Memory));

    if (!mem) {
        printf("ERROR: Memory object pointer is NULL @ mem/mem_create.\n");
        return NULL;
    }

    mem->size = size;
    mem->mem8 = (uint8_t*)calloc(mem->size, sizeof(uint8_t));
    mem->loaded = false;

    if (!mem->mem8) {
        printf("ERROR: Memory buffer is NULL @ mem/mem_create.\n");
        return NULL;
    }

    return mem;
}

void mem_destroy(Memory* mem) {
    if (!mem) return;
    free(mem->mem8);
    free(mem);
}

void mem_load_program(Memory* mem, char* input_path) {
    FILE* input = fopen(input_path, "r");

    if (!mem) {
        printf("ERROR: Loading bytes into a non-alocated memory @ mem/mem_load_program.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; !feof(input); i++) {
        if (fscanf(input, "%hhx ", &mem->mem8[i]) != 1)
            printf("WARN: Program loading error @ mem/mem_load_program.\n");
    }

    mem->loaded = true;
}


uint32_t mem_endianess_transform(uint32_t value, size_t size) {
    uint32_t* buffer = (uint32_t*)calloc(4, size);
    uint32_t content = 0x0;

    for (uint8_t i = 0; i < size; i++) {
        buffer[i] = (value & (0XFF << 8*i)) >> 8;
        content |= (buffer[i] << 8*(size-1-i));
    }

    return content;
}


uint8_t mem_read8(Memory* mem, uint16_t address) {
    if (!mem || !mem->loaded) {
        printf("ERROR: Loading bytes into a non-alocated memory @ mem/mem_read8.\n");
        exit(EXIT_FAILURE);
    }

    return mem->mem8[address];
}

uint32_t mem_read32(Memory* mem, uint16_t address, bool executing) {
    uint8_t buffer[4] = {0};
    uint32_t content = 0x0;

    for (uint16_t i = 0; i < 4; i++) {
        buffer[i] = mem_read8(mem, address + i);
        content |= buffer[i] << (executing ? 8*i : 8*(3-i));
    }

    return content;
}


void mem_write8(Memory* mem, uint16_t address, uint8_t data) {
    mem->mem8[address] = data;
}

void mem_write32(Memory* mem, uint16_t address, uint32_t data) {
    uint8_t buffer[4] = {0};

    for (uint8_t i = 0; i < 4; i++) {
        buffer[i] = (uint8_t)(data & (0xFF << 8*i));
        mem_write8(mem, address + i, buffer[i]);
    }
}
