// vm-c$.c
#include <stdio.h>
#include <windows.h>

__declspec(dllexport)
void PrintMessage() {
    printf("Ты меня вызвал в EXE из под DLL.\n");
}