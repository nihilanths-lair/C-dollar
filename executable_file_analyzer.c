#include <stdio.h>
#include <locale.h>

#define fprint printf
#define print puts

int main(int argc, char *argv[])
{
    fprint("\n argc = %d\n", argc);
    for (int i = -1; ++i != argc;) fprint(" argv[%d] = %s\n", i, argv[i]);
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) { fprint(" #error -1\n"); return -1; }
    int offset = 0;
    print("");
    print("\14-----------------------------------------------------------------------------\14");
    print("|            |     Da\03ta                                                      |");
    print("|            |   on  |   in                                                   |");
    print("|            |  disk | memory                                                 |");
    print("|-----------------------------------------------------------------------------|");
    print("|                                ____________                                 |");
    print("|-------------------------------/ DOS HEADER \\--------------------------------|");
    unsigned char e_magic[2] = {0};
    fread(&e_magic, sizeof (e_magic), 1, file);
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = \"%c%c\"\t\t\t\t\t      |\n", offset, offset,
        e_magic[0],e_magic[1],  e_magic[1],e_magic[0],  e_magic[0],e_magic[1]
    );
    /*
    unsigned short e_magic = 0;
    fread(&e_magic, sizeof (e_magic), 1, file);
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %c%c\t\t\t\t\t      |\n", offset, offset,
     e_magic&0xFF, (e_magic>>8)&0xFF,
     (e_magic>>8)&0xFF, e_magic&0xFF,
     e_magic&0xFF, (e_magic>>8)&0xFF
    );
    */
    unsigned char e_cblp[2] = {getc(file), getc(file)};
    unsigned short short_e_cblp = e_cblp[0] | (e_cblp[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_cblp[0], e_cblp[1], e_cblp[1], e_cblp[0], short_e_cblp
    );
    // Длина образа (страниц)
    unsigned char e_cp[2] = {getc(file), getc(file)};
    unsigned short short_e_cp = e_cp[0] | (e_cp[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_cp[0], e_cp[1], e_cp[1], e_cp[0], short_e_cp
    );
    unsigned char e_crlc[2] = {getc(file), getc(file)};
    unsigned short short_e_crlc = e_crlc[0] | (e_crlc[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_crlc[0], e_crlc[1], e_crlc[1], e_crlc[0], short_e_crlc
    );
    // Длина заголовка в параграфах
    unsigned char e_cparhdr[2] = {getc(file), getc(file)};
    unsigned short short_e_cparhdr = e_cparhdr[0] | (e_cparhdr[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_cparhdr[0], e_cparhdr[1], e_cparhdr[1], e_cparhdr[0], short_e_cparhdr
    );
    // Минимум требуемой памяти
    unsigned char e_minalloc[2] = {getc(file), getc(file)};
    unsigned short short_e_minalloc = e_minalloc[0] | (e_minalloc[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_minalloc[0], e_minalloc[1], e_minalloc[1], e_minalloc[0], short_e_minalloc
    );
    // Максимум требуемой памяти
    unsigned char e_maxalloc[2] = {getc(file), getc(file)};
    unsigned short short_e_maxalloc = e_maxalloc[0] | (e_maxalloc[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_maxalloc[0], e_maxalloc[1], e_maxalloc[1], e_maxalloc[0], short_e_maxalloc
    );
    // Сегмент стека
    unsigned char e_ss[2] = {getc(file), getc(file)};
    unsigned short short_e_ss = e_ss[0] | (e_ss[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_ss[0], e_ss[1], e_ss[1], e_ss[0], short_e_ss
    );
    // Указатель стека
    unsigned char e_sp[2] = {getc(file), getc(file)};
    unsigned short short_e_sp = e_sp[0] | (e_sp[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_sp[0], e_sp[1], e_sp[1], e_sp[0], short_e_sp
    );
    unsigned char e_csum[2] = {getc(file), getc(file)};
    unsigned short short_e_csum = e_csum[0] | (e_csum[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_csum[0], e_csum[1], e_csum[1], e_csum[0], short_e_csum
    );
    // Указатель команд
    unsigned char e_ip[2] = {getc(file), getc(file)};
    unsigned short short_e_ip = e_ip[0] | (e_ip[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_ip[0], e_ip[1], e_ip[1], e_ip[0], short_e_ip
    );
    unsigned char e_cs[2] = {getc(file), getc(file)};
    unsigned short short_e_cs = e_cs[0] | (e_cs[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_cs[0], e_cs[1], e_cs[1], e_cs[0], short_e_cs
    );
    unsigned char e_lfarlc[2] = {getc(file), getc(file)};
    unsigned short short_e_lfarlc = e_lfarlc[0] | (e_lfarlc[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_lfarlc[0], e_lfarlc[1], e_lfarlc[1], e_lfarlc[0], short_e_lfarlc
    );
    unsigned char e_ovno[2] = {getc(file), getc(file)};
    unsigned short short_e_ovno = e_ovno[0] | (e_ovno[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_ovno[0], e_ovno[1], e_ovno[1], e_ovno[0], short_e_ovno
    );
    unsigned char e_res[4*2]/*8*/ =
    {
        getc(file), getc(file), // 1  00 00
        getc(file), getc(file), // 2  00 00
        getc(file), getc(file), // 3  00 00
        getc(file), getc(file), // 4  00 00
    };
    unsigned short short_e_res[4] = {0};
    // little-endian;
    short_e_res[0] = e_res[0] | (e_res[1] << 8);
    short_e_res[1] = e_res[2] | (e_res[3] << 8);
    short_e_res[2] = e_res[4] | (e_res[5] << 8);
    short_e_res[3] = e_res[6] | (e_res[7] << 8);
    print("|-----------------------------------------------------------------------------|");
    fprint("| %03d=%02X | 4x2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
              | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
              | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
              | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset,
     e_res[0], e_res[1], e_res[1], e_res[0], short_e_res[0],
     e_res[2], e_res[3], e_res[3], e_res[2], short_e_res[1],
     e_res[4], e_res[5], e_res[5], e_res[4], short_e_res[2],
     e_res[6], e_res[7], e_res[7], e_res[6], short_e_res[3]
    );
    print("|-----------------------------------------------------------------------------|");
    unsigned char e_oemid[2] = {getc(file), getc(file)};
    unsigned short short_e_oemid = e_oemid[0] | (e_oemid[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 8, offset, e_oemid[0], e_oemid[1], e_oemid[1], e_oemid[0], short_e_oemid
    );
    unsigned char e_oeminfo[2] = {getc(file), getc(file)};
    unsigned short short_e_oeminfo = e_oeminfo[0] | (e_oeminfo[1] << 8); // little-endian;
    fprint("| %03d=%02X | 2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset, e_oeminfo[0], e_oeminfo[1], e_oeminfo[1], e_oeminfo[0], short_e_oeminfo
    );
    unsigned char e_res2[10*2]/*20*/ =
    {
        getc(file), getc(file), //  1  00 00
        getc(file), getc(file), //  2  00 00
        getc(file), getc(file), //  3  00 00
        getc(file), getc(file), //  4  00 00
        getc(file), getc(file), //  5  00 00
        getc(file), getc(file), //  6  00 00
        getc(file), getc(file), //  7  00 00
        getc(file), getc(file), //  8  00 00
        getc(file), getc(file), //  9  00 00
        getc(file), getc(file)  // 10  00 00
    };
    unsigned short short_e_res2[10] = {0};
    // little-endian;
    short_e_res2[0] = e_res2[0] | (e_res2[1] << 8);
    short_e_res2[1] = e_res2[2] | (e_res2[3] << 8);
    short_e_res2[2] = e_res2[4] | (e_res2[5] << 8);
    short_e_res2[3] = e_res2[6] | (e_res2[7] << 8);
    short_e_res2[4] = e_res2[8] | (e_res2[9] << 8);
    short_e_res2[5] = e_res2[10] | (e_res2[11] << 8);
    short_e_res2[6] = e_res2[12] | (e_res2[13] << 8);
    short_e_res2[7] = e_res2[14] | (e_res2[15] << 8);
    short_e_res2[8] = e_res2[16] | (e_res2[17] << 8);
    short_e_res2[9] = e_res2[18] | (e_res2[19] << 8);
    print("|-----------------------------------------------------------------------------|");
    fprint("| %03d=%02X | 10x2 | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n|\
               | %02X %02X | %02X %02X = %d\t\t\t\t\t      |\n",
     offset += 2, offset,
     e_res2[0], e_res2[1], e_res2[1], e_res2[0], short_e_res2[0],
     e_res2[2], e_res2[3], e_res2[3], e_res2[2], short_e_res2[1],
     e_res2[4], e_res2[5], e_res2[5], e_res2[4], short_e_res2[2],
     e_res2[6], e_res2[7], e_res2[7], e_res2[6], short_e_res2[3],
     e_res2[8], e_res2[9], e_res2[9], e_res2[8], short_e_res2[4],
     e_res2[10], e_res2[11], e_res2[11], e_res2[10], short_e_res2[5],
     e_res2[12], e_res2[13], e_res2[13], e_res2[12], short_e_res2[6],
     e_res2[14], e_res2[15], e_res2[15], e_res2[14], short_e_res2[7],
     e_res2[16], e_res2[17], e_res2[17], e_res2[16], short_e_res2[8],
     e_res2[18], e_res2[19], e_res2[19], e_res2[18], short_e_res2[9]
    );
    print("|-----------------------------------------------------------------------------|");
    // Смещение PE-заголовка от начала
    unsigned char e_lfanew[4] = {getc(file), getc(file), getc(file), getc(file)};
    unsigned short short_e_lfanew = e_lfanew[0] | (e_lfanew[1] << 8);
    fprint("| %03d=%02X | 4 | %02X %02X %02X %02X | %02X %02X %02X %02X = %d\t\t\t\t      |\n",
     offset += 20, offset,
     e_lfanew[0], e_lfanew[1], e_lfanew[2], e_lfanew[3],
     e_lfanew[3], e_lfanew[2], e_lfanew[1], e_lfanew[0],
     short_e_lfanew
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                                 __________                                  |");
    print("|--------------------------------/ DOS STUB \\---------------------------------|");
    fseek(file, 0x40, SEEK_SET);
    unsigned char _0x40[80+1] = {0}; // _0x40[80] = '\0';

    _0x40[0] = getc(file);
    fprint("| %03d=%02X | %02X                                              | PUSH CS          |\n", offset += 4, offset, _0x40[0]);
    
    _0x40[1] = getc(file);
    fprint("| %03d=%02X | %02X                                              | POP DS           |\n", offset += 1, offset, _0x40[1]);
    
    _0x40[2] = getc(file),
    _0x40[3] = getc(file),
    _0x40[4] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X                                        | MOV DX, %02X|%02X (h)|\n", offset += 1, offset, _0x40[2], _0x40[3], _0x40[4], _0x40[3], _0x40[4]);
    
    _0x40[5] = getc(file),
    _0x40[6] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | MOV AH, %02X (h)   |\n", offset += 3, offset, _0x40[5], _0x40[6], _0x40[6]);
    
    _0x40[7] = getc(file),
    _0x40[8] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | INT %d # %02X (h)  |\n", offset += 2, offset, _0x40[7], _0x40[8], _0x40[8], _0x40[8]);
    
    _0x40[9] = getc(file),
    _0x40[10] = getc(file),
    _0x40[11] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X                                        | MOV AX, %02X|%02X (h)|\n", offset += 2, offset, _0x40[9], _0x40[10], _0x40[11], _0x40[10], _0x40[11]);
    
    _0x40[12] = getc(file),
    _0x40[13] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | INT %d # %02X (h)  |\n", offset += 3, offset, _0x40[12], _0x40[13], _0x40[13], _0x40[13]);
    print("|        |-------------------------------------------------|------------------|");

    for (int i = 13; ++i < 30;) _0x40[i] = getc(file);
    fprint("| %03d=%02X | ", offset += 2, offset);
    for (int i = 13; ++i < 30;) fprint("%02X ", _0x40[i]&255);
    fprint("| ");
    for (int i = 13; ++i < 30;) fprint("%c", _0x40[i]);
    fprint(" |");

    for (int i = 29; ++i < 46;) _0x40[i] = getc(file);
    fprint("\n| %03d=%02X | ", offset += 16, offset);
    for (int i = 29; ++i < 46;) fprint("%02X ", _0x40[i]&255);
    fprint("| ");
    for (int i = 29; ++i < 46;) fprint("%c", _0x40[i]);
    fprint(" |");

    for (int i = 45; ++i < 62;) _0x40[i] = getc(file);
    fprint("\n| %03d=%02X | ", offset += 16, offset);
    for (int i = 45; ++i < 62;) fprint("%02X ", _0x40[i]&255);
    fprint("| ");
    for (int i = 45; ++i < 62;)
    {
        if (_0x40[i] == 0x0D || _0x40[i] == 0x0A) continue;
        fprint("%c", _0x40[i]);
    }
    print("    |");
    _0x40[62] = getc(file);
    _0x40[63] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           |                  |\n", offset += 16, offset, _0x40[62]&255, _0x40[63]&255);
    print("|-----------------------------------------------------------------------------|");
    print("|                                 ___________                                 |");
    print("|--------------------------------/ NT HEADER \\--------------------------------|");
    // Сигнатура PE
    unsigned char signature[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Signature: %02X %02X %02X %02X                          | %s               |\n", offset += 2, offset,
     signature[0], signature[1], signature[2], signature[3],
     signature
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                          _________________________                          |");
    print("|-------------------------/ NT HEADER / FILE HEADER \\-------------------------|");
    // Архитектура
    unsigned char machine[2+1] = {getc(file), getc(file), '\0'};
    print("|        |-------------------------------------------------|                  |");
    fprint("| %03d=%02X | Machine: %02X %02X                                  | %s               |\n", offset += 4, offset,
     machine[0], machine[1],
     machine
    );
    print("|        |-------------------------------------------------|                  |");
    // Количество секций
    unsigned char number_of_sections[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Number of sections: %02X %02X                       | %c%c               |\n", offset += 2, offset,
     number_of_sections[0], number_of_sections[1],

     number_of_sections[0], number_of_sections[1]
    );
    print("|        |-------------------------------------------------|                  |");
    // Дата и время создания файла
    unsigned char time_date_stamp[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Time date stamp: %02X %02X %02X %02X                    | %c%c%c%c             |\n", offset += 2, offset,
     time_date_stamp[0], time_date_stamp[1], time_date_stamp[2], time_date_stamp[3],

     (time_date_stamp[0] == 0x0D) ? ' ' : time_date_stamp[0],
     (time_date_stamp[1] == 0x0D) ? ' ' : time_date_stamp[1],
     (time_date_stamp[2] == 0x0D) ? ' ' : time_date_stamp[2],
     (time_date_stamp[3] == 0x0D) ? ' ' : time_date_stamp[3]
    );
    print("|        |-------------------------------------------------|                  |");
    // Таблица символов
    unsigned char pointer_to_symbol_table[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Pointer to symbol table: %02X %02X %02X %02X            | %s                 |\n", offset += 4, offset,
     pointer_to_symbol_table[0], pointer_to_symbol_table[1], pointer_to_symbol_table[2], pointer_to_symbol_table[3],
     pointer_to_symbol_table
    );
    print("|        |-------------------------------------------------|                  |");
    // Количество символов
    unsigned char number_of_symbols[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Number of symbols: %02X %02X %02X %02X                  | %s               |\n", offset += 4, offset,
     number_of_symbols[0], number_of_symbols[1], number_of_symbols[2], number_of_symbols[3],
     number_of_symbols
    );
    print("|        |-------------------------------------------------|                  |");
    // Размер следующего подзаголовка
    unsigned char size_of_optional_header[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of optional header: %02X %02X                  | %s                |\n", offset += 4, offset,
     size_of_optional_header[0], size_of_optional_header[1],
     size_of_optional_header
    );
    print("|        |-------------------------------------------------|                  |");
    // Характеристики файла
    unsigned char characteristics[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Characteristics: %02X %02X                          | %s                |\n", offset += 2, offset,
     characteristics[0], characteristics[1],
     characteristics
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                        _____________________________                        |");
    print("|-----------------------/ NT HEADER / OPTIONAL HEADER \\-----------------------|");
    // Название формата (PE32)
    unsigned char magic[2+1] = {getc(file), getc(file), '\0'};
    print("|        |-------------------------------------------------|                  |");
    fprint("| %03d=%02X | Magic: %02X %02X                                    | %s               |\n", offset += 2, offset,
     magic[0],
     magic[1],
     magic
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char major_linker_version = getc(file);
    fprint("| %03d=%02X | Major linker version: %02X                        | %c                |\n", offset += 2, offset,
     major_linker_version, major_linker_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char minor_linker_version = getc(file);
    fprint("| %03d=%02X | Minor linker version: %02X                        | %c                |\n", offset += 1, offset,
     minor_linker_version, minor_linker_version
    );
    print("|        |-------------------------------------------------|                  |");
    // Размер секций кода
    unsigned char size_of_code[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of code: %02X %02X %02X %02X                       | %s                 |\n", offset += 1, offset,
     size_of_code[0],
     size_of_code[1],
     size_of_code[2],
     size_of_code[3],
     size_of_code
    );
    print("|        |-------------------------------------------------|                  |");
    // Размер секций иниц. данных
    unsigned char size_of_initialized_data[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of initialized data: %02X %02X %02X %02X           | %s                 |\n", offset += 4, offset,
     size_of_initialized_data[0],
     size_of_initialized_data[1],
     size_of_initialized_data[2],
     size_of_initialized_data[3],
     size_of_initialized_data
    );
    print("|        |-------------------------------------------------|                  |");
    // Размер секций не иниц. данных
    unsigned char size_of_unininitialized_data[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of unininitialized data: %02X %02X %02X %02X       | %s                 |\n", offset += 4, offset,
     size_of_unininitialized_data[0],
     size_of_unininitialized_data[1],
     size_of_unininitialized_data[2],
     size_of_unininitialized_data[3],
     size_of_unininitialized_data
    );
    print("|        |-------------------------------------------------|                  |");
    // RVA точки входа в программу
    unsigned char address_of_entry_point[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Address of entry point: %02X %02X %02X %02X             | %s               |\n", offset += 4, offset,
     address_of_entry_point[0],
     address_of_entry_point[1],
     address_of_entry_point[2],
     address_of_entry_point[3],
     address_of_entry_point
    );
    print("|        |-------------------------------------------------|                  |");
    // RVA секции с кодом
    unsigned char base_of_code[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Base of code: %02X %02X %02X %02X                       | %s                 |\n", offset += 4, offset,
     base_of_code[0],
     base_of_code[1],
     base_of_code[2],
     base_of_code[3],
     base_of_code
    );
    print("|        |-------------------------------------------------|                  |");
    // RVA секции с данными
    unsigned char base_of_data[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Base of data: %02X %02X %02X %02X                       | %s                 |\n", offset += 4, offset,
     base_of_data[0],
     base_of_data[1],
     base_of_data[2],
     base_of_data[3],
     base_of_data
    );
    print("|        |-------------------------------------------------|                  |");
    // Кратно 64 Кб. (4 Мб.)
    unsigned char image_base[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Image base: %02X %02X %02X %02X                         | %s                |\n", offset += 4, offset,
     image_base[0],
     image_base[1],
     image_base[2],
     image_base[3],
     image_base
    );
    print("|        |-------------------------------------------------|                  |");
    // Выравнивание в вирт. памяти (4 Кб.)
    unsigned char section_alignment[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Section alignment: %02X %02X %02X %02X                  | %s                 |\n", offset += 4, offset,
     section_alignment[0],
     section_alignment[1],
     section_alignment[2],
     section_alignment[3],
     section_alignment
    );
    print("|        |-------------------------------------------------|                  |");
    // Выравнивание в exe файле (512 б.)
    unsigned char file_alignment[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | File alignment: %02X %02X %02X %02X                     | %s                 |\n", offset += 4, offset,
     file_alignment[0],
     file_alignment[1],
     file_alignment[2],
     file_alignment[3],
     file_alignment
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char major_operating_system_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Major operating system version: %02X %02X           | %s                |\n", offset += 2, offset,
     major_operating_system_version[0],
     major_operating_system_version[1],
     major_operating_system_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char minor_operating_system_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Minor operating system version: %02X %02X           | %s                 |\n", offset += 2, offset,
     minor_operating_system_version[0],
     minor_operating_system_version[1],
     minor_operating_system_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char major_image_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Major image version: %02X %02X                      | %s                 |\n", offset += 2, offset,
     major_image_version[0],
     major_image_version[1],
     major_image_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char minor_image_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Minor image version: %02X %02X                      | %s                 |\n", offset += 2, offset,
     minor_image_version[0],
     minor_image_version[1],
     minor_image_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char major_subsystem_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Major subsystem version: %02X %02X                  | %s                |\n", offset += 2, offset,
     major_subsystem_version[0],
     major_subsystem_version[1],
     major_subsystem_version
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char minor_subsystem_version[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Minor subsystem version: %02X %02X                  | %s                |\n", offset += 2, offset,
     minor_subsystem_version[0],
     minor_subsystem_version[1],
     minor_subsystem_version
    );
    print("|        |-------------------------------------------------|                  |");
    // Зарезервировано
    unsigned char win32_version_value[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Win32 version value: %02X %02X %02X %02X                | %s                 |\n", offset += 2, offset,
     win32_version_value[0],
     win32_version_value[1],
     win32_version_value[2],
     win32_version_value[3],
     win32_version_value
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char size_of_image[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of image: %02X %02X %02X %02X                      | %s                 |\n", offset += 4, offset,
     size_of_image[0],
     size_of_image[1],
     size_of_image[2],
     size_of_image[3],
     size_of_image
    );
    print("|        |-------------------------------------------------|                  |");
    // Размер всех заголовков
    unsigned char size_of_headers[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of headers: %02X %02X %02X %02X                    | %s                 |\n", offset += 4, offset,
     size_of_headers[0],
     size_of_headers[1],
     size_of_headers[2],
     size_of_headers[3],
     size_of_headers
    );
    print("|        |-------------------------------------------------|                  |");
    // Контрольная сумма
    unsigned char check_sum[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Check sum: %02X %02X %02X %02X                          | %s              |\n", offset += 4, offset,
     check_sum[0],
     check_sum[1],
     check_sum[2],
     check_sum[3],
     check_sum
    );
    print("|        |-------------------------------------------------|                  |");
    // Тип программы
    unsigned char subsystem[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Subsystem: %02X %02X                                | %s                |\n", offset += 4, offset,
     subsystem[0],
     subsystem[1],
     subsystem
    );
    print("|        |-------------------------------------------------|                  |");
    // Хар-ки DLL-файла
    unsigned char dll_characteristics[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Dll characteristics: %02X %02X                      | %s               |\n", offset += 2, offset,
     dll_characteristics[0],
     dll_characteristics[1],
     dll_characteristics
    );
    print("|        |-------------------------------------------------|                  |");
    // Зарезерв. место под стек (1 Мб.)
    unsigned char size_of_stack_reserve[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of stack reserve: %02X %02X %02X %02X              | %s                 |\n", offset += 2, offset,
     size_of_stack_reserve[0],
     size_of_stack_reserve[1],
     size_of_stack_reserve[2],
     size_of_stack_reserve[3],
     size_of_stack_reserve
    );
    print("|        |-------------------------------------------------|                  |");
    // Изначальный размер стека (4 Кб.)
    unsigned char size_of_stack_commit[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of stack commit: %02X %02X %02X %02X               | %s                 |\n", offset += 4, offset,
     size_of_stack_commit[0],
     size_of_stack_commit[1],
     size_of_stack_commit[2],
     size_of_stack_commit[3],
     size_of_stack_commit
    );
    print("|        |-------------------------------------------------|                  |");
    // Зарезерв. место под кучу (1 Мб.)
    unsigned char size_of_heap_reserve[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of heap reserve: %02X %02X %02X %02X               | %s                 |\n", offset += 4, offset,
     size_of_heap_reserve[0],
     size_of_heap_reserve[1],
     size_of_heap_reserve[2],
     size_of_heap_reserve[3],
     size_of_heap_reserve
    );
    print("|        |-------------------------------------------------|                  |");
    // Изначальный размер кучи (4 Кб.)
    unsigned char size_of_heap_commit[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Size of heap commit: %02X %02X %02X %02X                | %s                 |\n", offset += 4, offset,
     size_of_heap_commit[0],
     size_of_heap_commit[1],
     size_of_heap_commit[2],
     size_of_heap_commit[3],
     size_of_heap_commit
    );
    print("|        |-------------------------------------------------|                  |");
    // Не используется
    unsigned char loader_flags[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Loader flags: %02X %02X %02X %02X                       | %s                 |\n", offset += 4, offset,
     loader_flags[0],
     loader_flags[1],
     loader_flags[2],
     loader_flags[3],
     loader_flags
    );
    print("|        |-------------------------------------------------|                  |");
    unsigned char number_of_rva_and_sizes[4+1] = {getc(file), getc(file), getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | Number of RVA and sizes: %02X %02X %02X %02X            | %s                 |\n", offset += 4, offset,
     number_of_rva_and_sizes[0],
     number_of_rva_and_sizes[1],
     number_of_rva_and_sizes[2],
     number_of_rva_and_sizes[3],
     number_of_rva_and_sizes
    );
    print("|-----------------------------------------------------------------------------|");
    print("|               ______________________________________________                |");
    print("|--------------/ NT HEADER / OPTIONAL HEADER / DATA DIRECTORY \\---------------|");
    print("|-----------------------------------------------------------------------------|");
    print("|         | Virtual address | Size                                            |");
    print("|                                                                             |");
    fprint("| %03d=%02X  | -- -- -- --     | -- -- -- --                                     |\n", offset += 4, offset);
    fprint("| %03d=%02X  | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    fprint("| %03d=%02X | -- -- -- --     | -- -- -- --                                     |\n", offset += 8, offset);
    print("|-----------------------------------------------------------------------------|");
    print("|                              ________________                               |");
    print("|-----------------------------/ SECTION HEADER \\------------------------------|");
    print("|-----------------------------------------------------------------------------|");
    print("|                                 __________                                  |");
    print("|--------------------------------/ SECTIONS \\---------------------------------|");
    print("|-----------------------------------------------------------------------------|");
    print("|_____________________________________________________________________________|");
    
    setlocale(0, "");
    void *labels[] = {&&label0, &&label1, &&label2};
    int i = 2;
    goto *labels[i]; // переход по вычисляемой метке
    label0:
     printf("Это метка 0\n");
     return 0;
    label1:
     printf("Это метка 1\n");
     return 0;
    label2:
     printf("Это метка 2\n");
     return 0;
}