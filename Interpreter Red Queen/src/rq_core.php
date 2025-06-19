function assemble_rq(string $source): string
{
    $lines = preg_split('/\R/', $source);
    $bytes = [];

    foreach ($lines as $line)
    {
        $line = trim($line);
        if ($line === '' || str_starts_with($line, ';')) continue;

        // --- [n] := значение ---
        if (preg_match('/^\[(\d{1,5})\]\s*:=\s*(.+)$/u', $line, $m))
        {
            $target = intval($m[1]);
            $value_raw = trim($m[2]);

            // Символ в апострофах
            if (preg_match("/^'(.{1})'$/us", $value_raw, $sm)) {
                $value = mb_convert_encoding($sm[1], 'CP1251', 'UTF-8'); // Преобразуем в байт CP1251
                $value = ord($value);
            }
            // Шестнадцатеричное значение 0xFF
            else if (preg_match('/^0x([0-9a-fA-F]{1,2})$/', $value_raw, $hm)) {
                $value = hexdec($hm[1]);
            }
            // Десятичное значение
            else if (ctype_digit($value_raw)) {
                $value = intval($value_raw);
            }
            else {
                continue; // Невалидный формат — пропускаем
            }
            // Кодируем инструкцию
            $bytes[] = 0x40;     // переместить указатель
            $bytes[] = $target;
            $bytes[] = 0x77;     // :=
            $bytes[] = $value;
            continue;
        }
        // @n — прямое позиционирование
        else if (preg_match('/^@\s*(\d{1,5})$/', $line, $m))
        {
            $bytes[] = 0x40;
            $bytes[] = intval($m[1]);
        }
        // Базовые однобайтовые команды
        if ($line === '+') $bytes[] = 0x2B;
        else if ($line === '-') $bytes[] = 0x2D;
        else if ($line === '>') $bytes[] = 0x3E;
        else if ($line === '<') $bytes[] = 0x3C;
        else if ($line === '.') $bytes[] = 0x2E;

        // ~! переход
        else if (preg_match('/^~!\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x9F;
            $bytes[] = intval($m[1]);
        }
        // +=n
        else if (preg_match('/^\+=\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x68;
            $bytes[] = intval($m[1]);
        }
        // -=n
        else if (preg_match('/^-\=\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x6A;
            $bytes[] = intval($m[1]);
        }
        // :=n
        else if (preg_match('/^:=\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x77;
            $bytes[] = intval($m[1]);
        }
        // Можно добавить else для логирования некорректной строки
    }
    return implode(array_map('chr', $bytes));
}
function interpret_rq_binary(string $code): string
{
    $tape = array_fill(0, 30000, 0);
    $ptr = 0;
    $output = '';
    $i = 0;
    $len = strlen($code);

    $jmp_count = 0; // виртуальный регистр подсчёта переходов

    while ($i < $len)
    {
        if ($jmp_count >= 255)
        {
            $output .= "\n\n[!] Превышено максимальное число переходов (255). Выполнение остановлено.";
            break;
        }
        $op = ord($code[$i]);

        switch ($op){
        case 0x2B: // +
        {
            $tape[$ptr] = ($tape[$ptr] + 1) % 256;
            $i++;
            break;
        }
        case 0x2D: // -
        {
            $tape[$ptr] = ($tape[$ptr] - 1 + 256) % 256;
            $i++;
            break;
        }
        case 0x3E: // >
        {
            $ptr++;
            $i++;
            break;
        }
        case 0x3C: // <
        {
            $ptr--;
            $i++;
            break;
        }
        case 0x2E: // .
        {
            //$char = chr($tape[$ptr]);
            //$output .= mb_convert_encoding($char, 'UTF-8', 'Windows-1251');
            $output .= iconv('CP1251', 'UTF-8', chr($tape[$ptr]));
            $i++;
            break;
        }
        case 0x68: // += byte
        {
            if ($i+1 < $len)
            {
                $val = ord($code[$i+1]);
                $tape[$ptr] = ($tape[$ptr] + $val) % 256;
                $i += 2;
            }
            else $i++; // пропуск некорректного
            break;
        }
        case 0x6A: // -= byte
        {
            if ($i+1 < $len)
            {
                $val = ord($code[$i+1]);
                $tape[$ptr] = ($tape[$ptr] - $val + 256) % 256;
                $i += 2;
            }
            else $i++; // пропуск
            break;
        }
        case 0x9F: // ~! безусловный переход
        {
            if ($i+1 < $len)
            {
                $jmp_count++;
                $target = ord($code[$i+1]);
                $i = $target; // прыжок на указанный адрес в коде
            }
            else $i++; // если нет второго байта — пропустить
            break;
        }
        case 0x40: // @<n> — прямое позиционирование указателя
        {
            // [addr] := value — атомарная форма
            if ($i + 3 < $len && ord($code[$i+2]) === 0x77)
            {
                $target = ord($code[$i+1]);
                $value = ord($code[$i+3]);
                $tape[$target] = $value;
                $i += 4; // съедаем всю последовательность
            }
            else if ($i + 1 < $len)
            {
                // просто @<n>
                $ptr = ord($code[$i + 1]);
                $i += 2;
            }
            else $i++;
            break;
        }
        case 0x41: // [addr] += or -=
        {
            if ($i + 3 < $len)
            {
                $target = ord($code[$i + 1]);
                $op = ord($code[$i + 2]);
                $val = ord($code[$i + 3]);

                if ($op === 0x68) $tape[$target] = ($tape[$target] + $val) % 256; // +=
                else if ($op === 0x6A) $tape[$target] = ($tape[$target] - $val + 256) % 256; // -=
                $i += 4;
            }
            else $i++; // неполная команда — пропуск
            break;
        }
        case 0x77: // := byte (прямое присваивание)
        {
            if ($i + 1 < $len)
            {
                $val = ord($code[$i+1]);
                $tape[$ptr] = $val;
                $i += 2;
            }
            else $i++; // защита от ошибки
            break;
        }
        default: $i++; // неизвестный байт — пропустить
        }
    }
    return $output;
}
