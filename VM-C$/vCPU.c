/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.

char bytecode[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00};

short IP  = 0x0000;
short GPR = 0x0000; // general purpose register / регистр общего назначения

void Start_vCPU()
{
    void *instructions[] =
    {
        &&HLT, // idx:0, elm:1.
        &&MOV, // idx:1, elm:2.
        &&INT, // idx:2, elm:3.
        &&NOP  // idx:3, elm:4.
    };
    while (true)
    {
        goto *instructions[bytecode[IP]]; // Аналог *(instructions + (*(bytecode + IP))).
        //-------------------------------------------------
        HLT: // 0 | Останавливает выполнение vCPU
        puts("#DEBUG: HLT - Остановить выполнение vCPU.");
        break;
        //-------------------------------------------------
        MOV: // 1 | Пересылка данных
        puts("#DEBUG: MOV - Перемещает данные между памятью.");
        continue;
        //-------------------------------------------------
        INT: // 2 | Прерывание
        puts("#DEBUG: INT - Обращается к таблице векторных прерываний (IVT).");
        continue;
        //-------------------------------------------------
        NOP: // 3 | Заглушка
        puts("#DEBUG: NOP - Заглушка");
        IP++;
        continue;
        //-------------------------------------------------
    }
}
int main()
{
    setlocale(0, "");
    Start_vCPU();
    return 0;
}