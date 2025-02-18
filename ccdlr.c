//#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAXIMUM_BUFFER_SIZE 1024
unsigned char __buffer[MAXIMUM_BUFFER_SIZE];

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
    FILE *handle = fopen(argv[1], "rb+"); // .cdlr или .sccdlr
    fclose(handle);
    return 0;
}