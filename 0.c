#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define runb {
#define endb }

// Считанный байт-код с файла
unsigned char bytecode[] = {0xB4, 0x4C, 0xCD, 0x21};

// Регистры виртуальной машины
typedef struct {
    //char *opcode;   // Instruction (op-code)
    //-[ 8-bits General Purpose Register ]-//
    short ax;
    unsigned char ah, al;
    unsigned char dl;
    short ip;         // Instruction Pointer
} Registers;

// Номер прерывания
//unsigned char interrupt_number;
// Массив указателей на функции
void (*opcode_table[0xFF])(Registers *registers) = {0};

// Реализация функций, выполняющие команды
void mnemonic__mov_al(Registers *registers)
{
    registers->ip ++;
    registers->ax = (registers->ax & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__mov_ah(Registers *registers)
{
    registers->ip ++;
    registers->ax = (registers->ax & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnemonic__mov_dl(Registers *registers)
{
    registers->ip ++;
    registers->dl = bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__int(Registers *registers)
{
    registers->ip ++;
    unsigned char interrupt_number = bytecode[registers->ip];
    // Обработка прерывания 21h
    if (interrupt_number == 0x21)
    {
        registers->ah = (registers->ax >> 8) & 0xFF;
        switch (registers->ah) runb
        case 0x02: printf("%c\n", registers->dl);
        case 0x4C: exit(EXIT_SUCCESS); break;
        endb
    }
}
// отсутствует или свободный
void mnemonic__unknown(Registers *registers) { registers->ip ++; }
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xFF; i++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    opcode_table[0xB2] = mnemonic__mov_dl;
    opcode_table[0xB4] = mnemonic__mov_ah;
    opcode_table[0xCD] = mnemonic__int;
    /*
    for (unsigned char i = 0x00; i < 0xB0; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    opcode_table[0xB1] = mnemonic__unknown;
    opcode_table[0xB2] = mnemonic__mov_dl;
    opcode_table[0xB3] = mnemonic__unknown;
    opcode_table[0xB4] = mnemonic__mov_ah;
    for (unsigned char i = 0xB5; i < 0xCD; i ++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xCD] = mnemonic__int;
    for (unsigned char i = 0xCE; i < 0xFF; i ++) opcode_table[i] = mnemonic__unknown;
    // Убедимся, что все указатели на функции проинициализированы
    for (unsigned char i = 0x00; i < 0xFF; i ++) opcode_table[i];
    */
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    registers.ax = 0x0000;
    //registers.ah = 0x00, registers.al = 0x00;
    registers.dl = 0x00;
    registers.ip = 0x00;
    //printf("byte_code[] = \"%s\"\n", byte_code);
    // информация для отладки кода
    printf("- 8-bits registers -\n");
    // Выполнение кода виртуальной машины
    while (true)
    {
        //printf("bytecode[0x%02X] = %02X|%03i\n", registers.ip, bytecode[registers.ip], bytecode[registers.ip]);
        // информация для отладки кода
        printf("     H L\n");
        printf("AX:[%02X|%02X]\n", (registers.ax >> 8) & 0xFF, registers.ax & 0xFF);
        printf("DX:[--|%02X]\n", registers.dl);
        printf("IP:[%04X]\n", registers.ip);
        opcode_table[bytecode[registers.ip]](&registers);
    }
    return EXIT_SUCCESS;
}