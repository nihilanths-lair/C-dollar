#include <stdio.h>
#include <locale.h>
#include <string.h>
//#include <stdbool.h>

#define MAXIMUM_BUFFER_SIZE 1024
#define EOS 0
char __buffer[MAXIMUM_BUFFER_SIZE] = {EOS};

// Прочитать все данные полностью
//bool read_all_data_completely = true;
// Инкрементная обработка
//bool incremental_processing = false;
// Обработка исходного кода
unsigned char source_code_processing[] = "Целиком"; // Целиком / Частями

#define strfind strstr
#define DEBUG_CODE

#include "native.h"
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
    if (!strcmp(source_code_processing, "Частями")) IncrementalProcessing(handle);
    else if (!strcmp(source_code_processing, "Целиком")) NotIncrementalProcessing(handle);
    //if (incremental_processing) IncrementalProcessing(handle); //goto incremental_processing;
    //else NotIncrementalProcessing(handle);
    //goto end_of_program;
    incremental_processing:
    // код инкрементной обработки
    end_of_program:
    return 0;
}
int IncrementalProcessing(FILE *handle)
{
    return 0;
}
int NotIncrementalProcessing(FILE *handle)
{
    for (int i = -1; (__buffer[++ i] = getc(handle)) != EOF;);
    fclose(handle);
    if (__buffer[0] == EOF)
    {
        printf("\nВ файле нет данных.");
        return 3;
    }
    #if defined DEBUG_CODE
    printf("\n__buffer[0] = 0x%X.", __buffer[0] & 0xFF);
    #endif
    return 0;
}