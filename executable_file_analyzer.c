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
    char e_magic[2+1] = {0}; // MZ
    e_magic[0] = getc(file); // M
    e_magic[1] = getc(file); // Z
    fprint("| %03d=%02X | %02X %02X                                        | %s                  |\n", offset, offset, e_magic[0]&255, e_magic[1]&255, e_magic);
    
    char e_cblp[2+1] = {0};  // h=02
    e_cblp[0] = getc(file), e_cblp[1] = getc(file); // Необязательное поле
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset, e_cblp[0]&255, e_cblp[1]&255, e_cblp);
    
    char e_cp[2+1] = {0};   // h=04
    e_cp[0] = getc(file), e_cp[1] = getc(file); // Длина образа (страниц)
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset, e_cp[0]&255, e_cp[1]&255, e_cp);
    
    char e_crlc[2+1] = {0}; // h=06
    e_crlc[0] = getc(file), e_crlc[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_crlc[0]&255, e_crlc[1]&255, e_crlc);
    
    char e_cparhdr[2+1] = {0}; // Длина заголовка в параграфах (выставить значение)
    e_cparhdr[0] = getc(file), e_cparhdr[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset, e_cparhdr[0]&255, e_cparhdr[1]&255, e_cparhdr);
    
    char e_minalloc[2+1] = {0}; // Минимум требуемой памяти (необязательный параметр)
    e_minalloc[0] = getc(file), e_minalloc[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_minalloc[0]&255, e_minalloc[1]&255, e_minalloc);
    
    char e_maxalloc[2+1] = {0}; // Максимум требуемой памяти (выставить значение)
    e_maxalloc[0] = getc(file), e_maxalloc[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                  |\n", offset += 2, offset, e_maxalloc[0]&255, e_maxalloc[1]&255, e_maxalloc);
    
    char e_ss[2+1] = {0}; // Сегмент стека
    e_ss[0] = getc(file), e_ss[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_ss[0]&255, e_ss[1]&255, e_ss);
    
    char e_sp[2+1] = {0}; // Указатель стека
    e_sp[0] = getc(file), e_sp[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset, e_sp[0]&255, e_sp[1]&255, e_sp);
    
    char e_csum[2+1] = {0}; // Необязательный параметр
    e_csum[0] = getc(file), e_csum[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_csum[0]&255, e_csum[1]&255, e_csum);
    
    char e_ip[2+1] = {0}; // Указатель команд
    e_ip[0] = getc(file), e_ip[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_ip[0]&255, e_ip[1]&255, e_ip);
    
    char e_cs[2+1] = {0}; // Необязательный параметр
    e_cs[0] = getc(file), e_cs[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_cs[0]&255, e_cs[1]&255, e_cs);
    
    char e_lfarlc[2+1] = {0}; // Необязательный параметр
    e_lfarlc[0] = getc(file), e_lfarlc[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                   |\n", offset += 2, offset, e_lfarlc[0]&255, e_lfarlc[1]&255, e_lfarlc);
    
    char e_ovno[2+1] = {0}; // Необязательный параметр
    e_ovno[0] = getc(file), e_ovno[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_ovno[0]&255, e_ovno[1]&255, e_ovno);

    char e_res_4_[4*2+1]/*8*/ = {0}; // Необязательный параметр
    e_res_4_[0] = getc(file), e_res_4_[1] = getc(file),
    e_res_4_[2] = getc(file), e_res_4_[3] = getc(file),
    e_res_4_[4] = getc(file), e_res_4_[5] = getc(file),
    e_res_4_[6] = getc(file), e_res_4_[7] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X %02X %02X %02X %02X %02X                      | %s                    |\n", offset += 2, offset, 
     e_res_4_[0]&255, e_res_4_[1]&255, e_res_4_[2]&255, e_res_4_[3]&255,
     e_res_4_[4]&255, e_res_4_[5]&255, e_res_4_[6]&255, e_res_4_[7]&255,
     e_res_4_
    );

    char e_oemid[2+1] = {0}; // Необязательный параметр
    e_oemid[0] = getc(file), e_oemid[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 8, offset, e_oemid[0]&255, e_oemid[1]&255, e_oemid);

    char e_oeminfo[2+1] = {0}; // Необязательный параметр
    e_oeminfo[0] = getc(file), e_oeminfo[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                        | %s                    |\n", offset += 2, offset, e_oeminfo[0]&255, e_oeminfo[1]&255, e_oeminfo);

    char e_res2_10_[10*2+1]/*20*/ = {0}; // Необязательный параметр
    e_res2_10_[0] = getc(file), e_res2_10_[1] = getc(file); //  1  00 00
    e_res2_10_[2] = getc(file), e_res2_10_[3] = getc(file); //  2  00 00
    e_res2_10_[4] = getc(file), e_res2_10_[5] = getc(file); //  3  00 00
    e_res2_10_[6] = getc(file), e_res2_10_[7] = getc(file); //  4  00 00
    e_res2_10_[8] = getc(file), e_res2_10_[9] = getc(file); //  5  00 00
    e_res2_10_[10] = getc(file), e_res2_10_[11] = getc(file); //  6  00 00
    e_res2_10_[12] = getc(file), e_res2_10_[13] = getc(file); //  7  00 00
    e_res2_10_[14] = getc(file), e_res2_10_[15] = getc(file); //  8  00 00
    e_res2_10_[16] = getc(file), e_res2_10_[17] = getc(file); //  9  00 00
    e_res2_10_[18] = getc(file), e_res2_10_[19] = getc(file); // 10  00 00
    //print("|                                                        +-----------+            |");
    fprint("| %03d=%02X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X |                     |\n|\
        | %02X %02X %02X %02X %02X                               | %s                    |\n", offset += 2, offset,
     e_res2_10_[0]&255, e_res2_10_[1]&255, //  1  00 00
     e_res2_10_[2]&255, e_res2_10_[3]&255, //  2  00 00
     e_res2_10_[4]&255, e_res2_10_[5]&255, //  3  00 00
     e_res2_10_[6]&255, e_res2_10_[7]&255, //  4  00 00
     e_res2_10_[8]&255, e_res2_10_[9]&255, //  5  00 00
     e_res2_10_[10]&255, e_res2_10_[11]&255, //  6  00 00
     e_res2_10_[12]&255, e_res2_10_[13]&255, //  7  00 00
     e_res2_10_[14]&255, e_res2_10_[15]&255, //  8  00 00
     e_res2_10_[16]&255, e_res2_10_[17]&255, //  9  00 00
     e_res2_10_[18]&255, e_res2_10_[19]&255, // 10  00 00
     e_res2_10_
    );

    char e_lfanew[4+1] = {0}; // Смещение PE-заголовка от начала
    e_lfanew[0] = getc(file), e_lfanew[1] = getc(file), e_lfanew[2] = getc(file), e_lfanew[3] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X %02X                                  | %s                   |\n", offset += 20, offset, e_lfanew[0]&255, e_lfanew[1]&255, e_lfanew[2]&255, e_lfanew[3]&255, e_lfanew);

    print("|-----------------------------------------------------------------------------|");
    print("|                                 __________                                  |");
    print("|--------------------------------/ DOS STUB \\---------------------------------|");
    fseek(file, 0x40, SEEK_SET);
    char _0x40[80+1] = {0}; // _0x40[80] = '\0';

    _0x40[0] = getc(file);
    fprint("| %03d=%02X | %02X                                              | PUSH CS          |\n", offset += 4, offset, _0x40[0]&255);
    _0x40[1] = getc(file);
    fprint("| %03d=%02X | %02X                                              | POP DS           |\n", offset += 1, offset, _0x40[1]&255);
    _0x40[2] = getc(file), _0x40[3] = getc(file), _0x40[4] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X                                        | MOV DX, %02X|%02X (h)|\n", offset += 1, offset, _0x40[2]&255, _0x40[3]&255, _0x40[4]&255, _0x40[3]&255, _0x40[4]&255);
    _0x40[5] = getc(file), _0x40[6] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | MOV AH, %02X (h)   |\n", offset += 3, offset, _0x40[5]&255, _0x40[6]&255, _0x40[6]&255);
    _0x40[7] = getc(file), _0x40[8] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | INT %d # %02X (h)  |\n", offset += 2, offset, _0x40[7]&255, _0x40[8]&255, _0x40[8], _0x40[8]&255);
    _0x40[9] = getc(file), _0x40[10] = getc(file), _0x40[11] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X                                        | MOV AX, %02X|%02X (h)|\n", offset += 2, offset, _0x40[9]&255, _0x40[10]&255, _0x40[11]&255, _0x40[10]&255, _0x40[11]&255);
    _0x40[12] = getc(file), _0x40[13] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | INT %d # %02X (h)  |\n", offset += 3, offset, _0x40[12]&255, _0x40[13]&255, _0x40[13], _0x40[13]&255);
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
        if (_0x40[i] == 13 || _0x40[i] == 10) continue;
        fprint("%c", _0x40[i]);
    }
    print("    |");
    _0x40[62] = getc(file);
    _0x40[63] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           |                  |\n", offset += 16, offset, _0x40[62]&255, _0x40[63]&255);
    print("|-----------------------------------------------------------------------------|");
    print("|                                 ___________                                 |");
    print("|--------------------------------/ NT HEADER \\--------------------------------|");
    char signature[4+1] = {0}; // PE\0\0
    signature[0] = getc(file); // P
    signature[1] = getc(file); // E
    signature[2] = getc(file); // \0
    signature[3] = getc(file); // \0
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s               |\n", offset += 2, offset,
     signature[0]&255,
     signature[1]&255,
     signature[2]&255,
     signature[3]&255,
     signature
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                                _____________             |                  |");
    print("|-------------------------------/ FILE HEADER \\-------------------------------|");
    char machine[2+1] = {0}; // Архитектура
    machine[0] = getc(file);
    machine[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | %s               |\n", offset += 2, offset, machine[0]&255, machine[1]&255, machine);

    char number_of_sections[2+1] = {0}; // Количество секций
    number_of_sections[0] = getc(file);
    number_of_sections[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | %s                |\n", offset += 2, offset,
     number_of_sections[0]&255,
     number_of_sections[1]&255,
     number_of_sections
    );
    char time_date_stamp[4+1] = {0}; // Дата и время создания файла
    time_date_stamp[0] = getc(file);
    time_date_stamp[1] = getc(file);
    time_date_stamp[2] = getc(file);
    time_date_stamp[3] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s             |\n", offset += 2, offset,
     time_date_stamp[0]&255,
     time_date_stamp[1]&255,
     time_date_stamp[2]&255,
     time_date_stamp[3]&255,
     time_date_stamp
    );
    char pointer_to_symbol_table[4+1] = {0}; // Таблица символов
    pointer_to_symbol_table[0] = getc(file);
    pointer_to_symbol_table[1] = getc(file);
    pointer_to_symbol_table[2] = getc(file);
    pointer_to_symbol_table[3] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s                 |\n", offset += 4, offset,
     pointer_to_symbol_table[0]&255,
     pointer_to_symbol_table[1]&255,
     pointer_to_symbol_table[2]&255,
     pointer_to_symbol_table[3]&255,
     pointer_to_symbol_table
    );
    char number_of_symbols[4+1] = {0}; // Количество символов
    number_of_symbols[0] = getc(file);
    number_of_symbols[1] = getc(file);
    number_of_symbols[2] = getc(file);
    number_of_symbols[3] = getc(file);
    fprint("| %03d=%02X | %02X %02X %02X %02X                                     | %s               |\n", offset += 4, offset,
     number_of_symbols[0]&255,
     number_of_symbols[1]&255,
     number_of_symbols[2]&255,
     number_of_symbols[3]&255,
     number_of_symbols
    );
    char size_of_optional_header[2+1] = {0}; // Размер следующего подзаголовка
    size_of_optional_header[0] = getc(file);
    size_of_optional_header[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | %s                |\n", offset += 4, offset,
     size_of_optional_header[0]&255,
     size_of_optional_header[1]&255,
     size_of_optional_header
    );
    char characteristics[2+1] = {0}; // Характеристики файла
    characteristics[0] = getc(file);
    characteristics[1] = getc(file);
    fprint("| %03d=%02X | %02X %02X                                           | %s                |\n", offset += 2, offset,
     characteristics[0]&255,
     characteristics[1]&255,
     characteristics
    );
    print("|-----------------------------------------------------------------------------|");
    print("|                              _________________           |                  |");
    print("|-----------------------------/ OPTIONAL HEADER \\-----------------------------|");
    print("|-----------------------------------------------------------------------------|");
    return 0;
}