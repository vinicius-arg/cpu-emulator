#include "cpu_internals.h"
#include "mem.h"

bool is_revisitting_address(Disassembler* d, uint32_t next_pc) {
    return dss_get_reached_address(d) >= next_pc + 0x04;
}

void jump(Cpu* cpu, Instruction* instruction, Disassembler* d) {
    cpu->pc = (cpu->pc + ((int32_t)(int16_t)instruction->immediate)) & 0xFFFF;
}

void update_logger_enability(Cpu* cpu, Disassembler* d) {
    bool repeating = is_revisitting_address(d, cpu->pc);
    repeating ? dss_disable(d) : dss_enable(d);
}

void exec_jmp(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_pc = cpu->pc;
    
    jump(cpu, instruction, d);

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        apriori_pc,
        "JMP",
        cpu->pc+0x4
    );
    dss_add(d, dss_buffer);
    update_logger_enability(cpu, d);
}

void exec_jg(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_pc = cpu->pc;
    
    if (cpu->flags & (1 << G_SHIFT)) jump(cpu, instruction, d);

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        apriori_pc,
        "JG",
        cpu->pc+0x4
    );
    dss_add(d, dss_buffer);
    update_logger_enability(cpu, d);
}

void exec_jl(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_pc = cpu->pc;
    
    if (cpu->flags & (1 << L_SHIFT)) jump(cpu, instruction, d);

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        apriori_pc,
        "JL",
        cpu->pc+0x4
    );
    dss_add(d, dss_buffer);
    update_logger_enability(cpu, d);
}

void exec_je(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d) {
    char dss_buffer[LINE_MAX_SIZE];
    uint32_t apriori_pc = cpu->pc;
    
    if (cpu->flags & (1 << E_SHIFT)) jump(cpu, instruction, d);

    dss_format_cpu_output(
        d,
        dss_buffer,
        LINE_MAX_SIZE,
        instruction->opcode,
        apriori_pc,
        "JE",
        cpu->pc+0x4
    );
    dss_add(d, dss_buffer);
    update_logger_enability(cpu, d);
}
