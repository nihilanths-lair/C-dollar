#include <locale.h>
#include <windows.h>
#include <stdio.h>
//static HMODULE handle = NULL;
typedef void (*PrintMessage)();
static PrintMessage printMessage = NULL;
int main()
{
    setlocale(0, "");
    open_file:
    FILE *fhandle = fopen("VM_configuration.xml", "rb");
    if (fhandle == NULL)
    {
        puts("Не удалось создать файл!");
        fhandle = fopen("VM_configuration.xml", "wb");
        if (fhandle == NULL)
        {
            puts("Не удалось создать файл!");
            return 0;
        }
        /*
        fwrite("<!DOCTYPE html>\n", sizeof (char), 16, fhandle);
        fwrite("<html lang=\"ru\">\n", sizeof (char), 17, fhandle);
        fwrite("</html>", sizeof (char), 7, fhandle);
        */
        fwrite("<root xml:lang=\"en\">\n", sizeof (char), 21, fhandle);
        fwrite("  <path_to_executable_file></path_to_executable_file>\n", sizeof (char), 54, fhandle);
        fwrite("  <name_executable_file></name_executable_file>\n", sizeof (char), 48, fhandle);
        fwrite("</root>", sizeof (char), 7, fhandle);
        fclose(fhandle);
        goto open_file;
    }
    puts("Файл открыт на чтение.");
    /*
    fseek(fhandle, 0, SEEK_END); // Перемещаемся в конец
    long size = ftell(fhandle);  // Получаем текущую позицию (размер)
    */
    fseek(fhandle, 16, SEEK_SET);
    //fseek(fhandle, -11, SEEK_END);
    char lang[2+1] = {0};
    fread(lang, sizeof (char), 2, fhandle);
    if (!strcmp(lang, "en")) puts("1");
    else if (!strcmp(lang, "ru")) puts("2");
    else puts("3");
    return 0;///
    HMODULE handle = LoadLibraryA("VM-C$.dll");
    if (!handle)
    {
        puts("Не удалось загрузить VM-C$!");
        return 0;
    }
    puts("VM-C$ запущена.");
    printMessage = (PrintMessage) GetProcAddress(handle, "PrintMessage");
    if (!printMessage)
    {
        printf("Не удалось найти функцию PrintMessage\n");
        FreeLibrary(handle);
        handle = NULL;
        return 0;
    }
    char command[64];
    input:
    printf(">_ ");
    scanf("%s", command);
    if (!strcmp(command, "Start"))
    {
        printMessage();
    }
    else
    {
        puts("Неизвестная команда!");
        goto input;
    }
    FreeLibrary(handle);
    handle = NULL;
    return 0;
}