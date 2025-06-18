function interpret_rq(string $code): string
{
    $tape = array_fill(0, 30000, 0);
    $ptr = 0;
    $hoarder = '';

    // Фильтруем только допустимые команды
    $code = preg_replace('/[^+\-<>.]/', '', $code);
    $len = mb_strlen($code);

    for ($i = 0; $i < $len; $i++)
    {
        switch ($code[$i]){
        case '+':
            $tape[$ptr] = ($tape[$ptr]+1) % 256;
            break;
        case '-':
            $tape[$ptr] = ($tape[$ptr]-1 + 256) % 256;
            break;
        case '>':
            $ptr++;
            break;
        case '<':
            $ptr--;
            break;
        case '.': // 046 2E - выводит символ с текущей ячейки памяти
        {
            // Преобразуем в UTF-8, если символ в расширенном диапазоне
            $char = chr($tape[$ptr]);
            $hoarder .= mb_convert_encoding($char, 'UTF-8', 'Windows-1251');
            break;
        }}
    }
    return $hoarder;
}
