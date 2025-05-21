#include <locale.h>
#include "VMWrapper.h"

int main()
{
    setlocale(0, "");
    if (!VM_Init("VM-C$.dll")) return 1;
    VM_PrintMessage();
    VM_Cleanup();
    return 0;
}
/*
// VortexEngine.c
#include <windows.h>
#include <stdio.h>

#include <locale.h>

// Объявляем тип функции, как в DLL
typedef void (*PrintMessageFunc)();

int main()
{
    setlocale(0, "");

    // Загружаем DLL
    HMODULE hDll = LoadLibraryA("VM-C$.dll");
    if (!hDll) {
        printf("Не удалось загрузить DLL.\n");
        return 1;
    }

    // Получаем адрес функции
    PrintMessageFunc printFunc = (PrintMessageFunc) GetProcAddress(hDll, "PrintMessage");
    if (!printFunc) {
        printf("Не удалось найти функцию PrintMessage.\n");
        return 1;
    }

    // Вызываем функцию
    printFunc();

    // Освобождаем DLL
    FreeLibrary(hDll);
    return 0;
}
*/