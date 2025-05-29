#include <stdio.h>

int main()
{
    short count = 255;
    short i;
    puts("const unsigned char translate_opcode_into_symbolic_form[][8+1] =");
    puts("{");
    i = 0;
    while (i < 176)//0xB0
    {
        printf("    \"?\", // %02X %03d\n", i, i);
        i++;
    }
    //printf("    \"\", // %02X %03d\n", i, i);
    printf("    \"MOV AL\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV CL\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV DL\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV BL\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV AH\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV CH\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV DH\", // %02X %03d\n", i, i);
    i++;
    printf("    \"MOV BH\"  // %02X %03d\n", i, i);
    i = 184;//0xB8
    while (i < 255)//0xFF
    {
        printf("    \"?\", // %02X %03d\n", i, i);
        i++;
    }
    printf("    \"?\"  // %02X %03d\n", i, i);
    puts("};");
    /*
    puts("void *instructions[] =");
    puts("{");
    i = 0;
    while (i < count)
    {
        printf("    &&__%03d,\n", i);
        i++;
    }
    printf("    &&__%03d\n", i);
    puts("};");
    puts("goto *(*(instructions + *(opcode + IP)));");
    i = 0;
    while (i < count)
    {
        printf("__%03d:\n", i);
        i++;
    }
    printf("__%03d:", i);
    */
    return 0;
}
