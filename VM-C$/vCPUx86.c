/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <string.h>  // Для работы со строками.

unsigned char opcode[] =
{
    0xB4,  9,     // MOV AH, 9
    0x06, 45,     // MOV DX, offset String / -offset = 109
    0x00          // HLT
};
/*------------------------------------------------------------*/
typedef unsigned short R16;
/*------------------------------------------------------------*/
R16 IP; // Instruction pointer / Указатель инструкций.
/*------------------------------------------------------------*/
R16 AX; // Accumulator / Аккумулятор
R16 BX; //        Base / База
R16 CX; //     Counter / Счётчик
R16 DX; //        Data / Данные
/*------------------------------------------------------------*/
R16 CS; //     Code segment / Сегмент кода
R16 DS; //     Data segment / Сегмент данных
R16 SS; //    Stack segment / Сегмент стека

R16 ES; // Extended segment / Расширенный сегмент
/*------------------------------------------------------------*/
R16 SP; // Stack pointer / Указатель стека.
R16 BP; // Base pointer / Указатель базы.
/*------------------------------------------------------------*/
R16 SI; // Source index / Индекс источника.
R16 DI; // Destination index / Индекс приёмника.
/*------------------------------------------------------------*/
#include <stdbool.h> // Для использования логических типов: false/true.
bool ZF = false; // zero flag register / регистр нулевого флага.
//unsigned short DR = 0x00; // debug register / регистр отладки.
// [MOV GP], [?] / где, [MNC + OP1] = 0x01
// GP: [00] <- 0x00 или 00h
// MOV GP, imm8   - поместить в регистр GP непосредственное значение
// MOV GP, mem8   - поместить в регистр GP значение из памяти, обращение по имени (value = ptr_address)
// MOV GP, [mem8] - поместить в регистр GP значение из памяти, обращение по адресу (value = *ptr_value)
const unsigned char hex_to_string[][6+1] =
{
    "HLT",    //  0
    "MOV GP", //  1
    "INT",    //  2
    "NOP",    //  3
    "MUL GP", //  4
    "DIV GP", //  5
    "ADD GP", //  6
    "SUB GP", //  7
    "JMP",    //  8
    "CALL",   //  9
    "PUSH",   // 10
    "POP",    // 11
    "RET"     // 12
    "CMP"     // 13
};
//#define HEX_TO_STRING(arg) hex_to_string[arg]
unsigned char hex_to_bin[256][8+1];
void generate_hex_to_bin_table()
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 7; j >= 0; j--) hex_to_bin[i][7-j] = (i & (1 << j)) ? '1' : '0';
        hex_to_bin[i][8] = '\0';
    }
}
//#define HEX_TO_BIN(arg) hex_to_bin[arg]
#define DEBUG_MODE
void Run_vCPUx86()
{
    void *instructions[] =
    {
        &&__HLT,  //  0
        &&__MOV,  //  1
        &&__INT,  //  2
        &&__NOP,  //  3
        &&__MUL,  //  4
        &&__DIV,  //  5
        &&__ADD,  //  6
        &&__SUB,  //  7
        &&__JMP,  //  8
        &&__CALL, //  9
        &&__PUSH, // 10
        &&__POP,  // 11
        &&__RET,  // 12
        &&__CMP   // 13
    };
    #if defined DEBUG_MODE
    generate_hex_to_bin_table();
    puts("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #\n");
    #endif
    EXECUTE:
    #if defined DEBUG_MODE
    puts("------------");
    printf("IP [%03d|%02X|%s]\n", IP, IP, hex_to_bin[IP]);
    //
    puts("\tH | L\n");
    printf("AX [%03d|%02X]\n", AX, AX);//, hex_to_bin[AX]);
    printf("BX [%03d|%02X]\n", BX, BX);//, hex_to_bin[BX]);
    printf("CX [%03d|%02X]\n", CX, CX);//, hex_to_bin[CX]);
    printf("DX [%03d|%02X]\n", DX, DX);//, hex_to_bin[DX]);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZF);
    //puts("    Z");
    puts("------");
    #endif
    goto *(*(instructions + *(opcode + IP))); // goto *instructions[opcode[IP]];
    //--------------------------------------------------------------------------------
    __HLT: // 0 | Останавливает выполнение vCPU
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n", IP, IP, hex_to_string[opcode[IP]], opcode[IP]);
    #endif
    IP++;
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __MOV: // 1 | Пересылка данных
    IP++;
    //GP = opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __INT: // 2 | Обращение к таблице векторных прерываний (IVT)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n", hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    void *functions[] =
    {
        &&__09h,
        &&__10h
    };
    goto *functions[opcode[IP]]; // goto *(*(functions + AX));
    // { 2
    __09h:
    //uch *ptr_str = &opcode[DX];
    //puts(ptr_str);
    goto EXECUTE;
    __10h:
    // } 2
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __NOP: // 3 | Заглушка
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n\n", IP, IP, hex_to_string[opcode[IP]], opcode[IP]);
    //printf("\n%03d=%02X | %s\t\t| %02X", IP, IP, hex_to_string[opcode[IP]], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __MUL: // 4 | Умножение
    IP++;
    //GP *= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __DIV: // 5 | Деление
    IP++;
    //GP /= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __ADD: // 6 | Сложение
    IP++;
    //GP += opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 7 | Вычитание
    IP++;
    //GP -= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __JMP: // 8 | Прыжок на метку (адрес)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X\t\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]); //
    #endif
    IP = opcode[IP];
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CALL: //  9 | Вызов процедуры
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __PUSH: // 10 | Положить на стек
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __POP: // 11 | Снять со стека
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __RET: // 12 | Возврат из процедуры
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CMP: // 13 | Сравнение
    if (opcode[++IP] == opcode[++IP]) ZF = true;
    else ZF = false;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X %02X\t| %02X %02X %02X\n\n", IP-2, IP-2, hex_to_string[opcode[IP-2]], opcode[IP-1], opcode[IP], opcode[IP-2], opcode[IP-1], opcode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X %02X\t\t| %02X %02X %02X", IP-2, IP-2, hex_to_string[opcode[IP-2]], opcode[IP-1], opcode[IP], opcode[IP-2], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\nIP [%03d|%02X|%s]\n", IP, IP, hex_to_bin[IP]);
    //printf("GP [%03d|%02X|%s]\n", GP, GP, hex_to_bin[GP]);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZF);
    //puts("    Z");
    puts("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
    #endif
    //puts("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
}