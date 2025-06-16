/* * * * * * * * * * * * * * * * * * * * * * * * * * *
*  Interpreter Red Queen v.:0.3                      *
*  Интерпретатор Красная Королева в.:0.3             *
*                                                    *
*  Эта версия поддерживает:                          *
*  1) Однострочные комментарии;                      *
*  2) Запись в ячейку памяти с другой ячейки памяти; *
*                                                    *
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Для отладки кода расскоментируйте строку ниже
//#define DEBUG_CODE

#define begin {
#define end }

#define TAPE_SIZE 30000

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <source.rq>\n", argv[0]);
        return 1;
    }

    // Проверка, что имя файла заканчивается на ".rq"
    char *filename = argv[1];
    size_t len = strlen(filename);
    if (len < 4 || strcmp(filename+len-3, ".rq") != 0)
    {
        fprintf(stderr, "Error file must have '.rq' extension.\n");
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

    // Загрузка программы с пропуском комментариев
    int ch;
    int in_comment = 0;
    while ((ch = fgetc(file)) != EOF && program_size < 65536)
    {
        if (in_comment)
        {
            if (ch == '\n' || ch == '\r')
            {
                in_comment = 0; // Конец комментария
                program[program_size++] = '\n'; // Можно сохранить перенос строки
            }
            continue;
        }
        if (ch == ';')
        {
            in_comment = 1; // Начало комментария
            continue;
        }
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
        case '+': // inc
        {
            tape[ptr]++;
            pc++;
            break;
        }
        case '-': // dec
        {
            tape[ptr]--;
            pc++;
            break;
        }
        case '=': // mov
        {
            tape[ptr] = program[++pc]; // запишем в текущую ячейку памяти след. байт
            // нет необходимости инкрементировать pc, сделали это выше
            break;
        }
        case '>':
        {
            ptr = (ptr+1) % TAPE_SIZE;
            pc++;
            break;
        }
        case '<':
        {
            ptr = (ptr-1 + TAPE_SIZE) % TAPE_SIZE;
            pc++;
            break;
        }
        case '^':
        {
            tape[ptr] = getchar();
            pc++;
            break;
        }
        case '~':
        {
            putchar(tape[ptr]);
            pc++;
            break;
        }
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
            pc++;
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
            pc++;
            break;
        }
        default: break; // прочие символы
        end
    }
    return 0;
}