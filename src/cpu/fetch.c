#include "cpu_internals.h"

#include <stdio.h>

#include "mem.h"

uint32_t fetch(Memory* mem, Cpu* cpu) {
    bool executing = true;
    uint32_t instruction =  mem_read32(mem, cpu->pc, executing);

    // printf("fetch: 0x%08X\n", instruction);

    return instruction;
}
