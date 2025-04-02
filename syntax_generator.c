#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define then {
#define end }

#define EOS 0
#define strfind strstr

char *__source;
char *__source_code;
short __iter;

char RunCheck(const char source_code[]);

int main(int argc, unsigned char *argv[])
{
    setlocale(0, "");

    unsigned char *strptr = strfind(argv[1], ".cdlr");
    unsigned char *strptr_2 = strfind(argv[1], ".sccdlr");

    //unsigned char *strptr_3 = strfind(argv[1], ".avmcdlr"); unsigned char *strptr_4 = strfind(argv[1], ".scavmcdlr");

    if (strptr == NULL && strptr_2 == NULL)// && strptr_3 == NULL && strptr_4 == NULL)
    {
        printf(" Неверное расширение файла...");
        return 1;
    }
    FILE *handle = fopen(argv[1], "rb"); // *.cdlr или *.sccdlr или *.avmcdlr или *.scavmcdlr
    if (!handle)
    {
        printf("\nНе удалось открыть файл на чтение.");
        return 2;
    }
    fseek(handle, 0, SEEK_END);
	unsigned long fsize = ftell(handle);
    fseek(handle, 0, SEEK_SET);

    #if defined DEBUG_CODE
	printf("\n Размер файла (в байтах): %zu.", fsize);
    #endif

    __source_code = malloc(fsize + 1);
    if (__source_code == NULL)
    {
        puts(" Динамическая память не была выделена.");
        // тогда, здесь работаем с фиксированной (статической) длины памятью
        fclose(handle);
        return 3; // не забываем выйти (прервать процесс)
    }
    // иначе (в противном случае), работаем с переменной (динамической) длины памятью
    //{;
    __source_code[0] = getc(handle);
    if (__source_code[0] == EOF)
    {
        puts(" В файле нет данных.");
        return 4;
    }
    __iter = EOS;
    while ((__source_code[++__iter] = getc(handle)) != EOF){;}
    __source_code[__iter] = EOS;
    //;}
    fclose(handle);
    //printf("\n");
    /**    char    |    hex     |    dec     |          bin
     *  '0' - '9'    \30 - \39    048 - 057    0011 0000 - 0011 1001
     *  'A' - 'Z'    \41 - \5A    065 - 090    0100 0001 - 0101 1010
     *  'a' - 'z'    \61 - \7A    097 - 122    0110 0001 - 0111 1010
     *  'А' - 'Я'    \C0 - \DF    192 - 223    1100 0000 - 1101 1111
     *  'а' - 'я'    \E0 - \FF    224 - 255    1110 0000 - 1111 1111
     */
    /*
    for (short i = -1; ++ i < 256; )
    {
        if (i == '\0') printf(" \\0 \\%02X %d\n", i, i);
        else if (i == '\n') printf(" \\n \\%02X %d\n", i, i);
        else if (i == '\r') printf(" \\r \\%02X %d\n", i, i);
        else printf(" %c \\%02X %d\n", i, i, i);
    }
    */
    printf("\n");
    //for (unsigned char i = -1; ++ i < 256; ) putchar(i);
    //printf("\n");
    //char x=5;
    //printf(" x = %d\n", x);

    char ret = RunCheck(__source_code); // Запустить проверку
    free(__source_code);
    printf("\n");
    switch (ret) then
    case 0: printf(" Syntax -> [Done]\n"); break;
    case 1: printf(" Syntax -> [Error]\n"); break;
    default:
    {
        printf(" Код возврата: %d.\n", ret);
        return ret;
    }
    end
    return 0;
}
// Разрешенные символы
char allowed_characters[0xFF];
// Установить разрешенные символы
char SetAllowedCharacters()
{
    return 0;
}
short row = 1; // строка
short column = 1; // столбец
char RunCheck(const char source_code[]) //feed()
{
    printf("\n (string) source_code[] = \"%s\"\n", source_code);
    printf(" %c%c(byte) source_code[] = ", 0x20, 0x20);
    short i = -1;
    for (; source_code[++ i] != '\0'; ) printf("\\%02X", source_code[i]);
    printf("\\%02X\n", source_code[i]);

    if (source_code[0] == '\0') { printf(" Синтаксическая ошибка - строка %d, столбец %d: Не найдено ни одной инструкции.\n", row, column); return 1; }
    i = 0;
    while (source_code[++ i] != '\0')
    {
        if (source_code[i] == '\n') row += 1, column = 1;
        else column += 1;
    }
    //if (source_code[i] == '\0')
    //printf(" ..(byte) source_code[] = %X", source_code[0]);
    //for (short i = 0; source_code[++ i] != '\0'; ) printf(",%X", source_code[i]);
    return 2;
}