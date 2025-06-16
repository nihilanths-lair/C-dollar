#include <stdio.h>
#include <locale.h>
#include <string.h>

int findch(const char *str, int start_pos, const char ch)
{
    while (str[start_pos] != '\0')
    {
        if (str[start_pos] == ch) return start_pos; // найден, вернём позицию символа
        start_pos++;
    }
    return -1; // не найден символ, вернём код ошибки
}

int findstr(const char *str, int start_pos, const char *substr)
{
    int count = strlen(substr);
    int i = 0;
    while (i < count)
    {
        if ((start_pos = findch(str, start_pos, substr[i])) == -1)
        {
            return -1; // не найдена строка, вернём код ошибки
        }
        i++;
    }
    return start_pos;
}

int main()
{
    setlocale(0, "");
    int code;
    /*
    code = findch("int main(){//код}", 0, '/');
    printf("# code = %d\n", code);
    code = findch("int main(){//код}", code+1, '/');
    printf("# code = %d\n", code);
    */
    code = findstr("int main(){//код}", 0, "//");
    printf("# code = %d", code);
    return 0;
}