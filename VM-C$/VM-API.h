#ifndef VM_API_H
#define VM_API_H

int VM_Init(const char* dllPath);
void VM_Cleanup(void);
void VM_PrintMessage(void);

#endif // VM_API_H