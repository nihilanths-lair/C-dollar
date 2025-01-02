#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Считанный байт-код с файла
unsigned char bytecode[] = {0xB0, 0xEB, 0x65, 0xE1};

// Регистры виртуальной машины
typedef struct {
    //char *opcode; // Instruction (op-code)
    unsigned char al; // 8-bits General Purpose Register
    short ip;     // Instruction Pointer
} Registers;

// Массив указателей на функции
void (*opcode_table[0xFF])(Registers *registers);

// Объявления функций
// Реализация функций
void mnemonic__mov_al(Registers *registers)
{
    printf("mnemonic__mov_al();\n");
    printf("registers->ip = %04X\n", registers->ip);
    registers->ip ++;
    printf("registers->ip = %04X\n", registers->ip);
    //opcode_table[bytecode[registers->ip]]();
    registers->al = bytecode[registers->ip];
    printf("registers->al = %02X\n", registers->al);
}

// Определение команд
//typedef enum { OP_MOV, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_HALT } Opcode;

// Функции, выполняющие команды
void mnemonic__unknown() { printf("mnemonic__unknown();\n"); } // отсутствует или свободный

//
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xB0; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    for (unsigned char i = 0xB1; i < 0xFF; i ++) opcode_table[i] = mnemonic__unknown;
    // Убедимся, что все указатели на функции проинициализированы
    //for (unsigned char i = 0x00; i < 0xFF; i ++) opcode_table[i](i);
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    registers.ip = -1;
    //printf("byte_code[] = \"%s\"\n", byte_code);
    // Выполнение кода виртуальной машины
    while (bytecode[++ registers.ip] != 0x00)
    {
        //printf("bytecode[0x%02X] = %02X|%03i\n", registers.ip, bytecode[registers.ip], bytecode[registers.ip]);
        opcode_table[bytecode[registers.ip]](&registers);
    }
    Stop:
    return EXIT_SUCCESS;
}