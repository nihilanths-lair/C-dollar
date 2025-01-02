#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Определение команд
typedef enum { OP_MOV, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_HALT } Opcode;
// Структура виртуальной машины (VM) - можно добавить другие поля, например стек
typedef struct {
    unsigned char *code;
    short ip; // Instruction Pointer
} registers;
// Функции, выполняющие команды
void op_mov() { printf("op_mov()"); }
void op_add() { printf("op_add()"); }
void op_sub() { printf("op_sub();"); }
void op_mul() { printf("op_mul();"); }
void op_div() { printf("op_div();"); }
void op_halt() { printf("op_halt();"); }
// Массив указателей на функции
void (*opcode_table[])() = { op_mov, op_add, op_sub, op_mul, op_div, op_halt };
// Считанный байт-код с файла
char byte_code[] = {0xC3, 0xEB, 0x65, 0xE1};
//
int main(void)
{
    setlocale(0, "");
    // инициализация регистров и установка первоначальных значений
    registers registers;
    registers.ip = -1;
    //printf("byte_code[] = \"%s\"\n", byte_code);
    // Выполнение кода виртуальной машины
    while (byte_code[++ registers.ip] != 0x00)
    {
        printf("byte_code[%i] = %02X\n", registers.ip, byte_code[registers.ip] & 0xFF);
    }
    while (false)
    {
        registers.ip = 0;
        registers.code = "\0";
        printf("vm.pc = %i\n", registers.ip);
        printf("vm.code = \"%s\"\n", registers.code);
        printf("vm.code[vm.pc] = \"%s\"\n", registers.code[registers.ip]);
        Opcode opcode = (Opcode) registers.code[registers.ip++]; // Получение кода операции
        /*------------------------------------------------------------------------------------------------*/
        // Проверка на корректность oп-кода, обработка ошибок (использовать только при чистой интерпретации)
        if (opcode >= sizeof (opcode_table) / sizeof (opcode_table[0]))
        {
            fprintf(stderr, "Invalid opcode: %d\n", opcode);
            return EXIT_FAILURE;
        }
        /*------------------------------------------------------------------------------------------------*/
        opcode_table[opcode](&registers);
        if (opcode == OP_HALT) break;
    }
    return EXIT_SUCCESS;
}