/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.

char bytecode[] = {0x02, 0x02, 0x02, 0x00};

short ip = 0x0000;
short gpr = 0x0000; // general purpose register / регистр общего назначения

void Start_vCPU()
{
    void *instructions[] =
    {
        &&hlt, // idx:0, elm:1.
        &&mov, // idx:1, elm:2.
        &&nop  // idx:2, elm:3.
    };
    while (true)
    {
        goto *instructions[bytecode[ip]]; // Аналог *(ptr_label+i).
        //-------------------------------------------------
        hlt: // Останавливает выполнение vCPU
        puts("#DEBUG: HLT - остановить выполнение vCPU.");
        break;
        //-------------------------------------------------
        mov: // Пересылает данные
        puts("#DEBUG: MOV - перемещает данные между памятью.");
        continue;
        //-------------------------------------------------
        nop: // Заглушка
        puts("#DEBUG: NOP - заглушка");
        ip++;
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