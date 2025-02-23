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
unsigned char __token[2][64+1]; // лексемы

#define strfind strstr
#define DEBUG_CODE

#include "declaration.h"
int main(int argc, unsigned char *argv[])
{
    setlocale(0, "");
    //printf("Количество переданных аргументов: %d.", argc);
    //for (int i = 0; i < argc; i ++) printf("\nАргумент %d: \"%s\".", i+1, argv[i]);
    unsigned char *strptr = strfind(argv[1], ".cdlr");
    unsigned char *strptr_2 = strfind(argv[1], ".sccdlr");

    unsigned char *strptr_3 = strfind(argv[1], ".avmcdlr");
    unsigned char *strptr_4 = strfind(argv[1], ".scavmcdlr");

    if (strptr == NULL && strptr_2 == NULL && strptr_3 == NULL && strptr_4 == NULL)
    {
        printf("Неверное расширение файла...");
        return 1;
    }
    FILE *handle = fopen(argv[1], "rb"); // *.cdlr или *.sccdlr или *.avmcdlr или *.scavmcdlr
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
	printf("Размер файла (в байтах): %zu.", fsize);
    #endif

    __buffer = malloc(fsize+1);
    if (__buffer == NULL)
    {
        printf("Динамическая память не была выделена.");
        return 3;
    }
    fseek(handle, 0, SEEK_SET);
    {;
        int i = -1;
        while ((__buffer[++ i] = getc(handle)) != EOF){;}
        __buffer[i] = EOS;
    ;}
    fclose(handle);
    if (__buffer[0] == EOF)
    {
        printf("В файле нет данных.");
        return 4;
    }
    LexicalAnalyzer();
    free(__buffer);
    return 0;
}
// Лексический анализатор
int LexicalAnalyzer()
{
    for (int i = 0; __buffer[i] != EOS; i ++)
    {
        #if !defined DEBUG_CODE
        if (__buffer[i] == 0x0A) printf("\n__buffer[%02d] = '\\n'", i, __buffer[i]);
        else if (__buffer[i] == 0x0D) printf("\n__buffer[%02d] = '\\r'", i, __buffer[i]);
        else printf("\n__buffer[%02d] = '%c'", i, __buffer[i]);
        #endif
        if (__buffer[i] != ',')
        {
            __token[0][i] = __buffer[i];
        }
    }
    #if defined DEBUG_CODE
    printf("\n__token[0] = \"%s\".", __token[0]);
    printf("\n__token[1] = \"%s\".", __token[1]);
    #endif
    SyntacticAnalyzer();
    return 0;
}
// Синтаксический анализатор
int SyntacticAnalyzer()
{
    if (strcmp(__token[0], "mov ax")) return 5;
    SemanticAnalyzer();
    return 0;
}
// Семантический анализатор
int SemanticAnalyzer()
{
    BytecodeGenerator();
    return 0;
}
// Генератор байт-кода (переносимого кода)
int BytecodeGenerator() // int PortableCodeGenerator() {}
{
    unsigned char bytecode[2];
    if (!strcmp(__token[0], "mov ax"))
    {
        bytecode[0] = 0xB8;
        bytecode[1] = __token[1][0];
    }
    printf("\n0x%02X", bytecode[0]);
    printf("\n0x%02X", bytecode[1]);
    printf("\nCompilation complete.");
    return 0;
}
// Генератор машинного (нативного) кода
int MachineCodeGenerator() // int NativeCodeGenerator() {}
{
    printf("\nCompilation complete.");
    return 0;
}
// Оптимизатор кода - ?