#include "cpu_internals.h"
#include "mem.h"
#include "disassembler.h"

void cpu_update_flags(Cpu* cpu, flags_t flag) {
    cpu->flags = 0x00;
    switch (flag) {
        case E: cpu->flags |= (1 << E_SHIFT); break; // Zero flag (E)
        case L: cpu->flags |= (1 << L_SHIFT); break; // Less flag (L)
        case G: cpu->flags |= (1 << G_SHIFT); break; // Greater flag (G)
    }
}

uint8_t cpu_get_flag(Cpu* cpu, flags_t flag) {
    switch (flag) {
        case E: return (cpu->flags & (1 << E_SHIFT)) >> E_SHIFT; // Zero flag (E)
        case L: return (cpu->flags & (1 << L_SHIFT)) >> L_SHIFT; // Less flag (L)
        case G: return (cpu->flags & (1 << G_SHIFT)) >> G_SHIFT; // Greater flag (G)
    }

    return 0;
}

void exec_cmp(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    
    Instruction* i = instruction;
    flags_t result = ((int32_t)cpu->r[i->reg_x] > (int32_t)cpu->r[i->reg_y] ? G
        : ((int32_t)cpu->r[i->reg_x] < (int32_t)cpu->r[i->reg_y] ? L : E));

    cpu_update_flags(cpu, result);

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        i->opcode,
        cpu->pc,
        "CMP",
        i->reg_x,
        i->reg_y,
        cpu_get_flag(cpu, G),
        cpu_get_flag(cpu, L),
        cpu_get_flag(cpu, E)
    );
    dss_add(d, dss_buffer);
}

void exec_add(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];

    uint32_t add = cpu->r[instruction->reg_x] + cpu->r[instruction->reg_y];
    cpu->r[instruction->reg_x] = (uint32_t)add;

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "ADD",
        instruction->reg_x,
        instruction->reg_y,
        apriori_rx,
        cpu->r[instruction->reg_y],
        add
    );
    dss_add(d, dss_buffer);
}

void exec_sub(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    
    uint32_t sub = cpu->r[instruction->reg_x] - cpu->r[instruction->reg_y];
    cpu->r[instruction->reg_x] = (uint32_t)sub;

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "SUB",
        instruction->reg_x,
        instruction->reg_y,
        apriori_rx,
        cpu->r[instruction->reg_y],
        sub
    );
    dss_add(d, dss_buffer);
}

void exec_and(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    
    cpu->r[instruction->reg_x] &= cpu->r[instruction->reg_y];

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "AND",
        instruction->reg_x,
        instruction->reg_y,
        apriori_rx,
        cpu->r[instruction->reg_y],
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}

void exec_or(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    
    cpu->r[instruction->reg_x] |= cpu->r[instruction->reg_y];

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "OR",
        instruction->reg_x,
        instruction->reg_y,
        apriori_rx,
        cpu->r[instruction->reg_y],
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}

void exec_xor(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    uint32_t apriori_ry = cpu->r[instruction->reg_y];
    
    cpu->r[instruction->reg_x] ^= cpu->r[instruction->reg_y];

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "XOR",
        instruction->reg_x,
        instruction->reg_y,
        apriori_rx,
        apriori_ry,
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}

void exec_sal(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    
    cpu->r[instruction->reg_x] <<= instruction->immediate;

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "SAL",
        instruction->reg_x,
        instruction->immediate,
        apriori_rx,
        instruction->immediate,
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}

void exec_sar(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_rx = cpu->r[instruction->reg_x];
    
    cpu->r[instruction->reg_x] >>= instruction->immediate;

    dss_format_cpu_output(
        d,
        dss_buffer, 
        LINE_MAX_SIZE, 
        instruction->opcode,
        cpu->pc,
        "SAR",
        instruction->reg_x,
        instruction->immediate,
        apriori_rx,
        instruction->immediate,
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}
