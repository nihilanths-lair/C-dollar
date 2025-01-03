/*! Windows-1251 encoding is used! */

#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define runb {
#define endb }

// Считанный байт-код с файла
unsigned char bytecode[0xFFFF];

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  Эмуляция архитектуры процессора x86 (16 bits)  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Регистры виртуальной машины
typedef struct {
  uint16_t ax;
  uint16_t bx;
  uint16_t cx;
  uint16_t dx;
  uint16_t ip;
} RegistersCPUx86;

// Прототипы функций для таблицы опкодов
void mnc__mov_al(RegistersCPUx86 *registers);
void mnc__mov_cl(RegistersCPUx86 *registers);
void mnc__mov_dl(RegistersCPUx86 *registers);
void mnc__mov_bl(RegistersCPUx86 *registers);
void mnc__mov_ah(RegistersCPUx86 *registers);
void mnc__mov_ch(RegistersCPUx86 *registers);
void mnc__mov_dh(RegistersCPUx86 *registers);
void mnc__mov_bh(RegistersCPUx86 *registers);
//
void mnc__mov_ax(RegistersCPUx86 *registers);
void mnc__mov_cx(RegistersCPUx86 *registers);
void mnc__mov_dx(RegistersCPUx86 *registers);
void mnc__mov_bx(RegistersCPUx86 *registers);
//
void mnc__int(RegistersCPUx86 *registers);
void mnc__unknown(RegistersCPUx86 *registers);
void mnc__dos_exit(RegistersCPUx86 *registers);
void mnc__dos_print_char(RegistersCPUx86 *registers);

// Массив указателей на функции
void (*opcode_table[0xFF])(RegistersCPUx86 *registers) = {0};

// Реализация функций, выполняющие команды
void mnc__mov_al(RegistersCPUx86 *registers) 
{
    registers->ip++;
    registers->ax = (registers->ax & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_cl(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->cx = (registers->cx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_dl(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->dx = (registers->dx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_bl(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->bx = (registers->bx & 0xFF00) | (bytecode[registers->ip]);
    registers->ip++;
}
void mnc__mov_ah(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->ax = (registers->ax & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_ch(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->cx = (registers->cx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_dh(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->dx = (registers->dx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_bh(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->bx = (registers->bx & 0x00FF) | (bytecode[registers->ip] << 8);
    registers->ip++;
}
void mnc__mov_ax(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->ax = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_cx(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->cx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_dx(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->dx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__mov_bx(RegistersCPUx86 *registers)
{
    registers->ip++;
    registers->bx = (bytecode[registers->ip]) | (bytecode[registers->ip + 1] << 8);
    registers->ip += 2;
}
void mnc__dos_exit(RegistersCPUx86 *registers) { exit(EXIT_SUCCESS); }
void mnc__dos_print_char(RegistersCPUx86 *registers) { putchar(registers->dx & 0xFF); }
void mnc__int(RegistersCPUx86 *registers)
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
void mnc__unknown(RegistersCPUx86 *registers)
{ 
    printf("mnc__unknown();\n");
    registers->ip++;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  Свой собственный набор команд и регистров виртуальной машины  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// -- В разработке -- //
// Регистры виртуальной машины
typedef struct {
  uint16_t r16b;
  uint16_t ip;
} Registers;
// -- В разработке -- //
//
int main(void)
{
    setlocale(0, "");
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
    RegistersCPUx86 registers;
    
    registers.ax = 0x0000;
    registers.bx = 0x0000;
    registers.cx = 0x0000;
    registers.dx = 0x0000;
    registers.ip = 0x0000;
    
    FILE* h = fopen("0.bccdlr", "rb");
	if (h == NULL) return EXIT_FAILURE;
	
	fseek(h, 0, SEEK_END);
	int fsize = ftell(h);
	fseek(h, 0, SEEK_SET);
	
	int i = -1;
	while (++i < fsize)
    {
        bytecode[i] = fgetc(h);
        printf("%02X ", bytecode[i]);
    }
    printf("\nКол-во байт в файле: %lld.\n", fsize);
	fclose(h);

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
    return EXIT_SUCCESS;
}