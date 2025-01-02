#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Определение команд
typedef enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_HALT } Opcode;
// Структура виртуальной машины (VM) - можно добавить другие поля, например стек
typedef struct {
    unsigned char* code;
    int pc;
} VM;
// Функции, выполняющие команды
void op_add(VM *vm) { printf("op_add();\n"); }
void op_sub(VM *vm) { printf("op_sub();\n"); }
void op_mul(VM *vm) { printf("op_mul();\n"); }
void op_div(VM *vm) { printf("op_div();\n"); }
void op_halt(VM *vm) { printf("op_halt();\n"); }
// Таблица переходов
void (*op_table[])(VM*) = { op_add, op_sub, op_mul, op_div, op_halt };
// Выполнение кода виртуальной машины
int execute(VM* vm)
{
    while (true)
    {
        vm->pc = 0;
        vm->code = "\0";
        printf("vm->pc = %i\n", vm->pc);
        printf("vm->code = \"%s\"\n", vm->code);
        printf("vm->code[vm->pc] = \"%s\"\n", vm->code[vm->pc]);
        Opcode opcode = (Opcode) vm->code[vm->pc++]; // Получение кода операции
        // Проверка на корректность oп-кода, обработка ошибок
        if (opcode >= sizeof (op_table) / sizeof (op_table[0]))
        {
            fprintf(stderr, "Invalid opcode: %d\n", opcode);
            return EXIT_FAILURE; // или другое значение, сигнализирующее об ошибке
        }
        op_table[opcode](vm);  // Вызов соответствующей функции
        if (opcode == OP_HALT) break;
    }
    return EXIT_SUCCESS;
}
int main(void)
{
    setlocale(0, "");
    // Инициализация VM (загрузка кода и т.д.)
    VM vm;
    // ...
    execute(&vm);
    return 0;
}