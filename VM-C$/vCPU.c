/*/
-*-   /!\ Encoding Windows-1251
-*-   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.

char bytecode[] = {0x00, 0x00, 0x00, 0x01};

short ip = 0x0000;
short gpr = 0x0000; // general purpose register / регистр общего назначения

void Start_vCPU()
{
    void *instructions[] =
    {
        &&nop, // idx:0, elm:1.
        &&hlt  // idx:1, elm:2.
    };
    while (true)
    {
        goto *instructions[bytecode[ip]]; // Аналог *(ptr_label+i).
        nop: // Заглушка
        puts("#DEBUG: NOP - заглушка");
        ip++;
        continue;
        hlt: // Останавливает выполнение vCPU
        puts("#DEBUG: HLT - остановить выполнение vCPU.");
        break;
    }
}
int main()
{
    setlocale(0, "");
    Start_vCPU();
    return 0;
}