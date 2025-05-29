/*/
|*|   /!\ Encoding Windows-1251
|*|   /!\ Кодировка Windows-1251
/*/
#include <stdio.h>   // Для ввода/вывода данных на консоль.
#include <locale.h>  // Для локализации консоли.
#include <string.h>  // Для работы со строками.

unsigned char opcode[] =
{
    // Секция кода
    0xB4,  9,                                                                     // MOV AH, 9
    0xBA,  4,                                                                     // MOV DX, 4
    // Секция данных
    0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, // Hello world!
    //
    //0x06, 45,     // MOV DX, offset String / -offset = 109
    0x00            // HLT
};
/*------------------------------------------------------------*/
typedef unsigned short R16;
/*------------------------------------------------------------*/
R16 AX; // Accumulator / Аккумулятор
R16 BX; //        Base / База
R16 CX; //     Counter / Счётчик
R16 DX; //        Data / Данные
/*------------------------------------------------------------*/
R16 IP = 0;//0x0100; // Instruction pointer / Указатель инструкций
/*------------------------------------------------------------*/
R16 CS = 0x0700; //  Code segment / Сегмент кода
R16 DS = 0x0700; //  Data segment / Сегмент данных
R16 SS = 0x0700; // Stack segment / Сегмент стека

R16 ES = 0x0700; // Extended segment / Расширенный сегмент
/*------------------------------------------------------------*/
R16 SP; // Stack pointer / Указатель стека
R16 BP; //  Base pointer / Указатель базы
/*------------------------------------------------------------*/
R16 SI; //      Source index / Индекс источника
R16 DI; // Destination index / Индекс приёмника
/*------------------------------------------------------------*/
#include <stdbool.h> // Для использования логических типов: false/true.
bool ZF = false; // zero flag register / регистр нулевого флага.
// [MOV GP], [?] / где, [MNC + OP1] = 0x01
// R: [00] <- 0x00 или 00h
// MOV R, imm8   - поместить в регистр непосредственное значение
// MOV R, mem8   - поместить в регистр значение из памяти, обращение по имени (value = ptr_address)
// MOV R, [mem8] - поместить в регистр значение из памяти, обращение по адресу (value = *ptr_value)
const unsigned char translate_opcode_into_symbolic_form[][8+1] =
{
    "?", // 00 000
    "?", // 01 001
    "?", // 02 002
    "?", // 03 003
    "?", // 04 004
    "?", // 05 005
    "?", // 06 006
    "?", // 07 007
    "?", // 08 008
    "?", // 09 009
    "?", // 0A 010
    "?", // 0B 011
    "?", // 0C 012
    "?", // 0D 013
    "?", // 0E 014
    "?", // 0F 015
    "?", // 10 016
    "?", // 11 017
    "?", // 12 018
    "?", // 13 019
    "?", // 14 020
    "?", // 15 021
    "?", // 16 022
    "?", // 17 023
    "?", // 18 024
    "?", // 19 025
    "?", // 1A 026
    "?", // 1B 027
    "?", // 1C 028
    "?", // 1D 029
    "?", // 1E 030
    "?", // 1F 031
    "?", // 20 032
    "?", // 21 033
    "?", // 22 034
    "?", // 23 035
    "?", // 24 036
    "?", // 25 037
    "?", // 26 038
    "?", // 27 039
    "?", // 28 040
    "?", // 29 041
    "?", // 2A 042
    "?", // 2B 043
    "?", // 2C 044
    "?", // 2D 045
    "?", // 2E 046
    "?", // 2F 047
    "?", // 30 048
    "?", // 31 049
    "?", // 32 050
    "?", // 33 051
    "?", // 34 052
    "?", // 35 053
    "?", // 36 054
    "?", // 37 055
    "?", // 38 056
    "?", // 39 057
    "?", // 3A 058
    "?", // 3B 059
    "?", // 3C 060
    "?", // 3D 061
    "?", // 3E 062
    "?", // 3F 063
    "?", // 40 064
    "?", // 41 065
    "?", // 42 066
    "?", // 43 067
    "?", // 44 068
    "?", // 45 069
    "?", // 46 070
    "?", // 47 071
    "?", // 48 072
    "?", // 49 073
    "?", // 4A 074
    "?", // 4B 075
    "?", // 4C 076
    "?", // 4D 077
    "?", // 4E 078
    "?", // 4F 079
    "?", // 50 080
    "?", // 51 081
    "?", // 52 082
    "?", // 53 083
    "?", // 54 084
    "?", // 55 085
    "?", // 56 086
    "?", // 57 087
    "?", // 58 088
    "?", // 59 089
    "?", // 5A 090
    "?", // 5B 091
    "?", // 5C 092
    "?", // 5D 093
    "?", // 5E 094
    "?", // 5F 095
    "?", // 60 096
    "?", // 61 097
    "?", // 62 098
    "?", // 63 099
    "?", // 64 100
    "?", // 65 101
    "?", // 66 102
    "?", // 67 103
    "?", // 68 104
    "?", // 69 105
    "?", // 6A 106
    "?", // 6B 107
    "?", // 6C 108
    "?", // 6D 109
    "?", // 6E 110
    "?", // 6F 111
    "?", // 70 112
    "?", // 71 113
    "?", // 72 114
    "?", // 73 115
    "?", // 74 116
    "?", // 75 117
    "?", // 76 118
    "?", // 77 119
    "?", // 78 120
    "?", // 79 121
    "?", // 7A 122
    "?", // 7B 123
    "?", // 7C 124
    "?", // 7D 125
    "?", // 7E 126
    "?", // 7F 127
    "?", // 80 128
    "?", // 81 129
    "?", // 82 130
    "?", // 83 131
    "?", // 84 132
    "?", // 85 133
    "?", // 86 134
    "?", // 87 135
    "?", // 88 136
    "?", // 89 137
    "?", // 8A 138
    "?", // 8B 139
    "?", // 8C 140
    "?", // 8D 141
    "?", // 8E 142
    "?", // 8F 143
    "?", // 90 144
    "?", // 91 145
    "?", // 92 146
    "?", // 93 147
    "?", // 94 148
    "?", // 95 149
    "?", // 96 150
    "?", // 97 151
    "?", // 98 152
    "?", // 99 153
    "?", // 9A 154
    "?", // 9B 155
    "?", // 9C 156
    "?", // 9D 157
    "?", // 9E 158
    "?", // 9F 159
    "?", // A0 160
    "?", // A1 161
    "?", // A2 162
    "?", // A3 163
    "?", // A4 164
    "?", // A5 165
    "?", // A6 166
    "?", // A7 167
    "?", // A8 168
    "?", // A9 169
    "?", // AA 170
    "?", // AB 171
    "?", // AC 172
    "?", // AD 173
    "?", // AE 174
    "?", // AF 175
    
    "MOV AL", // B0 176
    "MOV CL", // B1 177
    "MOV DL", // B2 178
    "MOV BL", // B3 179

    "MOV AH", // B4 180
    "MOV CH", // B5 181
    "MOV DH", // B6 182
    "MOV BH", // B7 183

    "MOV AX", // B8 184
    "MOV CX", // B9 185
    "MOV DX", // BA 186
    "MOV BX", // BB 187

    "?", // BC 188
    "?", // BD 189
    "?", // BE 190
    "?", // BF 191
    "?", // C0 192
    "?", // C1 193
    "?", // C2 194
    "?", // C3 195
    "?", // C4 196
    "?", // C5 197
    "?", // C6 198
    "?", // C7 199
    "?", // C8 200
    "?", // C9 201
    "?", // CA 202
    "?", // CB 203
    "?", // CC 204

    "INT", // CD 205

    "?", // CE 206
    "?", // CF 207
    "?", // D0 208
    "?", // D1 209
    "?", // D2 210
    "?", // D3 211
    "?", // D4 212
    "?", // D5 213
    "?", // D6 214
    "?", // D7 215
    "?", // D8 216
    "?", // D9 217
    "?", // DA 218
    "?", // DB 219
    "?", // DC 220
    "?", // DD 221
    "?", // DE 222
    "?", // DF 223
    "?", // E0 224
    "?", // E1 225
    "?", // E2 226
    "?", // E3 227
    "?", // E4 228
    "?", // E5 229
    "?", // E6 230
    "?", // E7 231
    "?", // E8 232
    "?", // E9 233
    "?", // EA 234
    "?", // EB 235
    "?", // EC 236
    "?", // ED 237
    "?", // EE 238
    "?", // EF 239
    "?", // F0 240
    "?", // F1 241
    "?", // F2 242
    "?", // F3 243
    "?", // F4 244
    "?", // F5 245
    "?", // F6 246
    "?", // F7 247
    "?", // F8 248
    "?", // F9 249
    "?", // FA 250
    "?", // FB 251
    "?", // FC 252
    "?", // FD 253
    "?", // FE 254
    "?"  // FF 255
};
//#define HEX_TO_STRING(arg) hex_to_string[arg]
//#define HEX_TO_BIN(arg) hex_to_bin[arg]
#define DEBUG_MODE
void Run_vCPUx86()
{
    void *instructions[] =
    {
        &&__000,
        &&__001,
        &&__002,
        &&__003,
        &&__004,
        &&__005,
        &&__006,
        &&__007,
        &&__008,
        &&__009,
        &&__010,
        &&__011,
        &&__012,
        &&__013,
        &&__014,
        &&__015,
        &&__016,
        &&__017,
        &&__018,
        &&__019,
        &&__020,
        &&__021,
        &&__022,
        &&__023,
        &&__024,
        &&__025,
        &&__026,
        &&__027,
        &&__028,
        &&__029,
        &&__030,
        &&__031,
        &&__032,
        &&__033,
        &&__034,
        &&__035,
        &&__036,
        &&__037,
        &&__038,
        &&__039,
        &&__040,
        &&__041,
        &&__042,
        &&__043,
        &&__044,
        &&__045,
        &&__046,
        &&__047,
        &&__048,
        &&__049,
        &&__050,
        &&__051,
        &&__052,
        &&__053,
        &&__054,
        &&__055,
        &&__056,
        &&__057,
        &&__058,
        &&__059,
        &&__060,
        &&__061,
        &&__062,
        &&__063,
        &&__064,
        &&__065,
        &&__066,
        &&__067,
        &&__068,
        &&__069,
        &&__070,
        &&__071,
        &&__072,
        &&__073,
        &&__074,
        &&__075,
        &&__076,
        &&__077,
        &&__078,
        &&__079,
        &&__080,
        &&__081,
        &&__082,
        &&__083,
        &&__084,
        &&__085,
        &&__086,
        &&__087,
        &&__088,
        &&__089,
        &&__090,
        &&__091,
        &&__092,
        &&__093,
        &&__094,
        &&__095,
        &&__096,
        &&__097,
        &&__098,
        &&__099,
        &&__100,
        &&__101,
        &&__102,
        &&__103,
        &&__104,
        &&__105,
        &&__106,
        &&__107,
        &&__108,
        &&__109,
        &&__110,
        &&__111,
        &&__112,
        &&__113,
        &&__114,
        &&__115,
        &&__116,
        &&__117,
        &&__118,
        &&__119,
        &&__120,
        &&__121,
        &&__122,
        &&__123,
        &&__124,
        &&__125,
        &&__126,
        &&__127,
        &&__128,
        &&__129,
        &&__130,
        &&__131,
        &&__132,
        &&__133,
        &&__134,
        &&__135,
        &&__136,
        &&__137,
        &&__138,
        &&__139,
        &&__140,
        &&__141,
        &&__142,
        &&__143,
        &&__144,
        &&__145,
        &&__146,
        &&__147,
        &&__148,
        &&__149,
        &&__150,
        &&__151,
        &&__152,
        &&__153,
        &&__154,
        &&__155,
        &&__156,
        &&__157,
        &&__158,
        &&__159,
        &&__160,
        &&__161,
        &&__162,
        &&__163,
        &&__164,
        &&__165,
        &&__166,
        &&__167,
        &&__168,
        &&__169,
        &&__170,
        &&__171,
        &&__172,
        &&__173,
        &&__174,
        &&__175,
        &&__176,
        &&__177,
        &&__178,
        &&__179,
        &&__180,
        &&__181,
        &&__182,
        &&__183,
        &&__184,
        &&__185,
        &&__186,
        &&__187,
        &&__188,
        &&__189,
        &&__190,
        &&__191,
        &&__192,
        &&__193,
        &&__194,
        &&__195,
        &&__196,
        &&__197,
        &&__198,
        &&__199,
        &&__200,
        &&__201,
        &&__202,
        &&__203,
        &&__204,
        &&__205,
        &&__206,
        &&__207,
        &&__208,
        &&__209,
        &&__210,
        &&__211,
        &&__212,
        &&__213,
        &&__214,
        &&__215,
        &&__216,
        &&__217,
        &&__218,
        &&__219,
        &&__220,
        &&__221,
        &&__222,
        &&__223,
        &&__224,
        &&__225,
        &&__226,
        &&__227,
        &&__228,
        &&__229,
        &&__230,
        &&__231,
        &&__232,
        &&__233,
        &&__234,
        &&__235,
        &&__236,
        &&__237,
        &&__238,
        &&__239,
        &&__240,
        &&__241,
        &&__242,
        &&__243,
        &&__244,
        &&__245,
        &&__246,
        &&__247,
        &&__248,
        &&__249,
        &&__250,
        &&__251,
        &&__252,
        &&__253,
        &&__254,
        &&__255
    };
    #if defined DEBUG_MODE
    //generate_hex_to_bin_table();
    //puts("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #\n");
    puts("\n# DEBUG MODE ON | РЕЖИМ ОТЛАДКИ ВКЛЮЧЕН #");
    #endif
    EXECUTE:
    #if !defined DEBUG_MODE
    puts("-------------------------------------------");
    puts("\t HEX\t    DEC");
    puts("\t*H|*L(8)   *H|*L(8)");
    //puts("\tAH|AL(8)   AH|AL(8)");
    printf("AX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (AX>>8)&0xFF, AX&0xFF, (AX>>8)&0xFF, AX&0xFF, AX);//, hex_to_bin[AX]);
    //puts("\tBH|BL(8)   BH|BL(8)");
    printf("BX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (BX>>8)&0xFF, BX&0xFF, (BX>>8)&0xFF, BX&0xFF, BX);
    //puts("\tCH|CL(8)   CH|CL(8)");
    printf("CX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (CX>>8)&0xFF, CX&0xFF, (CX>>8)&0xFF, CX&0xFF, CX);
    //puts("\tDH|DL(8)   DH|DL(8)");
    printf("DX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (DX>>8)&0xFF, DX&0xFF, (DX>>8)&0xFF, DX&0xFF, DX);
    puts("");
    printf("CS(16):[%02X.%02X] | [%03d.%03d] = %d\n", (CS>>8)&0xFF, CS&0xFF, (CS>>8)&0xFF, CS&0xFF, CS); // "/%d", , 0xFFFF);
    printf("IP(16):[%02X.%02X] | [%03d.%03d] = %d\n", (IP>>8)&0xFF, IP&0xFF, (IP>>8)&0xFF, IP&0xFF, IP); // "/%d", , 0xFFFF);
    puts("");
    printf("CS:IP(16) [%02X.%02X:%02X.%02X] | [%03d.%03d:%03d.%03d] = %d:%d\n",// =%d
     (CS>>8)&0xFF, CS&0xFF, (IP>>8)&0xFF, IP&0xFF,
     (CS>>8)&0xFF, CS&0xFF, (IP>>8)&0xFF, IP&0xFF,
     CS, IP//, CS+IP-1
    );
    printf("CS+IP(16) [%02X.%02X]  [%03d.%03d] = %d\n",
     (CS+IP-1)>>8&0xFF, (CS+IP-1)&0xFF,
     (CS+IP-1)>>8&0xFF, (CS+IP-1)&0xFF,
     CS+IP-1
    );
    //printf("IP(16):[%02X|%02X] | [%03d|%03d] = %4d/%d\n", (IP>>8)&0xFF, IP&0xFF, (IP>>8)&0xFF, IP&0xFF, IP, 0xFFFF);
    puts("------------------------------------------");
    puts("    Z");
    printf("FR [%-1d]\n", ZF);
    //puts("    Z");
    puts("------");
    #endif
    goto *(*(instructions + *(opcode + IP))); // goto *instructions[opcode[IP]];
    //--------------------------------------------------------------------------------
    __000:
    __001:
    __002:
    __003:
    __004:
    __005:
    __006:
    __007:
    __008:
    __009:
    __010:
    __011:
    __012:
    __013:
    __014:
    __015:
    __016:
    __017:
    __018:
    __019:
    __020:
    __021:
    __022:
    __023:
    __024:
    __025:
    __026:
    __027:
    __028:
    __029:
    __030:
    __031:
    __032:
    __033:
    __034:
    __035:
    __036:
    __037:
    __038:
    __039:
    __040:
    __041:
    __042:
    __043:
    __044:
    __045:
    __046:
    __047:
    __048:
    __049:
    __050:
    __051:
    __052:
    __053:
    __054:
    __055:
    __056:
    __057:
    __058:
    __059:
    __060:
    __061:
    __062:
    __063:
    __064:
    __065:
    __066:
    __067:
    __068:
    __069:
    __070:
    __071:
    __072:
    __073:
    __074:
    __075:
    __076:
    __077:
    __078:
    __079:
    __080:
    __081:
    __082:
    __083:
    __084:
    __085:
    __086:
    __087:
    __088:
    __089:
    __090:
    __091:
    __092:
    __093:
    __094:
    __095:
    __096:
    __097:
    __098:
    __099:
    __100:
    __101:
    __102:
    __103:
    __104:
    __105:
    __106:
    __107:
    __108:
    __109:
    __110:
    __111:
    __112:
    __113:
    __114:
    __115:
    __116:
    __117:
    __118:
    __119:
    __120:
    __121:
    __122:
    __123:
    __124:
    __125:
    __126:
    __127:
    __128:
    __129:
    __130:
    __131:
    __132:
    __133:
    __134:
    __135:
    __136:
    __137:
    __138:
    __139:
    __140:
    __141:
    __142:
    __143:
    __144:
    __145:
    __146:
    __147:
    __148:
    __149:
    __150:
    __151:
    __152:
    __153:
    __154:
    __155:
    __156:
    __157:
    __158:
    __159:
    __160:
    __161:
    __162:
    __163:
    __164:
    __165:
    __166:
    __167:
    __168:
    __169:
    __170:
    __171:
    __172:
    __173:
    __174:
    __175:
    // Пересылка данных
    __176: // __B0: / MOV AL,
    {//#1
        //AX = opcode[++IP];//IP++;
        AX = (AX & 0xFF00) | opcode[++IP];  // обнуляем AL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#1
    __177: // __B1: / MOV CL,
    {//#2
        //CX = opcode[++IP];//IP++;
        CX = (CX & 0xFF00) | opcode[++IP];  // обнуляем CL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#2
    __178: // __B2: / MOV DL,
    {//#3
        //DX = opcode[++IP];//IP++;
        DX = (DX & 0xFF00) | opcode[++IP];  // обнуляем DL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#3
    __179: // __B3: / MOV BL,
    {//#4
        //BX = opcode[++IP];//IP++;
        BX = (BX & 0xFF00) | opcode[++IP];  // обнуляем BL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#4
    __180: // __B4: / MOV AH,
    {//#5
        //AX = opcode[++IP];//IP++;
        AX = (AX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем AH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#5
    __181: // __B5: / MOV CH,
    {//#6
        //CX = opcode[++IP];//IP++;
        CX = (CX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем CH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#6
    __182: // __B6: / MOV DH,
    {//#7
        //DX = opcode[++IP];//IP++;
        DX = (DX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем DH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#7
    __183: // __B7: / MOV BH,
    {//#8
        //BX = opcode[++IP];//IP++;
        BX = (BX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем BH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }//#8
    __184:
    {//#9

    }//#9
    __185:
    {//#10

    }//#10
    __186: // __BA: / MOV DX,
    {//#11
        //DX = opcode[++IP];//IP++;
        /*
        // Big-endian (DH = 1-й байт, DL = 2-й байт)
        DX = opcode[++IP] << 8;  // DH = первый байт (старший)
        DX |= opcode[++IP];      // DL = второй байт (младший)
        */
        //* 
        // Little-endian (DL = 1-й байт, DH = 2-й байт)
        DX = opcode[++IP];       // DL = первый байт (младший)
        DX |= opcode[++IP] << 8; // DH = второй байт (старший)
        //*/
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-2, IP-2, opcode[IP-2], opcode[IP-1], opcode[IP-2], opcode[IP-1], translate_opcode_into_symbolic_form[opcode[IP-2]], opcode[IP-1]);
        #endif
        IP++;
        //goto EXECUTE;
        goto STOP_vCPU;
    }//#11
    __187: // __BB: / MOV BX,
    {//#12
        //BX = opcode[++IP];//IP++;
        /*
        // Big-endian (BH = 1-й байт, BL = 2-й байт)
        BX = opcode[++IP] << 8;  // BH = первый байт (старший)
        BX |= opcode[++IP];      // BL = второй байт (младший)
        */
        //* 
        // Little-endian (BL = 1-й байт, BH = 2-й байт)
        BX = opcode[++IP];       // BL = первый байт (младший)
        BX |= opcode[++IP] << 8; // BH = второй байт (старший)
        //*/
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-2, IP-2, opcode[IP-2], opcode[IP-1], opcode[IP-2], opcode[IP-1], translate_opcode_into_symbolic_form[opcode[IP-2]], opcode[IP-1]);
        #endif
        IP++;
        goto EXECUTE;
    }//#12
    __188:
    __189:
    __190:
    __191:
    __192:
    __193:
    __194:
    __195:
    __196:
    __197:
    __198:
    __199:
    __200:
    __201:
    __202:
    __203:
    __204:
    __205: // __CD: / INT
    {
        void *functions[] =
        {
            &&functions_000,
            &&functions_001,
            &&functions_002,
            &&functions_003,
            &&functions_004,
            &&functions_005,
            &&functions_006,
            &&functions_007,
            &&functions_008,
            &&functions_009,
            &&functions_010,
            &&functions_011,
            &&functions_012,
            &&functions_013,
            &&functions_014,
            &&functions_015,
            &&functions_016,
            &&functions_017,
            &&functions_018,
            &&functions_019,
            &&functions_020,
            &&functions_021,
            &&functions_022,
            &&functions_023,
            &&functions_024,
            &&functions_025,
            &&functions_026,
            &&functions_027,
            &&functions_028,
            &&functions_029,
            &&functions_030,
            &&functions_031,
            &&functions_032,
            &&functions_033
        };
        goto *functions[opcode[++IP]];
        functions_000:
        {
        }
        functions_001:
        {
        }
        functions_002:
        {
        }
        functions_003:
        {
        }
        functions_004:
        {
        }
        functions_005:
        {
        }
        functions_006:
        {
        }
        functions_007:
        {
        }
        functions_008:
        {
        }
        functions_009:
        {
        }
        functions_010:
        {
        }
        functions_011:
        {
        }
        functions_012:
        {
        }
        functions_013:
        {
        }
        functions_014:
        {
        }
        functions_015:
        {
        }
        functions_016:
        {
        }
        functions_017:
        {
        }
        functions_018:
        {
        }
        functions_019:
        {
        }
        functions_020:
        {
        }
        functions_021:
        {
        }
        functions_022:
        {
        }
        functions_023:
        {
        }
        functions_024:
        {
        }
        functions_025:
        {
        }
        functions_026:
        {
        }
        functions_027:
        {
        }
        functions_028:
        {
        }
        functions_029:
        {
        }
        functions_030:
        {
        }
        functions_031:
        {
        }
        functions_032: // 20
        {
        }
        functions_033: // 21
        {
            while (opcode[++IP] != '\0') putchar(opcode[IP]);
            goto EXECUTE;
        }
    }
    __206:
    __207:
    __208:
    __209:
    __210:
    __211:
    __212:
    __213:
    __214:
    __215:
    __216:
    __217:
    __218:
    __219:
    __220:
    __221:
    __222:
    __223:
    __224:
    __225:
    __226:
    __227:
    __228:
    __229:
    __230:
    __231:
    __232:
    __233:
    __234:
    __235:
    __236:
    __237:
    __238:
    __239:
    __240:
    __241:
    __242:
    __243:
    __244:
    __245:
    __246:
    __247:
    __248:
    __249:
    __250:
    __251:
    __252:
    __253:
    __254:
    __255:
    __HLT: // 0 | Останавливает выполнение vCPU
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n", IP, IP, translate_opcode_into_symbolic_form[opcode[IP]], opcode[IP]);
    #endif
    IP++;
    goto STOP_vCPU; //break;
    //--------------------------------------------------------------------------------
    __INT: // 2 | Обращение к таблице векторных прерываний (IVT)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n", translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    void *functions[] =
    {
        &&__09h,
        &&__10h
    };
    goto *functions[opcode[IP]]; // goto *(*(functions + AX));
    // { 2
    __09h:
    //uch *ptr_str = &opcode[DX];
    //puts(ptr_str);
    goto EXECUTE;
    __10h:
    // } 2
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __NOP: // 3 | Заглушка
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s\t\t| %02X\n\n", IP, IP, translate_opcode_into_symbolic_form[opcode[IP]], opcode[IP]);
    //printf("\n%03d=%02X | %s\t\t| %02X", IP, IP, hex_to_string[opcode[IP]], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __MUL: // 4 | Умножение
    IP++;
    //GP *= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __DIV: // 5 | Деление
    IP++;
    //GP /= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%s, %02X\t| %02X %02X\n", translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __ADD: // 6 | Сложение
    IP++;
    //GP += opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __SUB: // 7 | Вычитание
    IP++;
    //GP -= opcode[IP];
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    //printf("\n%03d=%02X | %s, %02X\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __JMP: // 8 | Прыжок на метку (адрес)
    IP++;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X\t\t| %02X %02X", IP-1, IP-1, hex_to_string[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]); //
    #endif
    IP = opcode[IP];
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CALL: //  9 | Вызов процедуры
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __PUSH: // 10 | Положить на стек
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __POP: // 11 | Снять со стека
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __RET: // 12 | Возврат из процедуры
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    __CMP: // 13 | Сравнение
    if (opcode[++IP] == opcode[++IP]) ZF = true;
    else ZF = false;
    #if defined DEBUG_MODE
    printf("\n%03d=%02X | %s %02X %02X\t| %02X %02X %02X\n\n", IP-2, IP-2, translate_opcode_into_symbolic_form[opcode[IP-2]], opcode[IP-1], opcode[IP], opcode[IP-2], opcode[IP-1], opcode[IP]); // ,
    //printf("\n%03d=%02X | %s %02X %02X\t\t| %02X %02X %02X", IP-2, IP-2, hex_to_string[opcode[IP-2]], opcode[IP-1], opcode[IP], opcode[IP-2], opcode[IP-1], opcode[IP]);
    #endif
    IP++;
    goto EXECUTE;
    //--------------------------------------------------------------------------------
    STOP_vCPU:
    #if !defined DEBUG_MODE
    puts("-------------------------------------------");
    puts("\t HEX\t    DEC");
    puts("\t*H|*L(8)   *H|*L(8)");
    //puts("\tAH|AL(8)   AH|AL(8)");
    printf("AX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (AX>>8)&0xFF, AX&0xFF, (AX>>8)&0xFF, AX&0xFF, AX);//, hex_to_bin[AX]);
    //puts("\tBH|BL(8)   BH|BL(8)");
    printf("BX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (BX>>8)&0xFF, BX&0xFF, (BX>>8)&0xFF, BX&0xFF, BX);
    //puts("\tCH|CL(8)   CH|CL(8)");
    printf("CX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (CX>>8)&0xFF, CX&0xFF, (CX>>8)&0xFF, CX&0xFF, CX);
    //puts("\tDH|DL(8)   DH|DL(8)");
    printf("DX(16):[%02X.%02X] | [%03d.%03d] = %d\n", (DX>>8)&0xFF, DX&0xFF, (DX>>8)&0xFF, DX&0xFF, DX);
    puts("");
    printf("CS(16):[%02X.%02X] | [%03d.%03d] = %d\n", (CS>>8)&0xFF, CS&0xFF, (CS>>8)&0xFF, CS&0xFF, CS); // "/%d", , 0xFFFF);
    printf("IP(16):[%02X.%02X] | [%03d.%03d] = %d\n", (IP>>8)&0xFF, IP&0xFF, (IP>>8)&0xFF, IP&0xFF, IP); // "/%d", , 0xFFFF);
    puts("");
    printf("CS:IP(16) [%02X.%02X:%02X.%02X] | [%03d.%03d:%03d.%03d] = %d:%d\n",// =%d
     (CS>>8)&0xFF, CS&0xFF, (IP>>8)&0xFF, IP&0xFF,
     (CS>>8)&0xFF, CS&0xFF, (IP>>8)&0xFF, IP&0xFF,
     CS, IP//, CS+IP-1
    );
    printf("CS+IP(16) [%02X.%02X]  [%03d.%03d] = %d\n",
     (CS+IP-1)>>8&0xFF, (CS+IP-1)&0xFF,
     (CS+IP-1)>>8&0xFF, (CS+IP-1)&0xFF,
     CS+IP-1
    );
    //printf("IP(16):[%02X|%02X] | [%03d|%03d] = %4d/%d\n", (IP>>8)&0xFF, IP&0xFF, (IP>>8)&0xFF, IP&0xFF, IP, 0xFFFF);
    puts("------------------------------------------");
    puts("    Z");
    printf("FR [%-1d]\n", ZF);
    //puts("    Z");
    puts("------");
    puts("\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
    #endif
    puts("\n\n# DEBUG MODE OFF | РЕЖИМ ОТЛАДКИ ВЫКЛЮЧЕН #");
}