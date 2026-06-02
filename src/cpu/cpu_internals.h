#ifndef CPU_INTERNALS
#define CPU_INTERNALS

#include <stdint.h>
#include "mem.h"
#include "disassembler.h"

/** @brief Total architecture registers. */
#define REG_COUNT 16

/** @brief Total architecture opcodes. */
#define NUM_OPCODES 16

/** @brief Architecture defined exectution stop address. */
#define EXIT 0xF0F0

#define E_SHIFT 0
#define L_SHIFT 1
#define G_SHIFT 2

/** 
 * @brief Stores emulated CPU state.
 * 
 * Saves program counter, registers values and menage flags.
 * The registers are alocatted statically in a contigous space.
 */
typedef struct Cpu {
    uint32_t pc;            /** < Program counter */
    uint32_t r[REG_COUNT];  /** < Registers vector */
    uint8_t flags;          /** < Flags modified by cmp instruction */
} Cpu;

/**
 * @brief Represents an instructions decoded by processor.
 * 
 * Saves opcode and operands information.
 */
typedef struct Instruction {
    uint8_t opcode;
    uint8_t reg_x;
    uint8_t reg_y;
    uint16_t immediate;
} Instruction;

/** @brief Divides instructions into immediate, jump, mov, alu and shift types. */
typedef enum inst_type_t { IMMEDIATE, JUMP, MOV, ARITHMETIC_LOGIC, SHIFT, INVALID } inst_type_t;

/** @brief Possibly values for CPU flags. */
typedef enum flags_t { G, L, E } flags_t;

/** @brief Maps all opcodes to an instruction type. */
extern const inst_type_t instruction_table[16];

/** @brief Maps all opcodes to functions. */
extern const void* instruction_exec_table[16];

uint32_t fetch(Memory* mem, Cpu* cpu);
Instruction decode(uint32_t code);
void execute(Cpu* cpu, Memory* mem, Instruction instruction, Disassembler* d);

// Load Store Unit Instructions
void exec_movi(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_mov(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_mov_rmem(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_mov_memr(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);

// Arithmetic Logic Unit Instructions
void exec_cmp(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_add(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_sub(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_and(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_or(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_xor(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_sal(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_sar(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);

// Jumps
void exec_jmp(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_jg(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_jl(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);
void exec_je(Cpu* cpu, Memory* mem, Instruction* instruction, Disassembler* d);

#endif
