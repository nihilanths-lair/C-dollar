/*****************************************
 *  Interpreter Red Queen v.:0.1
 *  Интерпретатор Красная Королева в.:0.1
/*****************************************
/*/
#include <stdio.h>
#include <stdlib.h>

// Расскоментируйте для отладки кода
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
    int ptr = 0; // Указатель на ячейки памяти
    int pc = 0; // Установленное значение (ASCII-код символа)
    int program_size = 0;

    // Загрузка программы
    int ch;
    while ((ch = fgetc(file)) != EOF && program_size < 65536) {
        program[program_size++] = (char)ch;
    }
    fclose(file);
    //#
    #if defined DEBUG_CODE
    ///program[program_size] = '\0';
    //printf("# program[] = \"%s\"", program);
    #endif
    //#

    // Исполнение
    while (pc < program_size)
    {
        //#
        #if defined DEBUG_CODE
        printf("\n# ptr = (%02X %02X | %03d %03d) = %d\n", ptr << 8, ptr && 0xFF, ptr << 8, ptr && 0xFF, ptr);
        switch (pc) begin
        case '\0': printf("# pc = %03d | %s\n", pc, "\\0");
        case '\r': printf("# pc = %03d | %s\n", pc, "\\r");
        case '\n': printf("# pc = %03d | %s\n", pc, "\\n");
        default: printf("# pc = %03d | %c\n", pc, pc);
        end
        switch (program[program_size]) begin
        case '\0': printf("# program[%d] = '\\0'\n", program_size);
        case '\r': printf("# program[%d] = '\\r'\n", program_size);
        case '\n': printf("# program[%d] = '\\n'\n", program_size);
        default: printf("# program[%d] = %c\n", program_size, program[program_size]);
        end
        #endif
        //#
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
        default: break; // пропустить неизвестные символы
        end
        pc++;
    }
    return 0;
}