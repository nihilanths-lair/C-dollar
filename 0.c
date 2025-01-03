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
    //char *opcode; // Instruction (op-code)
    //-[ 16/8-bits General Purpose Register ]-//
    short ax; unsigned char ah, al;
    short bx; unsigned char bh, bl;
    short cx; unsigned char ch, cl;
    short dx; unsigned char dh, dl;
    //-[ 16-bits General Purpose Register ]-//
    short ip; // Instruction Pointer
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
void mnemonic__mov_cl(Registers *registers)
{
    registers->ip ++;
    registers->cx = (registers->cx & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__mov_ah(Registers *registers)
{
    registers->ip ++;
    registers->ax = (registers->ax & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnemonic__mov_dh(Registers *registers)
{
    registers->ip ++;
    registers->dx = (registers->dx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnemonic__mov_ax(Registers *registers)
{
    registers->ip ++;
    registers->ax = bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__mov_cx(Registers *registers)
{
    registers->ip ++;
    registers->cx = bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__mov_dl(Registers *registers)
{
    registers->ip ++;
    registers->dl = bytecode[registers->ip];
    registers->ip ++;
}
void mnemonic__mov_bl(Registers *registers)
{
    registers->ip ++;
    registers->bl = bytecode[registers->ip];
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
void mnemonic__unknown(Registers *registers) { exit(EXIT_FAILURE); }
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xFF; i++) opcode_table[i] = mnemonic__unknown;
    opcode_table[0xB0] = mnemonic__mov_al;
    opcode_table[0xB1] = mnemonic__mov_cl;
    opcode_table[0xB2] = mnemonic__mov_dl;
    opcode_table[0xB3] = mnemonic__mov_bl;
    opcode_table[0xB4] = mnemonic__mov_ah;
    opcode_table[0xB6] = mnemonic__mov_dh;
    opcode_table[0xB8] = mnemonic__mov_ax;
    opcode_table[0xB9] = mnemonic__mov_cx;
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
    
    registers.ax = 0x0000; //registers.ah = 0x00, registers.al = 0x00;
    registers.bx = 0x0000; //registers.bh = 0x00, registers.bl = 0x00;
    registers.cx = 0x0000; //registers.ch = 0x00, registers.cl = 0x00;
    registers.dx = 0x0000; //registers.dh = 0x00, registers.dl = 0x00;
    registers.ip = 0x0000;
    
    // информация для отладки кода
    printf("- 16-bits registers -\n");
    // Выполнение кода виртуальной машины
    while (true)
    {
        //printf("bytecode[0x%02X] = %02X|%03i\n", registers.ip, bytecode[registers.ip], bytecode[registers.ip]);
        // информация для отладки кода
        printf("\n");
        printf("     H L\n");
        printf("AX:[%02X|%02X]\n", (registers.ax >> 8) & 0xFF, registers.ax & 0xFF);
        printf("BX:[%02X|%02X]\n", (registers.bx >> 8) & 0xFF, registers.bx & 0xFF);
        printf("CX:[%02X|%02X]\n", (registers.cx >> 8) & 0xFF, registers.cx & 0xFF);
        printf("DX:[%02X|%02X]\n", (registers.dx >> 8) & 0xFF, registers.dx & 0xFF);
        printf("IP:[%04X]\n", registers.ip);
        opcode_table[bytecode[registers.ip]](&registers);
    }
    return EXIT_SUCCESS;
}