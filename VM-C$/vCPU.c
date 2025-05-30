/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ ��������� Windows-1251
/*/
#include <stdio.h>   // ��� �����/������ ������ �� �������.
#include <locale.h>  // ��� ����������� �������.
#include <string.h>  // ��� ������ �� ��������.

unsigned char bytecode[] =
{
    0x01,  5,     // MOV GP, 5  / gp = 5;
    0x06, 45,     // ADD GP, 45 / gp += 45; | 5 + 45 = 50
    0x07,  3,     // SUB GP, 3  / gp -= 3;  | 50 - 3 = 47
    0x04,  5,     // MUL GP, 5  / gp *= 5;  | 47 * 5 = 235
    0x08, 12,     // JMP 12
    0x03,         // NOP
    0x03,         // NOP
    0x0D, 2, 2,   // CMP 2 2     / 2 == 2
    0x03,         // NOP
    0x03,         // NOP
    0x00          // HLT
};
typedef unsigned char uch;
typedef unsigned short ush;
/*--------------------------*/
uch IP = 0x00; // Instruction pointer / ��������� ����������.
/*--------------------------*/
uch GP = 0x00; // General purpose / ������ ����������.

ush AX = 0x0000;
ush DX = 0x0000;
/*--------------------------*/
uch SP = 0x00; // Stack pointer / ��������� �����.
uch BP = 0x00; // Base pointer / ��������� ����.
/*--------------------------*/
uch SI = 0x00; // Source index / ������ ���������.
/*--------------------------*/
#include <stdbool.h> // ��� ������������� ���������� �����: false/true.
bool ZFR = false; // zero flag register / ������� �������� �����.
//unsigned short DR = 0x00; // debug register / ������� �������.
// [MOV GP], [?] / ���, [MNC + OP1] = 0x01
// GP: [00] <- 0x00 ��� 00h
// MOV GP, imm8   - ��������� � ������� GP ���������������� ��������
// MOV GP, mem8   - ��������� � ������� GP �������� �� ������, ��������� �� ����� (value = ptr_address)
// MOV GP, [mem8] - ��������� � ������� GP �������� �� ������, ��������� �� ������ (value = *ptr_value)
const unsigned char hex_to_string[][6+1] =
{
    "HLT",     //  0
    "MOV GP", //  1
    "INT",     //  2
    "NOP",     //  3
    "MUL GP", //  4
    "DIV GP", //  5
    "ADD GP", //  6
    "SUB GP", //  7
    "JMP",     //  8
    "CALL",    //  9
    "PUSH",    // 10
    "POP",     // 11
    "RET"      // 12
    "CMP"      // 13
};
//#define HEX_TO_STRING(arg) hex_to_string[arg]
unsigned char hex_to_bin[256][8+1];
void generate_hex_to_bin_table()
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 7; j >= 0; j--) hex_to_bin[i][7-j] = (i & (1 << j)) ? '1' : '0';
        hex_to_bin[i][8] = '\0';
    }
}
//#define HEX_TO_BIN(arg) hex_to_bin[arg]
#define DEBUG_MODE
void Run_vCPU()
{
    void *instructions[] =
    {
        &&__HLT,  //  0
        &&__MOV,  //  1
        &&__INT,  //  2
        &&__NOP,  //  3
        &&__MUL,  //  4
        &&__DIV,  //  5
        &&__ADD,  //  6
        &&__SUB,  //  7
        &&__JMP,  //  8
        &&__CALL, //  9
        &&__PUSH, // 10
        &&__POP,  // 11
        &&__RET,  // 12
        &&__CMP   // 13
    };
    #if defined DEBUG_MODE
    generate_hex_to_bin_table();
    puts("\n# DEBUG MODE ON | ����� ������� ������� #\n");
    #endif
    EXECUTE:
    #if defined DEBUG_MODE
    puts("------------");
    //puts("\n");
    printf("IP [%03d|%02X|%s]\n", IP, IP, hex_to_bin[IP]); // byte_to_binary(IP, hex_to_bin);
    printf("GP [%03d|%02X|%s]\n", GP, GP, hex_to_bin[GP]); // byte_to_binary(GP, hex_to_bin);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZFR);
    //puts("    Z");
    puts("------");
    #endif
    goto *(*(instructions + *(bytecode + IP))); // goto *instructions[bytecode[IP]];
    //--------------------------------------------------------------------------------
    __HLT: // 0 | ������������� ���������� vCPU
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n", IP, IP, hex_to_string[bytecode[IP]], bytecode[IP]);
    #endif
    IP++;
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __MOV: // 1 | ��������� ������
    IP++;
    GP = bytecode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __INT: // 2 | ��������� � ������� ��������� ���������� (IVT)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n", hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    void *functions[] =
    {
        &&__09h,
        &&__10h
    };
    goto *functions[bytecode[IP]]; // goto *(*(functions + AX));
    // { 2
    __09h:
    uch *ptr_str = &bytecode[DX];
    puts(ptr_str);
    goto EXECUTE;
    __10h:
    // } 2
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __NOP: // 3 | ��������
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n\n", IP, IP, hex_to_string[bytecode[IP]], bytecode[IP]);
    //printf("\n%03d=%02X | %s\t\t| %02X", IP, IP, hex_to_string[bytecode[IP]], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __MUL: // 4 | ���������
    IP++;
    GP *= bytecode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __DIV: // 5 | �������
    IP++;
    GP /= bytecode[IP];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __ADD: // 6 | ��������
    IP++;
    GP += bytecode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 7 | ���������
    IP++;
    GP -= bytecode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __JMP: // 8 | ������ �� ����� (�����)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n\n", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X\t\t| %02X %02X", IP-1, IP-1, hex_to_string[bytecode[IP-1]], bytecode[IP], bytecode[IP-1], bytecode[IP]); //
    #endif
    IP = bytecode[IP];
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CALL: //  9 | ����� ���������
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __PUSH: // 10 | �������� �� ����
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __POP: // 11 | ����� �� �����
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __RET: // 12 | ������� �� ���������
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CMP: // 13 | ���������
    if (bytecode[++IP] == bytecode[++IP]) ZFR = true;
    else ZFR = false;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X %02X\t| %02X %02X %02X\n\n", IP-2, IP-2, hex_to_string[bytecode[IP-2]], bytecode[IP-1], bytecode[IP], bytecode[IP-2], bytecode[IP-1], bytecode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X %02X\t\t| %02X %02X %02X", IP-2, IP-2, hex_to_string[bytecode[IP-2]], bytecode[IP-1], bytecode[IP], bytecode[IP-2], bytecode[IP-1], bytecode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    //}
    STOP_vCPU:
    #if defined DEBUG_MODE
    printf("\nIP [%03d|%02X|%s]\n", IP, IP, hex_to_bin[IP]);
    printf("GP [%03d|%02X|%s]\n", GP, GP, hex_to_bin[GP]);
    puts("------");
    puts("    Z");
    printf("FR [%-1d]\n", ZFR);
    //puts("    Z");
    puts("\n# DEBUG MODE OFF | ����� ������� �������� #");
    #endif
    //puts("\n# DEBUG MODE OFF | ����� ������� �������� #");
}