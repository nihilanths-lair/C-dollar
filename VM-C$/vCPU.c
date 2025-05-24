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
    0x01, 0x05, // MOV GPR, 5
    0x06, 0x01, // ADD GPR, 1
    0x07, 0x03, // SUB GPR, 3
    0x00        // HLT
};
unsigned int  IPR = 0x0; // instruction pointer register / регистр указателя инструкций
unsigned char GPR = 0x0; // general purpose register / регистр общего назначения
// [MOV GPR], [?] / где, [MNC + OP1] = 0x01
// GPR: [00] <- 0x00 или 00h
// MOV GPR, imm8   - поместить в регистр GPR непосредственное значение
// MOV GPR, mem8   - поместить в регистр GPR значение из памяти, обращение по имени (value = ptr_address)
// MOV GPR, [mem8] - поместить в регистр GPR значение из памяти, обращение по адресу (value = *ptr_value)
char hex_to_string[][7+1] = {"HLT", "MOV GPR", "INT", "NOP", "MUL GPR", "DIV GPR", "ADD GPR", "SUB GPR"};
char hex_to_bin[][8+1] =
{
    "00000000",
    "00000001",
    "00000010",
    "00000011",
    "00000100",
    "00000101",
    "00000110",
    "00000111"
};
char *HexToBin(char bytecode){
   return hex_to_bin[bytecode];
}
char *HexToString(char bytecode){
   return hex_to_string[bytecode];
}
#define DEBUG_MODE
void Start_vCPU()
{
    void *instructions[] =
    {
        &&__HLT, // 0x00
        &&__MOV, // 0x01
        &&__INT, // 0x02
        &&__NOP, // 0x03
        &&__MUL, // 0x04
        &&__DIV, // 0x05
        &&__ADD, // 0x06
        &&__SUB  // 0x07
    };
    //while (true)
    //{
    #if defined DEBUG_MODE
    printf("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #");
    #endif
    EXECUTE:
    #if defined DEBUG_MODE
    printf("\n\n");
    printf("IPR [%03d|%02X|%s]\n", IPR, IPR, HexToBin(IPR));
    printf("GPR [%03d|%02X|%s]\n", GPR, GPR, HexToBin(GPR));
    #endif
    goto *(*(instructions + *(bytecode + IPR)));
    //--------------------------------------------------------------------------------
    __HLT: // 0x00 | Останавливает выполнение vCPU
    IPR++;
    #if defined DEBUG_MODE
    printf("\n%s\t\t| %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __MOV: // 0x01 | Пересылка данных
    IPR++;
    GPR = bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
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
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 0x07 | Вычитание
    IPR++;
    GPR -= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\n\n");
    printf("IPR [%03d|%02X|%s]\n", IPR, IPR, HexToBin(IPR));
    printf("GPR [%03d|%02X|%s]\n", GPR, GPR, HexToBin(GPR));
    printf("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #\n");
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
*/