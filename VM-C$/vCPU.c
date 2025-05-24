/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.
#include <string.h>  // 

char bytecode[] =
{
    0x01, 5, // MOV GPR, 5 / gpr = 5;
    0x06, 1, // ADD GPR, 1 / gpr += 1; | 5+1=6
    0x07, 3, // SUB GPR, 3 / gpr -= 3; | 6-3=3
    0x04, 2, // MUL GPR, 2 / gpr *= 2; | 3*2=6
    0x00     // HLT
};
unsigned int  IPR = 0x00000000; // instruction pointer register / регистр указателя инструкций
unsigned char GPR = 0x00;       // general purpose register / регистр общего назначения
// [MOV GPR], [?] / где, [MNC + OP1] = 0x01
// GPR: [00] <- 0x00 или 00h
// MOV GPR, imm8   - поместить в регистр GPR непосредственное значение
// MOV GPR, mem8   - поместить в регистр GPR значение из памяти, обращение по имени (value = ptr_address)
// MOV GPR, [mem8] - поместить в регистр GPR значение из памяти, обращение по адресу (value = *ptr_value)
const unsigned char hex_to_string[][7+1] = {"HLT", "MOV GPR", "INT", "NOP", "MUL GPR", "DIV GPR", "ADD GPR", "SUB GPR"};
unsigned char hex_to_bin[256][8+1];
void generate_hex_to_bin_table()
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            hex_to_bin[i][7 - j] = (i & (1 << j)) ? '1' : '0';
        }
        hex_to_bin[i][8] = '\0';
    }
}
//#define HEX_TO_STRING(arg) hex_to_string[arg]
//#define HEX_TO_BIN(arg) hex_to_bin[arg]
//#define DEBUG_MODE
void Start_vCPU()
{
    void *instructions[] =
    {
        &&__HLT, // 0
        &&__MOV, // 1
        &&__INT, // 2
        &&__NOP, // 3
        &&__MUL, // 4
        &&__DIV, // 5
        &&__ADD, // 6
        &&__SUB  // 7
    };
    //while (true)
    //{
    #if defined DEBUG_MODE
    printf("\n");
    puts("# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #");
    generate_hex_to_bin_table();
    #endif
    EXECUTE:
    #if defined DEBUG_MODE
    printf("\n");
    printf("IPR [%03d|%02X|%s]\n", IPR, IPR, hex_to_bin[IPR]);
    printf("GPR [%03d|%02X|%s]\n", GPR, GPR, hex_to_bin[GPR]);
    #endif
    goto *(*(instructions + *(bytecode + IPR)));
    //--------------------------------------------------------------------------------
    __HLT: // 0x00 | Останавливает выполнение vCPU
    #if defined DEBUG_MODE
    printf("\n%s\t\t| %02X\n", hex_to_string[bytecode[IPR]], bytecode[IPR]);
    #endif
    IPR++;
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __MOV: // 0x01 | Пересылка данных
    IPR++;
    GPR = bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __INT: // 0x02 | Обращение к таблице векторных прерываний (IVT)
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __NOP: // 0x03 | Заглушка
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __MUL: // 0x04 | Умножение
    IPR++;
    GPR *= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __DIV: // 0x05 | Деление
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __ADD: // 0x06 | Сложение
    IPR++;
    GPR += bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 0x07 | Вычитание
    IPR++;
    GPR -= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IPR-1]], bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\nIPR [%03d|%02X|%s]\n", IPR, IPR, hex_to_bin[IPR]);
    printf("GPR [%03d|%02X|%s]\n\n", GPR, GPR, hex_to_bin[GPR]);
    puts("# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
    #endif
}
int main()
{
    setlocale(0, "");
    Start_vCPU();
    return 0;
}
// goto *instructions[bytecode[IP]];
// goto EXECUTE; / continue;
/*
void DebugCode(bool flag)
{
    if (flag)
    {
        flag = false;
        printf("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #");
    }
    else
    {
        printf("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
        flag = true;
    }
}
char *dbg(const char *strfmt, const char *flag)
{
    static char collection[10+1] = {'\0'};
    if (flag[0] == '+' && flag[1] == '<') strcat(collection, strfmt);
    else if (flag[0] == '<' && flag[1] == '<') strcpy(collection, strfmt);
    else if (flag[0] == '>' && flag[1] == '>') return collection;
    return "/!\\";
}
void dbg_SetBytecodeToCollection(bytecode, cell)
{
    static char collection[10+1] = {0};
    collection[cell] = bytecode;
}
void OutputDebugCode(){}
--
char *HexToString(char bytecode) // BytecodeToSymbolicView
{
    switch (bytecode){
    case 0x00: return "HLT";
    case 0x01: return "MOV GPR";
    case 0x02: return "INT";
    case 0x03: return "NOP";
    case 0x04: return "MUL GPR";
    case 0x05: return "DIV GPR";
    case 0x06: return "ADD GPR";
    case 0x07: return "SUB GPR";
    }
    return "/!\\";
}
char *HexToBin(char bytecode) // BytecodeToBin
{
    switch (bytecode){
    case 0x00: return "00000000";
    case 0x01: return "00000001";
    case 0x02: return "00000010";
    case 0x03: return "00000011";
    case 0x04: return "00000100";
    case 0x05: return "00000101";
    case 0x06: return "00000110";
    case 0x07: return "00000111";
    }
    return "/!\\";
}
char *HexToBin(char bytecode){
   return hex_to_bin[bytecode];
}
char *HexToString(char bytecode){
   return hex_to_string[bytecode];
}
const unsigned char hex_to_bin[256][8+1] =
{
    "00000000", "00000001", "00000010", "00000011",
    "00000100", "00000101", "00000110", "00000111",
    "00001000", "00001001", "00001010", "00001011",
    "00001100", "00001101", "00001110", "00001111",
    "00010000", "00010001", "00010010", "00010011",
    "00010100", "00010101", "00010110", "00010111",
    "00011000", "00011001", "00011010", "00011011",
    "00011100", "00011101", "00011110", "00011111",
    "00100000", "00100001", "00100010", "00100011",
    "00100100", "00100101", "00100110", "00100111",
    "00101000", "00101001", "00101010", "00101011",
    "00101100", "00101101", "00101110", "00101111",
    "00110000", "00110001", "00110010", "00110011",
    "00110100", "00110101", "00110110", "00110111",
    "00111000", "00111001", "00111010", "00111011",
    "00111100", "00111101", "00111110", "00111111",
    "01000000", "01000001", "01000010", "01000011",
    "01000100", "01000101", "01000110", "01000111",
    "01001000", "01001001", "01001010", "01001011",
    "01001100", "01001101", "01001110", "01001111",
    "01010000", "01010001", "01010010", "01010011",
    "01010100", "01010101", "01010110", "01010111",
    "01011000", "01011001", "01011010", "01011011",
    "01011100", "01011101", "01011110", "01011111",
    "01100000", "01100001", "01100010", "01100011",
    "01100100", "01100101", "01100110", "01100111",
    "01101000", "01101001", "01101010", "01101011",
    "01101100", "01101101", "01101110", "01101111",
    "01110000", "01110001", "01110010", "01110011",
    "01110100", "01110101", "01110110", "01110111",
    "01111000", "01111001", "01111010", "01111011",
    "01111100", "01111101", "01111110", "01111111",
    "10000000", "10000001", "10000010", "10000011",
    "10000100", "10000101", "10000110", "10000111",
    "10001000", "10001001", "10001010", "10001011",
    "10001100", "10001101", "10001110", "10001111",
    "10010000", "10010001", "10010010", "10010011",
    "10010100", "10010101", "10010110", "10010111",
    "10011000", "10011001", "10011010", "10011011",
    "10011100", "10011101", "10011110", "10011111",
    "10100000", "10100001", "10100010", "10100011",
    "10100100", "10100101", "10100110", "10100111",
    "10101000", "10101001", "10101010", "10101011",
    "10101100", "10101101", "10101110", "10101111",
    "10110000", "10110001", "10110010", "10110011",
    "10110100", "10110101", "10110110", "10110111",
    "10111000", "10111001", "10111010", "10111011",
    "10111100", "10111101", "10111110", "10111111",
    "11000000", "11000001", "11000010", "11000011",
    "11000100", "11000101", "11000110", "11000111",
    "11001000", "11001001", "11001010", "11001011",
    "11001100", "11001101", "11001110", "11001111",
    "11010000", "11010001", "11010010", "11010011",
    "11010100", "11010101", "11010110", "11010111",
    "11011000", "11011001", "11011010", "11011011",
    "11011100", "11011101", "11011110", "11011111",
    "11100000", "11100001", "11100010", "11100011",
    "11100100", "11100101", "11100110", "11100111",
    "11101000", "11101001", "11101010", "11101011",
    "11101100", "11101101", "11101110", "11101111",
    "11110000", "11110001", "11110010", "11110011",
    "11110100", "11110101", "11110110", "11110111",
    "11111000", "11111001", "11111010", "11111011",
    "11111100", "11111101", "11111110", "11111111"
};
*/