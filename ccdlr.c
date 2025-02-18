//#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAXIMUM_BUFFER_SIZE 1024
#define EOS 0
char __buffer[MAXIMUM_BUFFER_SIZE] = {EOS};

#define strfind strstr

int main(int argc, unsigned char *argv[])
{
    setlocale(0, "");
    printf("Количество переданных аргументов: %d.", argc);
    for (int i = 0; i < argc; i ++) printf("\nАргумент %d: \"%s\".", i+1, argv[i]);
    unsigned char *strptr = strfind(argv[1], ".cdlr");
    unsigned char *strptr_2 = strfind(argv[1], ".sccdlr");
    if (strptr == NULL && strptr_2 == NULL)
    {
        printf("Неверное расширение файла...");
        return 1;
    }
    FILE *handle = fopen(argv[1], "rb"); // .cdlr или .sccdlr
    if (!handle)
    {
        printf("\nНе удалось открыть файл на чтение.");
        return 2;
    }
    for (int i = -1; (__buffer[++ i] = getc(handle)) != EOF;){;}
    fclose(handle);
    if (__buffer[0] == EOF)
    {
        printf("\nВ файле нет данных.");
    }
    //printf("__buffer[] = \"%s\".", __buffer);
    return 0;
}