#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Определение команд
typedef enum { OP_MOV, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_HALT } Opcode;
// Структура виртуальной машины (VM) - можно добавить другие поля, например стек
typedef struct {
    char *opcode; // Instruction (op-code)
    short ip;     // Instruction Pointer
} Registers;
// Функции, выполняющие команды
void mnemonic__unknown() { printf("mnemonic__unknown();\n"); } // отсутствует или свободный
void mnemonic__mov_al() { printf("mnemonic__mov_al();\n"); }
//void mnemonic_add() { printf("op_add()"); }
//void mnemonic_sub() { printf("op_sub();"); }
//void mnemonic_mul() { printf("op_mul();"); }
//void mnemonic_div() { printf("op_div();"); }
//void mnemonic_halt() { printf("op_halt();"); }
// Массив указателей на функции
void (*opcode_table[0xFF])();
// Считанный байт-код с файла
char byte_code[] = {0xB0, 0xEB, 0x65, 0xE1};
//
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xB0; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    for (unsigned char i = 0xB1; i < 0xFF; i ++) opcode_table[i] = mnemonic__unknown;
    // Проверим, что все указатели на функции проинициализированы
    for (unsigned char i = 0x00; i < 0xFF; i ++) opcode_table[i]();
    goto Stop;
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    registers.ip = -1;
    //printf("byte_code[] = \"%s\"\n", byte_code);
    // Выполнение кода виртуальной машины
    while (byte_code[++ registers.ip] != 0x00)
    {
        printf("byte_code[%i] = %02X\n", registers.ip, byte_code[registers.ip] & 0xFF);
    }
    Stop:
    return EXIT_SUCCESS;
}
