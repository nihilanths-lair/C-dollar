/*****************************************
 *  Interpreter Red Queen v.:0.1
 *  ������������� ������� �������� �.:0.1
/*****************************************
/*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��� ������� ���� ���������������� ������ ����
//
#define DEBUG_CODE

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

    // ��������, ��� ��� ����� ������������� �� ".rq"
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

    char tape[TAPE_SIZE] = ""/* {'\0'} / {0} */; // ������ � �������, ������ (����)/������ (�����) ������
    char program[65536]  = ""/* {'\0'} / {0} */; // ����� ������� �������������� ��� ����� ��������� ����������� ����-���
    short ptr = 0; // ��������� �� ������ ������
    int pc = 0; // ������������� �������� (ASCII-��� �������)
    int program_size = 0;

    // �������� ���������
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

    // ����������
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
        case '>': ptr = (ptr+1) % TAPE_SIZE; break; // �������� ��������� �� ��� �����
        case '<': ptr = (ptr-1 + TAPE_SIZE) % TAPE_SIZE; break; // �������� ��������� �� ��� �����
        case '+': tape[ptr]++; break; // ����������� ������� �������� ������ �� �������
        case '-': tape[ptr]--; break; // ��������� ������� �������� ������ �� �������
        case '^': tape[ptr] = getchar(); break; // ��������� �������� (���� ������ - ASCII ���) � ������
        case '~': putchar(tape[ptr]); break; // �������� �������� (���� ������ - ASCII ���) �� ������
        case '[': // ������ �����
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
        case ']': // ����� �����
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
        default: break; // ������ �������
        end
        pc++;
    }
    return 0;
}