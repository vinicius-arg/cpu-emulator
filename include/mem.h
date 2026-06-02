#ifndef MEM
#define MEM

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct Memory Memory;

Memory* mem_create(uint32_t size);                       
void mem_destroy(Memory* mem);                           
void mem_load_program(Memory* mem, char* file_path); 
uint32_t mem_endianess_transform(uint32_t value, size_t size);
  uint8_t mem_read8(Memory* mem, uint16_t address);        
uint32_t mem_read32(Memory* mem, uint16_t address, bool executing);
void mem_write8(Memory* mem, uint16_t address, uint8_t data);
void mem_write32(Memory* mem, uint16_t address, uint32_t data);

#endif
