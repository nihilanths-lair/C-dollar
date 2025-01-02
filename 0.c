#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Считанный байт-код с файла
unsigned char bytecode[] = {0xB2, 0x20};

// Регистры виртуальной машины
typedef struct {
    //char *opcode;   // Instruction (op-code)
    //-[ 8-bits General Purpose Register ]-//
    unsigned char ah, al;
    unsigned char dl;
    short ip;         // Instruction Pointer
} Registers;

// Массив указателей на функции
void (*opcode_table[0xFF])(Registers *registers);

// Реализация функций, выполняющие команды
void mnemonic__mov_al(Registers *registers)
{
    printf("mnemonic__mov_al();\n");
    printf("registers->ip = %04X\n", registers->ip);
    registers->ip ++;
    printf("registers->ip = %04X\n", registers->ip);
    //opcode_table[bytecode[registers->ip]]();
    registers->al = bytecode[registers->ip];
    printf("registers->al = %02X\n", registers->al);
}
void mnemonic__mov_ah(Registers *registers)
{
    //printf("mnemonic__mov_ah();\n");
    printf("IP:[%04X]\n", registers->ip);
    registers->ip ++;
    printf("IP:[%04X]\n", registers->ip);
    registers->ah = bytecode[registers->ip];
    printf("AH:[%02X]\n", registers->ah);
}
void mnemonic__mov_dl(Registers *registers)
{
    //printf("mnemonic__mov_dl();\n");
    //printf("registers->ip = %04X\n", registers->ip);
    registers->ip ++;
    //printf("registers->ip = %04X\n", registers->ip);
    registers->dl = bytecode[registers->ip];
    //printf("registers->dl = %02X\n", registers->dl);
}
void mnemonic__unknown() { printf("mnemonic__unknown();\n"); } // отсутствует или свободный
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xB0; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    opcode_table[0xB1] = mnemonic__unknown;
    opcode_table[0xB2] = mnemonic__mov_dl;
    opcode_table[0xB3] = mnemonic__unknown;
    opcode_table[0xB4] = mnemonic__mov_ah;
    for (unsigned char i = 0xB5; i < 0xFF; i ++) opcode_table[i] = mnemonic__unknown;
    // Убедимся, что все указатели на функции проинициализированы
    //for (unsigned char i = 0x00; i < 0xFF; i ++) opcode_table[i](i);
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    registers.ip = -1;
    //printf("byte_code[] = \"%s\"\n", byte_code);
    // Выполнение кода виртуальной машины
    while (bytecode[++ registers.ip] != 0x00)
    {
        //printf("bytecode[0x%02X] = %02X|%03i\n", registers.ip, bytecode[registers.ip], bytecode[registers.ip]);
        // информация для отладки кода
        printf("- 8-bits registers -\n");
        printf("     H L\n");
        printf("AX:[%02X|%02X]\n", registers.ah, registers.al);
        printf("DX:[--|%02X]\n", registers.dl);
        opcode_table[bytecode[registers.ip]](&registers);
    }
    return EXIT_SUCCESS;
}