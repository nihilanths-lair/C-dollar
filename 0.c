#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define runb {
#define endb }

// Считанный байт-код с файла
unsigned char bytecode[] =
{
    0xB4, 0x02, // MOV AH, 02h
    0xB2, 0x43, // MOV DL, 'C'
    0xCD, 0x21, // INT 21h
    0xB4, 0x02, // MOV AH, 02h
    0xB2, 0x24, // MOV DL, '$'
    0xCD, 0x21, // INT 21h
    0xB4, 0x4C, // MOV AH, 4Ch
    0xCD, 0x21  // INT 21h
};

/**
 * Эмуляция архитектуры процессора x86 (16 bits)
 */

// Регистры виртуальной машины
typedef struct {
  uint16_t ax;
  uint16_t bx;
  uint16_t cx;
  uint16_t dx;
  uint16_t ip;
} Registers;

// Прототипы функций для таблицы опкодов
void mnc__mov_al(Registers *registers);
void mnc__mov_cl(Registers *registers);
void mnc__mov_dl(Registers *registers);
void mnc__mov_bl(Registers *registers);
void mnc__mov_ah(Registers *registers);
void mnc__mov_ch(Registers *registers);
void mnc__mov_dh(Registers *registers);
void mnc__mov_bh(Registers *registers);
//
void mnc__mov_ax(Registers *registers);
void mnc__mov_cx(Registers *registers);
void mnc__mov_dx(Registers *registers);
void mnc__mov_bx(Registers *registers);
//
void mnc__int(Registers *registers);
void mnc__unknown(Registers *registers);
void mnc__dos_exit(Registers *registers);
void mnc__dos_print_char(Registers *registers);

// Массив указателей на функции
void (*opcode_table[0xFF])(Registers *registers) = {0};

// Реализация функций, выполняющие команды
void mnc__mov_al(Registers *registers) 
{
    registers->ip++;
    registers->ax = (registers->ax & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_cl(Registers *registers)
{
    registers->ip++;
    registers->cx = (registers->cx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_dl(Registers *registers)
{
    registers->ip++;
    registers->dx = (registers->dx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_bl(Registers *registers)
{
    registers->ip++;
    registers->bx = (registers->bx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_ah(Registers *registers)
{
    registers->ip++;
    registers->ax = (registers->ax & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_ch(Registers *registers)
{
    registers->ip++;
    registers->cx = (registers->cx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_dh(Registers *registers)
{
    registers->ip++;
    registers->dx = (registers->dx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_bh(Registers *registers)
{
    registers->ip++;
    registers->bx = (registers->bx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_ax(Registers *registers)
{
    registers->ip++;
    registers->ax = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_cx(Registers *registers)
{
    registers->ip++;
    registers->cx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_dx(Registers *registers)
{
    registers->ip++;
    registers->dx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_bx(Registers *registers)
{
    registers->ip++;
    registers->bx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__dos_exit(Registers *registers) { exit(EXIT_SUCCESS); }
void mnc__dos_print_char(Registers *registers) { putchar(registers->dx & 0xFF); }
void mnc__int(Registers *registers)
{
    registers->ip++;
    unsigned char int_number = bytecode[registers->ip];
    if (int_number == 0x21)
    {
        switch (registers->ax >> 8 & 0xFF) runb
        case 0x4C: // Exit
            mnc__dos_exit(registers);
            break;
        case 0x02: // Вывод символа
            mnc__dos_print_char(registers);
            break;
        default:
            printf("Unknown DOS interrupt function: AH=0x%02X\n", registers->ax >> 8 & 0xFF);
            break;
        endb
    } else { printf("Unhandled interrupt 0x%02X\n", int_number); }
    registers->ip++;
}
void mnc__unknown(Registers *registers)
{ 
    printf("mnc__unknown();\n");
    registers->ip++;
}
int main(void)
{
    setlocale(0x00, "");
    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xFF; i++) { opcode_table[i] = mnc__unknown; }

    opcode_table[0xB0] = mnc__mov_al;
    opcode_table[0xB1] = mnc__mov_cl;
    opcode_table[0xB2] = mnc__mov_dl;
    opcode_table[0xB3] = mnc__mov_bl;

    opcode_table[0xB4] = mnc__mov_ah;
    opcode_table[0xB5] = mnc__mov_ch;
    opcode_table[0xB6] = mnc__mov_dh;
    opcode_table[0xB7] = mnc__mov_bh;

    opcode_table[0xB8] = mnc__mov_ax;
    opcode_table[0xB9] = mnc__mov_cx;
    opcode_table[0xBA] = mnc__mov_dx;
    opcode_table[0xBB] = mnc__mov_bx;

    opcode_table[0xCD] = mnc__int;
   
    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    
    registers.ax = 0x0000;
    registers.bx = 0x0000;
    registers.cx = 0x0000;
    registers.dx = 0x0000;
    registers.ip = 0x0000;
    
    // информация для отладки кода
    //printf("\n- 16-bits registers -\n");

    // Выполнение кода виртуальной машины
    while (registers.ip < sizeof (bytecode))
    {
        // информация для отладки кода
        /*
        printf("\n");
        printf("     H L\n");
        printf("AX:[%02X|%02X]\n", (registers.ax >> 8) & 0xFF, registers.ax & 0xFF);
        printf("BX:[%02X|%02X]\n", (registers.bx >> 8) & 0xFF, registers.bx & 0xFF);
        printf("CX:[%02X|%02X]\n", (registers.cx >> 8) & 0xFF, registers.cx & 0xFF);
        printf("DX:[%02X|%02X]\n", (registers.dx >> 8) & 0xFF, registers.dx & 0xFF);
        printf("IP:[%04X]\n\n", registers.ip);
        */
        if (registers.ip >= sizeof(bytecode)) break;
        opcode_table[bytecode[registers.ip]](&registers);
    }
    //system("pause");
    return EXIT_SUCCESS;
}
/* -- EOF -- */
/*
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define runb {
#define endb }

// Считанный байт-код с файла
unsigned char bytecode[] =
{
    // Поместить код символа из таблицы ASCII в регистр DL
    0xB2, 0x43, // он же 'C'
    // Выводим символ на экран
    0xB4, 0x02,
    0xCD, 0x21,
    // Поместить код символа из таблицы ASCII в регистр DL
    0xB2, 0x24, // он же '$'
    // Выводим символ на экран
    0xB4, 0x02,
    0xCD, 0x21,
    // Завершение
    0xB4, 0x4C
};

// Поместить код символа из таблицы ASCII в регистр DL
// mov dl, 0x43 или 'C' | B2 43

// Выводим символ на экран
// mov ah, 02h | B4 02
// int 21h     | CD 21

// Поместить код символа из таблицы ASCII в регистр DL
// mov dl, 0x24 или '$' | B2 24

// Выводим символ на экран
// mov ah, 02h | B4 02
// int 21h     | CD 21

// Завершение
// mov ah, 4Ch | B4 4C

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
void mnc__mov_al(Registers *registers)
{
    registers->ip ++;
    registers->ax = (registers->ax & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_cl(Registers *registers)
{
    registers->ip ++;
    registers->cx = (registers->cx & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_dl(Registers *registers)
{
    registers->ip ++;
    registers->dx = (registers->dx & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_bl(Registers *registers)
{
    registers->ip ++;
    registers->bx = (registers->bx & 0xFF00) | bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_ah(Registers *registers)
{
    registers->ip ++;
    registers->ax = (registers->ax & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnc__mov_ch(Registers *registers)
{
    registers->ip ++;
    registers->cx = (registers->cx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnc__mov_dh(Registers *registers)
{
    registers->ip ++;
    registers->dx = (registers->dx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnc__mov_bh(Registers *registers)
{
    registers->ip ++;
    registers->bx = (registers->bx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip ++;
}
void mnc__mov_ax(Registers *registers)
{
    registers->ip ++;
    registers->ax = bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_cx(Registers *registers)
{
    registers->ip ++;
    registers->cx = bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_dx(Registers *registers)
{
    registers->ip ++;
    registers->dx = bytecode[registers->ip];
    registers->ip ++;
}
void mnc__mov_bx(Registers *registers)
{
    registers->ip ++;
    registers->bx = bytecode[registers->ip];
    registers->ip ++;
}
void mnc__int(Registers *registers)
{
    registers->ip ++;
    unsigned char interrupt_number = bytecode[registers->ip];

    // Обработка прерывания 21h
    if (interrupt_number == 0x21)
    {
        registers->ah = (registers->ax >> 8) & 0xFF;
        switch (registers->ah) runb
        case 0x02: printf("%c\n", registers->dx & 0xFF);
        case 0x4C: exit(EXIT_SUCCESS); break;
        endb
    }
}
// отсутствует или свободный
void mnc__unknown(Registers *registers) { exit(EXIT_FAILURE); }
int main(void)
{
    setlocale(0x00, "");

    // Инициализация указателей на функции
    for (unsigned char i = 0x00; i < 0xFF; i++) opcode_table[i] = mnc__unknown;

    opcode_table[0xB0] = mnc__mov_al;
    opcode_table[0xB1] = mnc__mov_cl;
    opcode_table[0xB2] = mnc__mov_dl;
    opcode_table[0xB3] = mnc__mov_bl;

    opcode_table[0xB4] = mnc__mov_ah;
    opcode_table[0xB5] = mnc__mov_ch;
    opcode_table[0xB6] = mnc__mov_dh;
    opcode_table[0xB7] = mnc__mov_bh;

    opcode_table[0xB8] = mnc__mov_ax;
    opcode_table[0xB9] = mnc__mov_cx;
    opcode_table[0xBA] = mnc__mov_dx;
    opcode_table[0xBB] = mnc__mov_bx;

    opcode_table[0xCD] = mnc__int;

    // Инициализация регистров и установка первоначальных значений
    Registers registers;
    
    registers.ax = 0x0000; //registers.ah = 0x00, registers.al = 0x00;
    registers.bx = 0x0000; //registers.bh = 0x00, registers.bl = 0x00;
    registers.cx = 0x0000; //registers.ch = 0x00, registers.cl = 0x00;
    registers.dx = 0x0000; //registers.dh = 0x00, registers.dl = 0x00;
    registers.ip = 0x0000;
    
    // информация для отладки кода
    printf("\n- 16-bits registers -\n");

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
        printf("IP:[%04X]\n\n", registers.ip);
        opcode_table[bytecode[registers.ip]](&registers);
    }
    return EXIT_SUCCESS;
}
*/