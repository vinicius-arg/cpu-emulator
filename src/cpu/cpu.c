#include "cpu.h"
#include "cpu_internals.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

Cpu* cpu_create() {
    Cpu* cpu = (Cpu*)calloc(1, sizeof(Cpu));

    if (!cpu) {
        printf("ERROR: CPU object pointer is NULL @ cpu/cpu_create.\n");
        return NULL;
    }

    return cpu;
}

void cpu_cycle(Cpu* cpu, Memory* mem, Disassembler* d) {
    uint32_t machine_code = fetch(mem, cpu);
    Instruction instruction = decode(machine_code);
    dss_count_instruction(d, instruction.opcode);
    execute(cpu, mem, instruction, d);
    
    cpu->pc += 4;
}

void cpu_destroy(Cpu* cpu) {
    free(cpu);
}

bool cpu_simulation_finished(Cpu* cpu) {
    return cpu->pc == EXIT;
}

void cpu_finishing_simulation_log(Cpu* cpu, Disassembler* d) {
    const char* finish_msg = "0xF0F0->EXIT";
    dss_add(d, finish_msg);
}
