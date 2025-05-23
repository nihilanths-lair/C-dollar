/*/
 *   /!\ Encoding Windows-1251
 *   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <stdbool.h> // Для использования логических типов: false/true.

char bytecode[] = {0x00, 0x00, 0x00, 0x01};
short ip = 0x0000;

void Start_vCPU()
{
    void *instructions[] =
    {
        &&nop,
        &&hlt
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