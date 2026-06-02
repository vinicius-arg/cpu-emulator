#include "cpu_internals.h"
#include "mem.h"
#include "disassembler.h"

void exec_movi(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    
    // Extensão de sinal
    cpu->r[instruction->reg_x] = (int32_t)(int16_t)instruction->immediate;

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        cpu->pc,
        "MOV",
        instruction->reg_x,
        cpu->r[instruction->reg_x]
    );
    dss_add(d, dss_buffer);
}

void exec_mov(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    
    cpu->r[instruction->reg_x] = cpu->r[instruction->reg_y];

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        cpu->pc,
        "MOV",
        instruction->reg_x,
        instruction->reg_y,
        cpu->r[instruction->reg_y]
    );
    dss_add(d, dss_buffer);
}

void exec_mov_rmem(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    
    bool executing = true;
    uint32_t value = mem_read32(mem, cpu->r[instruction->reg_y], executing);
    cpu->r[instruction->reg_x] = value;

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        cpu->pc,
        "MOV",
        instruction->reg_x,
        cpu->r[instruction->reg_y]+0x00,
        cpu->r[instruction->reg_y]+0x01,
        cpu->r[instruction->reg_y]+0x02,
        cpu->r[instruction->reg_y]+0x03,
        value & 0xFF,
        (value & (0xFF<<8))>>8,
        (value & (0xFF<<16))>>16,
        (value & (0xFF<<24))>>24
    );
    dss_add(d, dss_buffer);
}

void exec_mov_memr(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    
    uint16_t address = (uint16_t)cpu->r[instruction->reg_x];
    uint32_t data = cpu->r[instruction->reg_y];
    mem_write32(mem, address, data);

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        cpu->pc,
        "MOV",
        address+0x00,
        address+0x01,
        address+0x02,
        address+0x03,
        instruction->reg_y,
        cpu->r[instruction->reg_y] & 0xFF,
        (cpu->r[instruction->reg_y] & (0xFF<<8))>>8,
        (cpu->r[instruction->reg_y] & (0xFF<<16))>>16,
        (cpu->r[instruction->reg_y] & (0xFF<<24))>>24
    );
    dss_add(d, dss_buffer);
}
