/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <string.h>  // Для работы со строками.

unsigned char bytecode[] =
{
    0x01,  5,     // MOV GPR, 5  / gpr = 5;
    0x06, 45,     // ADD GPR, 45 / gpr += 45; | 5 + 45 = 50
    0x07,  3,     // SUB GPR, 3  / gpr -= 3;  | 50 - 3 = 47
    0x04,  5,     // MUL GPR, 5  / gpr *= 5;  | 47 * 5 = 235
    0x08, 12,     // JMP 12
    0x03,         // NOP
    0x03,         // NOP
    0x0D, 2, 2,   // CMP 2 2     / 2 == 2
    0x03,         // NOP
    0x03,         // NOP
    0x00          // HLT
};
unsigned char IPR = 0x00; // instruction pointer register / регистр указателя инструкций
unsigned char GPR = 0x00; // general purpose register / регистр общего назначения
#include <stdbool.h> // Для использования логических типов: false/true.
bool ZFR = false; // zero flag register / регистр нулевого флага
//unsigned short DR = 0x00; // debug register / регистр отладки
// [MOV GPR], [?] / где, [MNC + OP1] = 0x01
// GPR: [00] <- 0x00 или 00h
// MOV GPR, imm8   - поместить в регистр GPR непосредственное значение
// MOV GPR, mem8   - поместить в регистр GPR значение из памяти, обращение по имени (value = ptr_address)
// MOV GPR, [mem8] - поместить в регистр GPR значение из памяти, обращение по адресу (value = *ptr_value)
const unsigned char hex_to_string[][7+1] =
{
    "HLT",     //  0
    "MOV GPR", //  1
    "INT",     //  2
    "NOP",     //  3
    "MUL GPR", //  4
    "DIV GPR", //  5
    "ADD GPR", //  6
    "SUB GPR", //  7
    "JMP",     //  8
    "CALL",    //  9
    "PUSH",    // 10
    "POP",     // 11
    "RET"      // 12
    "CMP"      // 13
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
void Start_vCPU()
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
    //while (true)
    //{
    #if defined DEBUG_MODE
    generate_hex_to_bin_table();
    puts("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #\n");
    #endif
    EXECUTE:
    #if defined DEBUG_MODE
    puts("------------");
    //puts("\n");
    printf("IPR [%03d|%02X|%s]\n", IPR, IPR, hex_to_bin[IPR]); // byte_to_binary(IPR, hex_to_bin);
    printf("GPR [%03d|%02X|%s]\n", GPR, GPR, hex_to_bin[GPR]); // byte_to_binary(GPR, hex_to_bin);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZFR);
    //puts("    Z");
    puts("------");
    #endif
    goto *(*(instructions + *(bytecode + IPR)));
    //--------------------------------------------------------------------------------
    __HLT: // 0 | Останавливает выполнение vCPU
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n", IPR, IPR, hex_to_string[bytecode[IPR]], bytecode[IPR]);
    #endif
    IPR++;
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __MOV: // 1 | Пересылка данных
    IPR++;
    GPR = bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __INT: // 2 | Обращение к таблице векторных прерываний (IVT)
    IPR++;
    switch (GPR){
    case 0x00: {}
    case 0xFF: {}
    }
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]); // ,
    #endif
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __NOP: // 3 | Заглушка
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n\n", IPR, IPR, hex_to_string[bytecode[IPR]], bytecode[IPR]);
    //printf("\n%03d=%02X | %s\t\t| %02X", IPR, IPR, hex_to_string[bytecode[IPR]], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __MUL: // 4 | Умножение
    IPR++;
    GPR *= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __DIV: // 5 | Деление
    IPR++;
    GPR /= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __ADD: // 6 | Сложение
    IPR++;
    GPR += bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 7 | Вычитание
    IPR++;
    GPR -= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __JMP: // 8 | Прыжок на метку (адрес)
    IPR++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n\n", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]); // ,
    //printf("\n%03d=%02X | %s %02X\t\t| %02X %02X", IPR-1, IPR-1, hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]); //
    #endif
    IPR = bytecode[IPR];
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CALL: //  9 | Вызов процедуры
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __PUSH: // 10 | Положить на стек
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __POP: // 11 | Снять со стека
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __RET: // 12 | Возврат из процедуры
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CMP: // 13 | Сравнение
    if (bytecode[++IPR] == bytecode[++IPR]) ZFR = true;
    else ZFR = false;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X %02X\t| %02X %02X %02X\n\n", IPR-2, IPR-2, hex_to_string[bytecode[IPR-2]], bytecode[IPR-1], bytecode[IPR], bytecode[IPR-2], bytecode[IPR-1], bytecode[IPR]); // ,
    //printf("\n%03d=%02X | %s %02X %02X\t\t| %02X %02X %02X", IPR-2, IPR-2, hex_to_string[bytecode[IPR-2]], bytecode[IPR-1], bytecode[IPR], bytecode[IPR-2], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\nIPR [%03d|%02X|%s]\n", IPR, IPR, hex_to_bin[IPR]);
    printf("GPR [%03d|%02X|%s]\n", GPR, GPR, hex_to_bin[GPR]);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZFR);
    //puts("    Z");
    puts("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
    #endif
    //puts("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
}
int main()
{
    setlocale(0, "");
    Start_vCPU();
    return 0;
}