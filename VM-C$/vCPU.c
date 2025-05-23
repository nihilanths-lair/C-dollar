/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
//#include <stdbool.h> // Для использования логических типов: false/true.
#include <string.h>  // 

char bytecode[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00};

short IPR  = 0x0000; // instruction pointer register / регистр указателя инструкций
short GPR = 0x0000; // general purpose register / регистр общего назначения

char *HexToString(char bytecode)
{
    switch (bytecode){
    case 0x00: return "HLT";
    case 0x01: return "MOV";
    case 0x02: return "INT";
    case 0x03: return "NOP";
    case 0x04: return "MUL";
    case 0x05: return "DIV";
    case 0x06: return "ADD";
    case 0x07: return "SUB";
    }
    return "/!\\";
}
void Start_vCPU()
{
    void *instructions[] =
    {
        &&_HLT, // 0x00
        &&_MOV, // 0x01
        &&_INT, // 0x02
        &&_NOP, // 0x03
        &&_MUL, // 0x04
        &&_DIV, // 0x05
        &&_ADD, // 0x06
        &&_SUB  // 0x07
    };
    //while (true)
    //{
    EXECUTE: printf("\n"); goto *(*(instructions + *(bytecode + IPR)));
    //----------------------------------------------------------------------------------------------------
    _HLT: printf("# DEBUG (%d): 0x%02X|%s - Остановить выполнение vCPU.", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x00 | Останавливает выполнение vCPU
    goto STOP_vCPU; //break;
    //----------------------------------------------------------------------------------------------------
    _MOV: printf("# DEBUG (%d): 0x%02X|%s - Перемещение данных.", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x01 | Пересылка данных
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _INT: printf("# DEBUG (%d): 0x%02X|%s - Обращение к таблице векторных прерываний (IVT).", IPR, bytecode[IPR], HexToString(bytecode[IPR])); // 0x02 | Прерывание
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _NOP: printf("# DEBUG (%d): 0x%02X|%s - Заглушка", IPR, bytecode[IPR], HexToString(bytecode[IPR]));            // 0x03 | Заглушка
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _MUL: printf("# DEBUG (%d): 0x%02X|%s - Перемножение чисел", IPR, bytecode[IPR], HexToString(bytecode[IPR]));  // 0x04 | Перемножение чисел
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _DIV: printf("# DEBUG (%d): 0x%02X|%s - Деление чисел", IPR, bytecode[IPR], HexToString(bytecode[IPR]));       // 0x05 | Деление чисел
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _ADD: printf("# DEBUG (%d): 0x%02X|%s - Сложение чисел", IPR, bytecode[IPR], HexToString(bytecode[IPR]));      // 0x06 | Сложение чисел
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    _SUB: printf("# DEBUG (%d): 0x%02X|%s - Вычитание чисел", IPR, bytecode[IPR], HexToString(bytecode[IPR]));     // 0x07 | Вычитание чисел
    IPR++;
    goto EXECUTE; //continue;
    //----------------------------------------------------------------------------------------------------
    //}
    STOP_vCPU: printf("\n");
}
int main()
{
    setlocale(0, "");
    Start_vCPU();
    return 0;
}
// goto *instructions[bytecode[IP]];