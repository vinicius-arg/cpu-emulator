#include "disassembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

const char* format_table[16] = {
    [0x00] = "0x%04X->%s_R%hhd=0x%08X",
    [0x01] = "0x%04X->%s_R%hhd=R%hhd=0x%08X",
    [0x02] = "0x%04X->%s_R%hhd=MEM[0x%02X,0x%02X,0x%02X,0x%02X]=[0x%02X,0x%02X,0x%02X,0x%02X]",
    [0x03] = "0x%04X->%s_MEM[0x%02X,0x%02X,0x%02X,0x%02X]=R%hhd=[0x%02X,0x%02X,0x%02X,0x%02X]",
    [0x04] = "0x%04X->%s_R%hhd<=>R%hhd(G=%x,L=%x,E=%x)",
    [0x05] = "0x%04X->%s_0x%04X",
    [0x06] = "0x%04X->%s_0x%04X",
    [0x07] = "0x%04X->%s_0x%04X",
    [0x08] = "0x%04X->%s_0x%04X",
    [0x09] = "0x%04X->%s_R%hhd+=R%hhd=0x%08X+0x%08X=0x%08X",
    [0x0A] = "0x%04X->%s_R%hhd-=R%hhd=0x%08X-0x%08X=0x%08X",
    [0x0B] = "0x%04X->%s_R%hhd&=R%hhd=0x%08X&0x%08X=0x%08X",
    [0x0C] = "0x%04X->%s_R%hhd|=R%hhd=0x%08X|0x%08X=0x%08X",
    [0x0D] = "0x%04X->%s_R%hhd^=R%hhd=0x%08X^0x%08X=0x%08X",
    [0x0E] = "0x%04X->%s_R%hhd<<=%hd=0x%08X<<%hd=0x%08X",
    [0x0F] = "0x%04X->%s_R%hhd>>=%hd=0x%08X>>%hd=0x%08X"
};

typedef struct Buffer {
    char** content;
    int index;
    int size;
} Buffer;

/**
 * @brief Stores machine code assembly instructions.
 * 
 * This object stores decoded instructions into a buffer 
 * object, a counter for each of them and the output file pointer.
 * The disassembler can be disabled with helper functions.
 * 
 */
typedef struct Disassembler {
    Buffer buffer;               /** < Buffer object  */
    bool enabled;                /** < Enabling flag */
    int instruction_cnt[16];     /** < Instruction counter addressed by opcode */
    uint32_t reached_address;    /** < Higher reached address */ 
    FILE* output;                /** < Output file pointer */
} Disassembler;

Disassembler* dss_create(int size) {
    Disassembler* d = (Disassembler*)calloc(1, sizeof(Disassembler));

    d->output = fopen(OUTPUT_PATH, "w");
    d->enabled = true;

    if (!d->output) {
        printf("**Erro ao abrir arquivo de output (%s).\n", OUTPUT_PATH);
        return NULL;
    }

    d->buffer.size = size;

    if (!d) {
        printf("**Erro ao alocar estrutura do disassembler.\n");
        return NULL;
    }

    d->buffer.content = (char**)malloc(d->buffer.size*sizeof(char*));

    if (!d->buffer.content) {
        printf("**Erro ao alocar buffer do disassembler.\n");
        return NULL;
    }

    for (int i = 0; i < d->buffer.size; i++) {
        d->buffer.content[i] = (char*)malloc(LINE_MAX_SIZE);
        if (!d->buffer.content[i]) {
            printf("**Erro ao alocar buffer do disassembler (idx=%d).\n", i);
            return NULL;
        }
    }

    return d;
}

void dss_enable(Disassembler* d) {
    d->enabled = true;
}

void dss_disable(Disassembler* d) {
    d->enabled = false;
}

void dss_count_instruction(Disassembler* d, uint8_t opcode) {
    if (!d) return;
    
    if (opcode < 16) { 
        d->instruction_cnt[opcode]++;
    }
}

void dss_set_reached_address(Disassembler* d, uint32_t pc) {
    if (pc > d->reached_address) d->reached_address = pc;
}

uint32_t dss_get_reached_address(Disassembler* d) {
    return d->reached_address;
}

void dss_add(Disassembler* d, const char* content) {
    if (d->buffer.index == d->buffer.size-1) {
        printf("**Erro: Buffer do disassembler esta cheio!\n");
        exit(EXIT_FAILURE);
    }

    //printf("dss_enabled=%d\n", d->enabled);
    if (d->enabled) {
        snprintf(d->buffer.content[d->buffer.index], LINE_MAX_SIZE, "%s", content);
        d->buffer.index++;
    }
}

void dss_print(Disassembler* d, int index) {
    fprintf(d->output, "%s\n", d->buffer.content[index]);
}

void dss_print_all(Disassembler* d) {
    for (int i = 0; i < d->buffer.index; i++) {
        dss_print(d, i);
    }
}

const char* dss_get_instruction_format(uint8_t opcode) {
    if (opcode > 16) {
        return "0x%04X->[INVALID]";
    }

    return format_table[opcode];
}

void dss_format_cpu_output(Disassembler* d, char* buffer, int buffer_size, uint8_t opcode, ...) {
    if (d->enabled) {
        const char* format = dss_get_instruction_format(opcode);
    
        va_list args;
        va_start(args, opcode);
        vsnprintf(buffer, buffer_size, format, args);
        
        va_end(args);
    }
}

void dss_destroy(Disassembler* d) {
    for (int i = 0; i < d->buffer.size; i++) {
        free(d->buffer.content[i]);
    }

    free(d->buffer.content);
    fclose(d->output);
    free(d);
}