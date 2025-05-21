#include <windows.h>
#include <stdio.h>

#include "VM-API.h"

typedef void (*PrintMessageFunc)();

static HMODULE hVM = NULL;
static PrintMessageFunc printMessage = NULL;

int VM_Init()
{
    hVM = LoadLibraryA("VM-C$.dll");
    if (!hVM)
    {
        puts("Не удалось загрузить VM-C$.");
        return 0;
    }
    printMessage = (PrintMessageFunc) GetProcAddress(hVM, "PrintMessage");
    if (!printMessage)
    {
        printf("Не удалось найти функцию PrintMessage\n");
        FreeLibrary(hVM);
        hVM = NULL;
        return 0;
    }
    return 1;
}
void VM_Cleanup(void)
{
    if (hVM)
    {
        FreeLibrary(hVM);
        hVM = NULL;
        printMessage = NULL;
    }
}
void VM_PrintMessage(void)
{
    if (printMessage) printMessage();
    else printf("VM не инициализирована.\n");
}