<?php
declare(strict_types=1);

$access_token = 'NaN';
$peer_id = 2000000005;
$vk_callback = json_decode(file_get_contents('php://input'));
file_put_contents('input.log', file_get_contents('php://input'));

$type = $vk_callback->type ?? null;
if ($type === 'message_new')
{
    $text = $vk_callback->object->message->text ?? '';
    $peer_id = $vk_callback->object->message->peer_id ?? 0;

    $prefix = '[club226434493|@viki_rq] ';
    if (strpos($text, $prefix) === 0)
    {
        $source_code = substr($text, strlen($prefix)); // Сам текст без префикса
        //$rq_code = substr($text, strlen($prefix));
        $source_code = trim($source_code); // убрать лишние пробелы и переносы

        if ($source_code === '') SendMessage("❗️Вы не указали код для выполнения на языке RQ.", $peer_id, $access_token);
        else
        {
            $bytecode = assemble_rq($source_code);    // Преобразуем в байт-код
            $result = interpret_rq_binary($bytecode); // Исполняем
            //$result = interpret_rq($source_code);
            SendMessage("Код на RQ:\n$source_code\n\nРезультат выполнения программы:\n$result", $peer_id, $access_token);
        }
    }
    echo 'ok';
}
else if ($type === 'confirmation') echo '-';
///
function SendMessage(string $message, int $peer_id, string $access_token) : void
{
    $request_params = [
     'message' => $message,
     'peer_id' => $peer_id,
     'access_token' => $access_token,
     'v' => '5.199',
     'random_id' => rand(1, 1000000)
    ];

    $url = 'https://api.vk.com/method/messages.send?' . http_build_query($request_params);

    file_put_contents('url.log', $url);

    $result = file_get_contents($url);
    file_put_contents('vk_response.log', $result);
}
///
function assemble_rq(string $source): string
{
    $lines = preg_split('/\R/', $source); // делим по строкам
    $bytes = [];

    foreach ($lines as $line)
    {
        //$line = preg_replace('/\s*(#|\/\/).*$/', '', $line); // Удаляем комментарии
        $line = trim($line);
        if ($line === '' || str_starts_with($line, ';')) continue; // была #, сделал ;

        if (preg_match('/^\[(\d{1,5})\]\s*:=\s*(\d{1,3})$/', $line, $m))
        {
            $target = intval($m[1]);
            $value = intval($m[2]);
            
            // генерируем последовательность перемещения
            $bytes[] = 0x40; // специальный код перехода по адресу (расширим — см. ниже)
            $bytes[] = $target; // ячейка
            $bytes[] = 0x77; // :=
            $bytes[] = $value;
            continue;
        }
        else if (preg_match('/^@\s*(\d{1,5})$/', $line, $m))
        {
            $bytes[] = 0x40;
            $bytes[] = intval($m[1]);
        }

        if ($line === '+') $bytes[] = 0x2B;
        else if ($line === '-') $bytes[] = 0x2D;
        else if ($line === '>') $bytes[] = 0x3E;
        else if ($line === '<') $bytes[] = 0x3C;
        else if ($line === '.') $bytes[] = 0x2E;

        else if (preg_match('/^~!\s*(\d{1,3})$/', $line, $m)) // ~!
        {
            $bytes[] = 0x9F;           // байт-код безусловного перехода
            $bytes[] = intval($m[1]);  // адрес ячейки памяти, куда необходимо осуществить переход (однобайтовый)
        }
        else if (preg_match('/^\[(\d{1,5})\]\s*\+=\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x41;           // код модификации ячейки
            $bytes[] = intval($m[1]);  // адрес
            $bytes[] = 0x68;           // +=
            $bytes[] = intval($m[2]);  // значение
        }
        else if (preg_match('/^\[(\d{1,5})\]\s*\-=\s*(\d{1,3})$/', $line, $m))
        {
            $bytes[] = 0x41;           // код модификации ячейки
            $bytes[] = intval($m[1]);  // адрес
            $bytes[] = 0x6A;           // -=
            $bytes[] = intval($m[2]);  // значение
        }
        else if (preg_match('/^:=\s*(\d{1,3})$/', $line, $m)) // :=
        {
            $bytes[] = 0x77;
            $bytes[] = intval($m[1]);
        }
        else {
            // можно добавить ошибку или лог
        }
    }
    return implode(array_map('chr', $bytes)); // бинарная строка
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