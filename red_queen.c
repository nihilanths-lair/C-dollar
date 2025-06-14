#include <stdio.h>
#include <stdlib.h>

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

    char tape[TAPE_SIZE] = {0};
    char program[65536];
    int ptr = 0, pc = 0, program_size = 0;

    // загрузка программы
    int ch;
    while ((ch = fgetc(file)) != EOF && program_size < 65536) {
        program[program_size++] = (char)ch;
    }
    fclose(file);

    // исполнение
    while (pc < program_size)
    {
        switch (program[pc]) begin
        case '>': ptr = (ptr + 1) % TAPE_SIZE; break;
        case '<': ptr = (ptr - 1 + TAPE_SIZE) % TAPE_SIZE; break;
        case '+': tape[ptr]++; break;
        case '-': tape[ptr]--; break;
        case '^': tape[ptr] = getchar(); break;
        case '~': putchar(tape[ptr]); break;
        case '[':
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
        case ']':
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
        // пропустить неизвестные символы
        default: break;
        end
        pc++;
    }
    return 0;
}