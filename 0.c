#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Структура виртуальной машины (VM) - можно добавить другие поля, например стек
typedef struct {
    char *opcode; // Instruction (op-code)
    short ip;     // Instruction Pointer
} Registers;

// Функции, выполняющие команды
void mnemonic__unknown() { printf("mnemonic__unknown();\n"); } // отсутствует или свободный
void mnemonic__mov_al() { printf("mnemonic__mov_al();\n"); }

// Массив указателей на функции
void (*opcode_table[0xFF])();

// Считанный байт-код с файла
unsigned char bytecode[] = {0xB0, 0xEB, 0x65, 0xE1};

int main(void)
{
    setlocale(0x00, "");
    
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xB0; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    for (unsigned char i = 0xB1; i < 0xFF; i ++) opcode_table[i] = mnemonic__unknown;
    
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    registers.ip = -1;
    
    // Выполнение кода виртуальной машины
    while (bytecode[++ registers.ip] != 0x00) { opcode_table[bytecode[registers.ip]](); }
    return EXIT_SUCCESS;
}
