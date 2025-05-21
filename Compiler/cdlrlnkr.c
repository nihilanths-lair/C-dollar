/*/
 *    Компиляция: gcc -o cdlrlnkr.exe cdlrlnkr.c
 *    Использование: cdlrlnkr.exe main.bin main.exe
/*/

#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)

typedef struct {
    IMAGE_DOS_HEADER dos;
    BYTE stub[64]; // Minimal DOS stub
    IMAGE_NT_HEADERS32 nt;
    IMAGE_SECTION_HEADER text;
} MinimalPEHeader;

#pragma pack(pop)

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s input.bin output.exe\n", argv[0]);
        return 1;
    }

    FILE* in = fopen(argv[1], "rb");
    if (!in) {
        perror("Input");
        return 1;
    }

    FILE* out = fopen(argv[2], "wb");
    if (!out) {
        perror("Output");
        return 1;
    }

    fseek(in, 0, SEEK_END);
    DWORD size = ftell(in);
    fseek(in, 0, SEEK_SET);

    BYTE* code = malloc(size);
    fread(code, 1, size, in);
    fclose(in);

    MinimalPEHeader pe = {0};
    pe.dos.e_magic = IMAGE_DOS_SIGNATURE; // MZ
    pe.dos.e_lfanew = sizeof (IMAGE_DOS_HEADER) + sizeof (pe.stub);

    pe.nt.Signature = IMAGE_NT_SIGNATURE; // PE\0\0
    pe.nt.FileHeader.Machine = IMAGE_FILE_MACHINE_I386;
    pe.nt.FileHeader.NumberOfSections = 1;
    pe.nt.FileHeader.SizeOfOptionalHeader = sizeof (IMAGE_OPTIONAL_HEADER32);
    pe.nt.FileHeader.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_32BIT_MACHINE;

    IMAGE_OPTIONAL_HEADER32* opt = &pe.nt.OptionalHeader;
    opt->Magic = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
    opt->AddressOfEntryPoint = 0x1000; // RVA
    opt->ImageBase = 0x400000;
    opt->SectionAlignment = 0x1000;
    opt->FileAlignment = 0x200;
    opt->SizeOfImage = 0x2000;
    opt->SizeOfHeaders = 0x200;
    opt->Subsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
    opt->NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
    opt->SizeOfCode = 0x1000;
    opt->BaseOfCode = 0x1000;

    IMAGE_SECTION_HEADER* sec = &pe.text;
    memcpy(sec->Name, ".text", 5);
    sec->VirtualAddress = 0x1000;
    sec->Misc.VirtualSize = size;
    sec->SizeOfRawData = 0x1000;
    sec->PointerToRawData = 0x200;
    sec->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

    fwrite(&pe, 1, sizeof (pe), out);

    BYTE pad[512] = {0}; // для выравнивания
    fwrite(pad, 1, 0x200 - sizeof (pe), out);

    fwrite(code, 1, size, out);
    if (size < 0x1000) fwrite(pad, 1, 0x1000 - size, out);

    fclose(out);
    free(code);

    printf("Готово! %s создан.\n", argv[2]);
    return 0;
}