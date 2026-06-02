#ifndef DISASSEMBLER
#define DISASSEMBLER

#include <stdint.h>

/** @brief Defines log max line size */
#define LINE_MAX_SIZE 256

/** @brief Defines output file name and dir */
#define OUTPUT_PATH "output/output.txt"

typedef struct Disassembler Disassembler;

extern const char* format_table[16];

/*
Desc.: Lida com alocações e atribuições iniciais do disassembler.

Parâmetros:
    (int) size: Tamanho/Quantidade de linhas salvas no buffer;

Return: Disassembler inicializado.
*/
Disassembler* dss_create(int size);


/*
Desc.: Setter para salvar último ponto acessado do programa.
       Salva o maior valor de PC já passado.
Parâmetros:
    (Disassembler*) d: Estrutura do disassembler;
    (uint32_t) pc: Contador de programa.
*/
void dss_set_reached_address(Disassembler* d, uint32_t pc);


/*
Desc.: Getter para acessar o último ponto acessado do programa.
Parâmetros:
    (Disassembler*) d: Estrutura do disassembler.
Return: Maior valor já registrado para o contador de programa.
*/
uint32_t dss_get_reached_address(Disassembler* d);


/*
Desc.: Habilita disassembler.
Parâmetros:
    (Disassembler*) d: Estrutura do disassembler.
*/
void dss_enable(Disassembler *d);


/*
Desc.: Desabilita disassembler.
Parâmetros:
    (Disassembler*) d: Estrutura do disassembler.
*/
void dss_disable(Disassembler* d);


/*
Desc.: Adiciona string no buffer do disassembler.

Parâmetros:
    (Disassembler*) d: Estrutura do disassembler;
    (char*) content: String a ser adicionada.
*/
void dss_add(Disassembler* d, const char* content);


/*
Desc.: Imprime string do buffer.

Parâmetros:
    (Disassembler*) d: Estrutura do disassembler;
    (int) index: Índice do buffer a ser impresso.
*/
void dss_print(Disassembler* d, int index);

/*
Desc.: Imprime todas as strings armazenadas no buffer.

Parâmetros:
    (Disassembler*) d: Estrutura do disassembler.
*/
void dss_print_all(Disassembler* d);

/*
Desc.: Formata parâmetros da cpu, preenchendo um buffer com a string resultante.

Parâmetros:
    (Disassembler*) d: Estrutura do disassembler;
    (char) buffer: Buffer que será preenchido;
    (int) buffer_size: Tamanho do buffer;
    (uin8_t) opcode: Código da instrução.
*/
void dss_format_cpu_output(Disassembler* d, char* buffer, int buffer_size, uint8_t opcode, ...);


void dss_count_instruction(Disassembler* d, uint8_t opcode);

/*
Desc.: Libera memória do disassembler.

Parâmetros:
    (Disassembler*) Estrutura do disassembler a destruir.
*/
void dss_destroy(Disassembler* d);

#endif