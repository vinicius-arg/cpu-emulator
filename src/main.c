#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "mem.h"
#include "disassembler.h"

int main(int argc, char* argv[]) {
    // Arguments checking
    if (argc < 2) {
        printf("Usage: main /path/to/input\n");
        exit(EXIT_FAILURE);
    }

    // Creating main objects
    Cpu* cpu = cpu_create();
    Memory* mem = mem_create(256);
    Disassembler* d = dss_create(100);

    // Loading "input.txt" program into memory
    mem_load_program(mem, argv[1]);

    bool run = true;

    // Execution
    while (run) {
        cpu_cycle(cpu, mem, d);
        run = !cpu_simulation_finished(cpu);

        if (!run) cpu_finishing_simulation_log(cpu, d);
    }

    // Printing output
    dss_print_all(d);

    // Freeing objects
    cpu_destroy(cpu);
    mem_destroy(mem);
    dss_destroy(d);

    return 0;
}
