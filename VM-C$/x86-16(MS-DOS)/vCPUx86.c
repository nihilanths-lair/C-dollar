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
        &&instructions_000,
        &&instructions_001,
        &&instructions_002,
        &&instructions_003,
        &&instructions_004,
        &&instructions_005,
        &&instructions_006,
        &&instructions_007,
        &&instructions_008,
        &&instructions_009,
        &&instructions_010,
        &&instructions_011,
        &&instructions_012,
        &&instructions_013,
        &&instructions_014,
        &&instructions_015,
        &&instructions_016,
        &&instructions_017,
        &&instructions_018,
        &&instructions_019,
        &&instructions_020,
        &&instructions_021,
        &&instructions_022,
        &&instructions_023,
        &&instructions_024,
        &&instructions_025,
        &&instructions_026,
        &&instructions_027,
        &&instructions_028,
        &&instructions_029,
        &&instructions_030,
        &&instructions_031,
        &&instructions_032,
        &&instructions_033,
        &&instructions_034,
        &&instructions_035,
        &&instructions_036,
        &&instructions_037,
        &&instructions_038,
        &&instructions_039,
        &&instructions_040,
        &&instructions_041,
        &&instructions_042,
        &&instructions_043,
        &&instructions_044,
        &&instructions_045,
        &&instructions_046,
        &&instructions_047,
        &&instructions_048,
        &&instructions_049,
        &&instructions_050,
        &&instructions_051,
        &&instructions_052,
        &&instructions_053,
        &&instructions_054,
        &&instructions_055,
        &&instructions_056,
        &&instructions_057,
        &&instructions_058,
        &&instructions_059,
        &&instructions_060,
        &&instructions_061,
        &&instructions_062,
        &&instructions_063,
        &&instructions_064,
        &&instructions_065,
        &&instructions_066,
        &&instructions_067,
        &&instructions_068,
        &&instructions_069,
        &&instructions_070,
        &&instructions_071,
        &&instructions_072,
        &&instructions_073,
        &&instructions_074,
        &&instructions_075,
        &&instructions_076,
        &&instructions_077,
        &&instructions_078,
        &&instructions_079,
        &&instructions_080,
        &&instructions_081,
        &&instructions_082,
        &&instructions_083,
        &&instructions_084,
        &&instructions_085,
        &&instructions_086,
        &&instructions_087,
        &&instructions_088,
        &&instructions_089,
        &&instructions_090,
        &&instructions_091,
        &&instructions_092,
        &&instructions_093,
        &&instructions_094,
        &&instructions_095,
        &&instructions_096,
        &&instructions_097,
        &&instructions_098,
        &&instructions_099,
        &&instructions_100,
        &&instructions_101,
        &&instructions_102,
        &&instructions_103,
        &&instructions_104,
        &&instructions_105,
        &&instructions_106,
        &&instructions_107,
        &&instructions_108,
        &&instructions_109,
        &&instructions_110,
        &&instructions_111,
        &&instructions_112,
        &&instructions_113,
        &&instructions_114,
        &&instructions_115,
        &&instructions_116,
        &&instructions_117,
        &&instructions_118,
        &&instructions_119,
        &&instructions_120,
        &&instructions_121,
        &&instructions_122,
        &&instructions_123,
        &&instructions_124,
        &&instructions_125,
        &&instructions_126,
        &&instructions_127,
        &&instructions_128,
        &&instructions_129,
        &&instructions_130,
        &&instructions_131,
        &&instructions_132,
        &&instructions_133,
        &&instructions_134,
        &&instructions_135,
        &&instructions_136,
        &&instructions_137,
        &&instructions_138,
        &&instructions_139,
        &&instructions_140,
        &&instructions_141,
        &&instructions_142,
        &&instructions_143,
        &&instructions_144,
        &&instructions_145,
        &&instructions_146,
        &&instructions_147,
        &&instructions_148,
        &&instructions_149,
        &&instructions_150,
        &&instructions_151,
        &&instructions_152,
        &&instructions_153,
        &&instructions_154,
        &&instructions_155,
        &&instructions_156,
        &&instructions_157,
        &&instructions_158,
        &&instructions_159,
        &&instructions_160,
        &&instructions_161,
        &&instructions_162,
        &&instructions_163,
        &&instructions_164,
        &&instructions_165,
        &&instructions_166,
        &&instructions_167,
        &&instructions_168,
        &&instructions_169,
        &&instructions_170,
        &&instructions_171,
        &&instructions_172,
        &&instructions_173,
        &&instructions_174,
        &&instructions_175,
        &&instructions_176,
        &&instructions_177,
        &&instructions_178,
        &&instructions_179,
        &&instructions_180,
        &&instructions_181,
        &&instructions_182,
        &&instructions_183,
        &&instructions_184,
        &&instructions_185,
        &&instructions_186,
        &&instructions_187,
        &&instructions_188,
        &&instructions_189,
        &&instructions_190,
        &&instructions_191,
        &&instructions_192,
        &&instructions_193,
        &&instructions_194,
        &&instructions_195,
        &&instructions_196,
        &&instructions_197,
        &&instructions_198,
        &&instructions_199,
        &&instructions_200,
        &&instructions_201,
        &&instructions_202,
        &&instructions_203,
        &&instructions_204,
        &&instructions_205
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
    goto *instructions[opcode[++IP]];
    instructions_000:
    {
    }
    instructions_001:
    {
    }
    instructions_002:
    {
    }
    instructions_003:
    {
    }
    instructions_004:
    {
    }
    instructions_005:
    {
    }
    instructions_006:
    {
    }
    instructions_007:
    {
    }
    instructions_008:
    {
    }
    instructions_009:
    {
    }
    instructions_010:
    {
    }
    instructions_011:
    {
    }
    instructions_012:
    {
    }
    instructions_013:
    {
    }
    instructions_014:
    {
    }
    instructions_015:
    {
    }
    instructions_016:
    {
    }
    instructions_017:
    {
    }
    instructions_018:
    {
    }
    instructions_019:
    {
    }
    instructions_020:
    {
    }
    instructions_021:
    {
    }
    instructions_022:
    {
    }
    instructions_023:
    {
    }
    instructions_024:
    {
    }
    instructions_025:
    {
    }
    instructions_026:
    {
    }
    instructions_027:
    {
    }
    instructions_028:
    {
    }
    instructions_029:
    {
    }
    instructions_030:
    {
    }
    instructions_031:
    {
    }
    instructions_032:
    {
    }
    instructions_033:
    {
    }
    instructions_034:
    {
    }
    instructions_035:
    {
    }
    instructions_036:
    {
    }
    instructions_037:
    {
    }
    instructions_038:
    {
    }
    instructions_039:
    {
    }
    instructions_040:
    {
    }
    instructions_041:
    {
    }
    instructions_042:
    {
    }
    instructions_043:
    {
    }
    instructions_044:
    {
    }
    instructions_045:
    {
    }
    instructions_046:
    {
    }
    instructions_047:
    {
    }
    instructions_048:
    {
    }
    instructions_049:
    {
    }
    instructions_050:
    {
    }
    instructions_051:
    {
    }
    instructions_052:
    {
    }
    instructions_053:
    {
    }
    instructions_054:
    {
    }
    instructions_055:
    {
    }
    instructions_056:
    {
    }
    instructions_057:
    {
    }
    instructions_058:
    {
    }
    instructions_059:
    {
    }
    instructions_060:
    {
    }
    instructions_061:
    {
    }
    instructions_062:
    {
    }
    instructions_063:
    {
    }
    instructions_064:
    {
    }
    instructions_065:
    {
    }
    instructions_066:
    {
    }
    instructions_067:
    {
    }
    instructions_068:
    {
    }
    instructions_069:
    {
    }
    instructions_070:
    {
    }
    instructions_071:
    {
    }
    instructions_072:
    {
    }
    instructions_073:
    {
    }
    instructions_074:
    {
    }
    instructions_075:
    {
    }
    instructions_076:
    {
    }
    instructions_077:
    {
    }
    instructions_078:
    {
    }
    instructions_079:
    {
    }
    instructions_080:
    {
    }
    instructions_081:
    {
    }
    instructions_082:
    {
    }
    instructions_083:
    {
    }
    instructions_084:
    {
    }
    instructions_085:
    {
    }
    instructions_086:
    {
    }
    instructions_087:
    {
    }
    instructions_088:
    {
    }
    instructions_089:
    {
    }
    instructions_090:
    {
    }
    instructions_091:
    {
    }
    instructions_092:
    {
    }
    instructions_093:
    {
    }
    instructions_094:
    {
    }
    instructions_095:
    {
    }
    instructions_096:
    {
    }
    instructions_097:
    {
    }
    instructions_098:
    {
    }
    instructions_099:
    {
    }
    instructions_100:
    {
    }
    instructions_101:
    {
    }
    instructions_102:
    {
    }
    instructions_103:
    {
    }
    instructions_104:
    {
    }
    instructions_105:
    {
    }
    instructions_106:
    {
    }
    instructions_107:
    {
    }
    instructions_108:
    {
    }
    instructions_109:
    {
    }
    instructions_110:
    {
    }
    instructions_111:
    {
    }
    instructions_112:
    {
    }
    instructions_113:
    {
    }
    instructions_114:
    {
    }
    instructions_115:
    {
    }
    instructions_116:
    {
    }
    instructions_117:
    {
    }
    instructions_118:
    {
    }
    instructions_119:
    {
    }
    instructions_120:
    {
    }
    instructions_121:
    {
    }
    instructions_122:
    {
    }
    instructions_123:
    {
    }
    instructions_124:
    {
    }
    instructions_125:
    {
    }
    instructions_126:
    {
    }
    instructions_127:
    {
    }
    instructions_128:
    {
    }
    instructions_129:
    {
    }
    instructions_130:
    {
    }
    instructions_131:
    {
    }
    instructions_132:
    {
    }
    instructions_133:
    {
    }
    instructions_134:
    {
    }
    instructions_135:
    {
    }
    instructions_136:
    {
    }
    instructions_137:
    {
    }
    instructions_138:
    {
    }
    instructions_139:
    {
    }
    instructions_140:
    {
    }
    instructions_141:
    {
    }
    instructions_142:
    {
    }
    instructions_143:
    {
    }
    instructions_144:
    {
    }
    instructions_145:
    {
    }
    instructions_146:
    {
    }
    instructions_147:
    {
    }
    instructions_148:
    {
    }
    instructions_149:
    {
    }
    instructions_150:
    {
    }
    instructions_151:
    {
    }
    instructions_152:
    {
    }
    instructions_153:
    {
    }
    instructions_154:
    {
    }
    instructions_155:
    {
    }
    instructions_156:
    {
    }
    instructions_157:
    {
    }
    instructions_158:
    {
    }
    instructions_159:
    {
    }
    instructions_160:
    {
    }
    instructions_161:
    {
    }
    instructions_162:
    {
    }
    instructions_163:
    {
    }
    instructions_164:
    {
    }
    instructions_165:
    {
    }
    instructions_166:
    {
    }
    instructions_167:
    {
    }
    instructions_168:
    {
    }
    instructions_169:
    {
    }
    instructions_170:
    {
    }
    instructions_171:
    {
    }
    instructions_172:
    {
    }
    instructions_173:
    {
    }
    instructions_174:
    {
    }
    instructions_175:
    {
    }
    instructions_176: // __B0: / MOV AL,
    {
        //AX = opcode[++IP];//IP++;
        AX = (AX & 0xFF00) | opcode[++IP];  // обнуляем AL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_177: // __B1: / MOV CL,
    {
        //CX = opcode[++IP];//IP++;
        CX = (CX & 0xFF00) | opcode[++IP];  // обнуляем CL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_178: // __B2: / MOV DL,
    {
        //DX = opcode[++IP];//IP++;
        DX = (DX & 0xFF00) | opcode[++IP];  // обнуляем DL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_179: // __B3: / MOV BL,
    {
        //BX = opcode[++IP];//IP++;
        BX = (BX & 0xFF00) | opcode[++IP];  // обнуляем BL, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_180: // __B4: / MOV AH,
    {
        //AX = opcode[++IP];//IP++;
        AX = (AX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем AH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_181: // __B5: / MOV CH,
    {
        //CX = opcode[++IP];//IP++;
        CX = (CX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем CH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_182: // __B6: / MOV DH,
    {
        //DX = opcode[++IP];//IP++;
        DX = (DX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем DH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_183: // __B7: / MOV BH,
    {
        //BX = opcode[++IP];//IP++;
        BX = (BX & 0x00FF) | (opcode[++IP] << 8);  // обнуляем BH, записываем байт
        #if defined DEBUG_MODE
        //printf("\n%03d=%02X | %s, %02X\t| %02X %02X\n\n", IP-1, IP-1, translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP], opcode[IP-1], opcode[IP]);
        printf("\n%03d=%02X | %02X %02X | %03d %03d | %s, %d",     IP-1, IP-1, opcode[IP-1], opcode[IP], opcode[IP-1], opcode[IP], translate_opcode_into_symbolic_form[opcode[IP-1]], opcode[IP]);
        #endif
        IP++;
        goto EXECUTE;
    }
    instructions_184:
    {
    }
    instructions_185:
    {
    }
    instructions_186: // __BA: / MOV DX,
    {
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
    }
    instructions_187: // __BB: / MOV BX,
    {
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
    }
    instructions_188:
    {
    }
    instructions_189:
    {
    }
    instructions_190:
    {
    }
    instructions_191:
    {
    }
    instructions_192:
    {
    }
    instructions_193:
    {
    }
    instructions_194:
    {
    }
    instructions_195:
    {
    }
    instructions_196:
    {
    }
    instructions_197:
    {
    }
    instructions_198:
    {
    }
    instructions_199:
    {
    }
    instructions_200:
    {
    }
    instructions_201:
    {
    }
    instructions_202:
    {
    }
    instructions_203:
    {
    }
    instructions_204:
    {
    }
    instructions_205: // __CD: / INT
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