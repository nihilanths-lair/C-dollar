/*****************************************
 *  Interpreter Red Queen v.:0.1
 *  Интерпретатор Красная Королева в.:0.1
/*****************************************
/*/
#include <stdio.h>
#include <stdlib.h>

// Для отладки кода расскоментируйте строку ниже
//#define DEBUG_CODE

#define begin {
#define end }

#define TAPE_SIZE 30000

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <source.dsl>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Error opening source file");
        return 1;
    }

    char tape[TAPE_SIZE] = ""/* {'\0'} / {0} */; // Работа с памятью, запись (ввод)/чтение (вывод) данных
    char program[65536]  = ""/* {'\0'} / {0} */; // После запуска интерпретатора тут будет находится загружаемый байт-код
    short ptr = 0; // Указатель на ячейки памяти
    int pc = 0; // Установленное значение (ASCII-код символа)
    int program_size = 0;

    // Загрузка программы
    int ch;
    while ((ch = fgetc(file)) != EOF && program_size < 65536) {
        program[program_size++] = (char)ch;
    }
    fclose(file);
    //##
    #if defined DEBUG_CODE
    FILE *debug_log = fopen("debug_log.txt", "w");
    #endif
    //##

    // Исполнение
    while (pc < program_size)
    {
        //##
        #if defined DEBUG_CODE
        fprintf(debug_log, "# ptr = (%02X %02X | %03d %03d) = %d\n", (ptr << 16) && 0xFF, ptr && 0xFF, (ptr << 16) && 0xFF, ptr && 0xFF, ptr);
        switch (program[pc]) begin
        case '\0': fprintf(debug_log, "# program[pc:%d] = '\\0'\n\n", pc); break;
        case '\r': fprintf(debug_log, "# program[pc:%d] = '\\r'\n\n", pc); break;
        case '\n': fprintf(debug_log, "# program[pc:%d] = '\\n'\n\n", pc); break;
        default: fprintf(debug_log, "# program[pc:%d] = '%c'\n\n", pc, program[pc]);
        end
        #endif
        //##
        switch (program[pc]) begin
        case '>': ptr = (ptr+1) % TAPE_SIZE; break; // сместить указатель на шаг вперёд
        case '<': ptr = (ptr-1 + TAPE_SIZE) % TAPE_SIZE; break; // сместить указатель на шаг назад
        case '+': tape[ptr]++; break; // увеличивает текущее значение ячейки на единицу
        case '-': tape[ptr]--; break; // уменьшает текущее значение ячейки на единицу
        case '^': tape[ptr] = getchar(); break; // считывает значение (один символ - ASCII код) в ячейку
        case '~': putchar(tape[ptr]); break; // получает значение (один символ - ASCII код) из ячейки
        case '[': // начало цикла
        {
            if (!tape[ptr])
            {
                int loop = 1;
                while (loop && ++pc < program_size)
                {
                    if (program[pc] == '[') loop++;
                    else if (program[pc] == ']') loop--;
                }
            }
            break;
        }
        case ']': // конец цикла
        {
            if (tape[ptr])
            {
                int loop = 1;
                while (loop && --pc >= 0)
                {
                    if (program[pc] == ']') loop++;
                    else if (program[pc] == '[') loop--;
                }
            }
            break;
        }
        default: break; // прочие символы
        end
        pc++;
    }
    return 0;
}