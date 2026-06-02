#ifndef CPU
#define CPU

#include <stdint.h>
#include "mem.h"
#include "disassembler.h"

typedef struct Cpu Cpu;

Cpu* cpu_create();
void cpu_destroy(Cpu* cpu);
void cpu_cycle(Cpu* cpu, Memory* mem, Disassembler* d);
bool cpu_simulation_finished(Cpu* cpu);
void cpu_finishing_simulation_log(Cpu* cpu, Disassembler* d);

#endif
