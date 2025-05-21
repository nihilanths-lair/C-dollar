#ifndef VMWRAPPER_H
#define VMWRAPPER_H

int VM_Init(const char* dllPath);
void VM_Cleanup(void);
void VM_PrintMessage(void);

#endif // VMWRAPPER_H