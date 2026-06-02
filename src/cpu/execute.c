#include "cpu_internals.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mem.h"
#include "disassembler.h"

const void* instruction_exec_table[16] = {
    [0x00] = exec_movi,
    [0x01] = exec_mov,
    [0x02] = exec_mov_rmem,
    [0x03] = exec_mov_memr,
    [0x04] = exec_cmp,
    [0x05] = exec_jmp,
    [0x06] = exec_jg,
    [0x07] = exec_jl,
    [0x08] = exec_je,
    [0x09] = exec_add,
    [0x0A] = exec_sub,
    [0x0B] = exec_and,
    [0x0C] = exec_or,
    [0x0D] = exec_xor,
    [0x0E] = exec_sal,
    [0x0F] = exec_sar
};

void execute(Cpu* cpu, Memory* mem, Instruction instruction, Disassembler* d) {
    uint8_t opcode = instruction.opcode;
    dss_set_reached_address(d, cpu->pc);

    if (opcode <= 0x0F) {
        void (*handler)(Cpu*, Memory*, Instruction*, Disassembler*) = instruction_exec_table[opcode];
        handler(cpu, mem, &instruction, d);
    }
}
