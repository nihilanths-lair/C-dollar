#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
//#include <stdbool.h>

#define MAXIMUM_BUFFER_SIZE 1024
#define EOS 0
//char __buffer[MAXIMUM_BUFFER_SIZE] = {EOS};
char *__buffer; // = NULL

//bool read_all_data_completely = true; // Прочитать все данные полностью
//bool incremental_processing = false; // Инкрементная обработка
// Обработка исходного кода
unsigned char source_code_processing[] = "Целиком"; // Целиком / Частями

#define strfind strstr
#define DEBUG_CODE

#include "native.h"
int main(int argc, unsigned char *argv[])
{
    setlocale(0, "");
    //printf("Количество переданных аргументов: %d.", argc);
    //for (int i = 0; i < argc; i ++) printf("\nАргумент %d: \"%s\".", i+1, argv[i]);
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
    fclose(handle);
    return 0;
}
int NotIncrementalProcessing(FILE *handle)
{
    fseek(handle, 0, SEEK_END);
    unsigned long fsize = ftell(handle);

    #if defined DEBUG_CODE
    printf("Размер файла (в байтах): %zu.\n", fsize);
    #endif

    __buffer = malloc(fsize+1);
    if (__buffer == NULL)
    {
        printf("Динамическая память не была выделена.");
        return 3;
    }
    fseek(handle, 0, SEEK_SET);
    int i = -1;
    for (;(__buffer[++ i] = getc(handle)) != EOF;);
    __buffer[i] = EOS;
    fclose(handle);
    if (__buffer[0] == EOF)
    {
        printf("В файле нет данных.");
        return 4;
    }

    #if defined DEBUG_CODE
    printf("__buffer[] = \"%s\".", __buffer);
    #endif

    free(__buffer);
    return 0;
}
// Синтаксический анализатор
// Семантический анализатор
// Оптимизатор кода - ?
// Генератор байт-кода (переносимого кода)
