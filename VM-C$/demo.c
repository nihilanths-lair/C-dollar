#include "VM-C$.h"

int main()
{
    //vmcs_execute_command(argv[1]);
    vmcs_start();
    printf("\n");
    system("pause");
    return 0;
}