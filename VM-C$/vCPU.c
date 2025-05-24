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
char *HexToString(char bytecode)
{
    /*
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
    */
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
    printf("\n\nIPR [%02X|%03d]\n", IPR, IPR);// | %s\t| %02X\n", IPR, IPR, HexToString(bytecode[IPR]), bytecode[IPR]
    printf("GPR [%02X|%03d]\n", GPR, GPR);
    #endif
    goto *(*(instructions + *(bytecode + IPR)));
    //-------------------------------------------------------------------------------------
    __HLT: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x00 | Останавливает выполнение vCPU
    IPR++;
    #if defined DEBUG_MODE
    printf("\n%s\t\t| %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    goto STOP_vCPU; //break;
    //-------------------------------------------------------------------------------------
    __MOV: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x01 | Пересылка данных
    //dbg_SetBytecodeToCollection()
    IPR++;
    GPR = bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __INT: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x02 | Обращение к таблице векторных прерываний (IVT)
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __NOP: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x03 | Заглушка
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __MUL: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x04 | Перемножение чисел
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __DIV: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x05 | Деление чисел
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __ADD: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x06 | Сложение чисел
    IPR++;
    GPR += bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    __SUB: //printf("# DEBUG (%d): 0x%02X|%s", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x07 | Вычитание чисел
    IPR++;
    GPR -= bytecode[IPR];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X", HexToString(bytecode[IPR-1]), bytecode[IPR], bytecode[IPR-1], bytecode[IPR]);
    #endif
    IPR++;
    goto EXECUTE;
    //-------------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\n\nIPR [%02X|%03d]\n", IPR, IPR);// | %s\t| %02X\n", IPR, IPR, HexToString(bytecode[IPR]), bytecode[IPR]
    printf("GPR [%02X|%03d]\n", GPR, GPR);
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
*/