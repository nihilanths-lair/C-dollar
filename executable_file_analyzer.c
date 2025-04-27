#include <stdio.h>
#define fprint printf
#define print puts
/*char *hex_to_str(const char *str)
{
    char new_str[(16*2)+1];
    for (int i = -1; str[++i] != '\0';) new_str[i] = str[i];
    return new_str;
}*/
int main(int argc, char *argv[])
{
    fprint("\n argc = %d\n", argc);
    for (int i = -1; ++i != argc;) fprint(" argv[%d] = %s\n", i, argv[i]);
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) { fprint(" #error -1\n"); return -1; }
    int offset = 0;
    print("                                 ____________");
    print("|-------------------------------/ DOS HEADER \\--------------------------------|");
    unsigned char e_magic[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                  |\n", offset, offset,
     e_magic[0],
     e_magic[1],
     e_magic
    );
    unsigned char e_cblp[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset,
     e_cblp[0],
     e_cblp[1],
     e_cblp
    );
    // Длина образа (страниц)
    unsigned char e_cp[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset,
     e_cp[0],
     e_cp[1],
     e_cp
    );
    unsigned char e_crlc[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_crlc[0],
     e_crlc[1],
     e_crlc
    );
    // Длина заголовка в параграфах
    unsigned char e_cparhdr[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset,
     e_cparhdr[0],
     e_cparhdr[1],
     e_cparhdr
    );
    // Минимум требуемой памяти
    unsigned char e_minalloc[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_minalloc[0],
     e_minalloc[1],
     e_minalloc
    );
    // Максимум требуемой памяти
    unsigned char e_maxalloc[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                  |\n", offset += 2, offset,
     e_maxalloc[0],
     e_maxalloc[1],
     e_maxalloc
    );
    // Сегмент стека
    unsigned char e_ss[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_ss[0],
     e_ss[1],
     e_ss
    );
    // Указатель стека
    unsigned char e_sp[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset,
     e_sp[0],
     e_sp[1],
     e_sp
    );
    unsigned char e_csum[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_csum[0],
     e_csum[1],
     e_csum
    );
    // Указатель команд
    unsigned char e_ip[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_ip[0],
     e_ip[1],
     e_ip
    );
    unsigned char e_cs[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_cs[0],
     e_cs[1],
     e_cs
    );
    unsigned char e_lfarlc[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset,
     e_lfarlc[0],
     e_lfarlc[1],
     e_lfarlc
    );
    unsigned char e_ovno[2+1] = {getc(file), getc(file), '\0'};
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_ovno[0],
     e_ovno[1],
     e_ovno
    );
    unsigned char e_res_4_[4*2+1]/*8*/ =
    {
        getc(file), getc(file),
        getc(file), getc(file),
        getc(file), getc(file),
        getc(file), getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X %02X %02X %02X %02X                      | %s                    |\n", offset += 2, offset,
     e_res_4_[0],
     e_res_4_[1],
     e_res_4_[2],
     e_res_4_[3],
     e_res_4_[4],
     e_res_4_[5],
     e_res_4_[6],
     e_res_4_[7],
     e_res_4_
    );
    unsigned char e_oemid[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 8, offset,
     e_oemid[0],
     e_oemid[1],
     e_oemid
    );
    unsigned char e_oeminfo[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset,
     e_oeminfo[0],
     e_oeminfo[1],
     e_oeminfo
    );
    unsigned char e_res2_10_[10*2+1]/*20*/ =
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
        getc(file), getc(file), // 10  00 00
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X |                     |\n|\
        | %02X %02X %02X %02X %02X                               | %s                    |\n", offset += 2, offset,
     e_res2_10_ [0], e_res2_10_ [1], //  1  00 00
     e_res2_10_ [2], e_res2_10_ [3], //  2  00 00
     e_res2_10_ [4], e_res2_10_ [5], //  3  00 00
     e_res2_10_ [6], e_res2_10_ [7], //  4  00 00
     e_res2_10_ [8], e_res2_10_ [9], //  5  00 00
     e_res2_10_[10], e_res2_10_[11], //  6  00 00
     e_res2_10_[12], e_res2_10_[13], //  7  00 00
     e_res2_10_[14], e_res2_10_[15], //  8  00 00
     e_res2_10_[16], e_res2_10_[17], //  9  00 00
     e_res2_10_[18], e_res2_10_[19], // 10  00 00
     e_res2_10_
    );
    // Смещение PE-заголовка от начала
    unsigned char e_lfanew[4+1] =
    {
        getc(file),
        getc(file),
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X                                  | %s                   |\n", offset += 20, offset,
     e_lfanew[0],
     e_lfanew[1],
     e_lfanew[2],
     e_lfanew[3],
     e_lfanew
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
    unsigned char signature[4+1] =
    {
        getc(file),
        getc(file),
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s               |\n", offset += 2, offset,
     signature[0],
     signature[1],
     signature[2],
     signature[3],
     signature
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                                _____________             |                  |");
    print("|-------------------------------/ FILE HEADER \\-------------------------------|");
    // Архитектура
    unsigned char machine[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                           | %s               |\n", offset += 2, offset,
     machine[0],
     machine[1],
     machine
    );
    // Количество секций
    unsigned char number_of_sections[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                           | %c%c               |\n", offset += 2, offset,
     number_of_sections[0],
     number_of_sections[1],

     number_of_sections[0],
     number_of_sections[1]
    );
    // Дата и время создания файла
    unsigned char time_date_stamp[4+1] =
    {
        getc(file),
        getc(file),
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %c%c%c%c             |\n", offset += 2, offset,
     time_date_stamp[0],
     time_date_stamp[1],
     time_date_stamp[2],
     time_date_stamp[3],

     (time_date_stamp[0] == 0x0D) ? ' ' : time_date_stamp[0],
     (time_date_stamp[1] == 0x0D) ? ' ' : time_date_stamp[1],
     (time_date_stamp[2] == 0x0D) ? ' ' : time_date_stamp[2],
     (time_date_stamp[3] == 0x0D) ? ' ' : time_date_stamp[3]
    );
    // Таблица символов
    unsigned char pointer_to_symbol_table[4+1] =
    {
        getc(file),
        getc(file),
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s                 |\n", offset += 4, offset,
     pointer_to_symbol_table[0],
     pointer_to_symbol_table[1],
     pointer_to_symbol_table[2],
     pointer_to_symbol_table[3],
     pointer_to_symbol_table
    );
    // Количество символов
    unsigned char number_of_symbols[4+1] =
    {
        getc(file),
        getc(file),
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s               |\n", offset += 4, offset,
     number_of_symbols[0],
     number_of_symbols[1],
     number_of_symbols[2],
     number_of_symbols[3],
     number_of_symbols
    );
    // Размер следующего подзаголовка
    unsigned char size_of_optional_header[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                           | %s                |\n", offset += 4, offset,
     size_of_optional_header[0],
     size_of_optional_header[1],
     size_of_optional_header
    );
    // Характеристики файла
    unsigned char characteristics[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                           | %s                |\n", offset += 2, offset,
     characteristics[0],
     characteristics[1],
     characteristics
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                              _________________           |                  |");
    print("|-----------------------------/ OPTIONAL HEADER \\-----------------------------|");
    unsigned char magic[2+1] =
    {
        getc(file),
        getc(file),
        '\0'
    };
    fprint("| %03d=%02X | %02X %02X                                           | %s               |\n", offset += 2, offset,
     magic[0],
     magic[1],
     magic
    );
    unsigned char major_linker_version = getc(file);
    fprint("| %03d=%02X | %02X                                              | %c                |\n", offset += 2, offset,
     major_linker_version,
     major_linker_version
    );
    unsigned char minor_linker_version = getc(file);
    fprint("| %03d=%02X | %02X                                              | %c                |\n", offset += 1, offset,
     minor_linker_version,
     minor_linker_version
    );
    print("|-----------------------------------------------------------------------------|");
    return 0;
}