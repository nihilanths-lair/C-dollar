#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#define runblock {
#define endblock }

// Корректируемые состояния (обучающие/запоминающие кибернейроны) в процессе подачи данных
char __cyberneuron[2][5+1] =
{
    {""},
    {""}
};

void ln() { puts(""); }
int chi(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    return -1; // Ошибка, если символ не цифра
}
char *innings(char *buf, const char *cyberneuron_1, const char *cyberneuron_2)
{
    snprintf(buf, sizeof (buf), "%d", chi(cyberneuron_1[0]) && chi(cyberneuron_2[0]));
    return buf;
}
int main()
{
    setlocale(0, "");
    puts("---------------");
    char buf[1+1];
    printf("0 && 0 = %s", innings(buf, "0", "0")); ln();
    printf("0 && 1 = %s", innings(buf, "0", "1")); ln();
    printf("1 && 0 = %s", innings(buf, "1", "0")); ln();
    printf("1 && 1 = %s", innings(buf, "1", "1")); ln();
    printf("---------------");
    return 0;
}