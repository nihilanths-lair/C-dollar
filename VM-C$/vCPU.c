/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.

char bytecode[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00};

short IPR  = 0x0000; // instruction pointer register / регистр указателя инструкций
short GPR = 0x0000; // general purpose register / регистр общего назначения

void Start_vCPU()
{
    void *instructions[] =
    {
        &&_HLT, // i0, e1.
        &&_MOV, // i1, e2.
        &&_INT, // i2, e3.
        &&_NOP  // i3, e4.
    };
    //while (true)
    //{
    EXECUTE: printf("\n"); goto *(*(instructions + *(bytecode + IPR)));
    //-------------------------------------------------
    _HLT: printf("# DEBUG (%d): HLT - Остановить выполнение vCPU.", IPR); // i0 | Останавливает выполнение vCPU
    goto STOP_vCPU; //break;
    //-------------------------------------------------
    _MOV: printf("# DEBUG (%d): MOV - Перемещение данных.", IPR); // i1 | Пересылка данных
    IPR++;
    goto EXECUTE; //continue;
    //-------------------------------------------------
    _INT: printf("# DEBUG (%d): INT - Обращение к таблице векторных прерываний (IVT).", IPR); // i2 | Прерывание
    IPR++;
    goto EXECUTE; //continue;
    //-------------------------------------------------
    _NOP: printf("# DEBUG (%d): NOP - Заглушка", IPR); // i3 | Заглушка
    IPR++;
    goto EXECUTE; //continue;
    //-------------------------------------------------
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