#include <stdio.h>
#include <locale.h>
//
char fop(FILE *h, const char *fn);
//
int main(int argc, char *argv[])
{
    setlocale(0, ""); // установим русский язык
    //
    FILE* h = NULL;
    if (fop(h, "compiler.cfg") == -1) return 1;
    printf("Количество переданных аргументов функции main: %d.\n", argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("Аргумент %d: %s\n", i, argv[i]);
    }
    char symbol;
    if ((symbol = fgetc(h)) == EOF) // если файл пуст, то
    {
        printf("Файл пустой.\n");
    }
    else printf("Файл содержит запись.");
    fclose(h);
    printf("Файл закрыт.");
    return 0;
}
char fop(FILE *h, const char *fn)
{
    h = fopen(fn, "rb"); // откроем файл
    if (h == NULL) // если файл не существует, то
    {
        h = fopen(fn, "wb"); // создадим его, и
        if (h == NULL) // снова проверим, что файл создан
        {
            printf("Не смог создать файл.");
            return -1;
        }
        printf("Файл создан и открыт на запись.\n");
        // заполним его конфигурационными данными по умолчанию
        fputs("Аргументы с консоли: Нет", h);
        fclose(h);
        printf("Файл закрыт.\n");
        /*
        h = fopen(fn, "rb"); // откроем файл, и считаем конфигурационные данные с него
        if (h == NULL) // если файл не существует, то
        {
            printf("Не смог открыть файл на чтение.");
            return -1;
        }
        printf("Файл переоткрыт на чтение.\n"); // если файл существует, то считаем с него конфигурационные данные
        */
    }
    else printf("Файл открыт на чтение.\n"); // если файл существует, то считаем с него конфигурационные данные
    char cfg[64]; cfg[0] = '\0';
    printf("A | cfg[] = \"%s\"\n", cfg);
    fgets(cfg, sizeof (cfg), h);
    printf("B | cfg[] = \"%s\"\n", cfg);
    return 0;
}