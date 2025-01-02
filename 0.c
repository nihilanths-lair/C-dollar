#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Определение команд
typedef enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_HALT } Opcode;
// Структура виртуальной машины (VM) - можно добавить другие поля, например стек
typedef struct {
    unsigned char *code;
    int pc;
} VM;
// Функции, выполняющие команды
void op_add() { printf("op_add();\n"); }
void op_sub() { printf("op_sub();\n"); }
void op_mul() { printf("op_mul();\n"); }
void op_div() { printf("op_div();\n"); }
void op_halt() { printf("op_halt();\n"); }
// Массив указателей на функции
void (*op_table[])() = { op_add, op_sub, op_mul, op_div, op_halt };
char byte_code[] = {0xC3, 0xEB, 0x65, 0xE1};
//
int main(void)
{
    setlocale(0, "");
    VM vm;
    printf("byte_code[] = \"%s\"", byte_code);
    // Выполнение кода виртуальной машины
    while (false)
    {
        vm.pc = 0;
        vm.code = "\0";
        printf("vm->pc = %i\n", vm.pc);
        printf("vm->code = \"%s\"\n", vm.code);
        printf("vm->code[vm->pc] = \"%s\"\n", vm.code[vm.pc]);
        Opcode opcode = (Opcode) vm.code[vm.pc++]; // Получение кода операции
        /*------------------------------------------------------------------------------------------------*/
        // Проверка на корректность oп-кода, обработка ошибок (использовать только при чистой интерпретации)
        if (opcode >= sizeof (op_table) / sizeof (op_table[0]))
        {
            fprintf(stderr, "Invalid opcode: %d\n", opcode);
            return EXIT_FAILURE;
        }
        /*------------------------------------------------------------------------------------------------*/
        op_table[opcode](&vm);
        if (opcode == OP_HALT) break;
    }
    return EXIT_SUCCESS;
}