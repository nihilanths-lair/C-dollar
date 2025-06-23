<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

session_start();

// Post → Redirect → Get
if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
    $_SESSION['last_input'] = $_POST;
    header('Location: ' . $_SERVER['PHP_SELF']);
    exit;
}

// Восстановление данных после редиректа
$inputCode = '';
$mode = 'bf';

if (isset($_SESSION['last_input']))
{
    $inputCode = $_SESSION['last_input']['code'] ?? '';
    $mode = $_SESSION['last_input']['mode'] ?? 'bf';
    unset($_SESSION['last_input']);
}
function rq_run_bf(string $code): array
{
    $memory = array_fill(0, 256, 0);
    $ptr = 0;
    $code = preg_replace('/\s+/', '', $code);
    preg_match_all('/(\[\d*\])\s*([+\-*\/%=]=?|=)\s*(\[\d*\]|\d+)/', $code, $matches, PREG_SET_ORDER);
    foreach ($matches as $match)
    {
        [$full, $lhs, $op, $rhs] = $match;
        $lhs_index = strlen($lhs) === 2 ? $ptr : (int) trim($lhs, '[]');
        $lhs_value = &$memory[$lhs_index];

        if (str_starts_with($rhs, '['))
        {
            $rhs_index = strlen($rhs) === 2 ? $ptr : (int) trim($rhs, '[]');
            $rhs_value = $memory[$rhs_index];
        }
        else {
            $rhs_value = (int)$rhs;
        }
        switch ($op) {
        case '=':  $lhs_value = $rhs_value; break;
        case '+=': $lhs_value = ($lhs_value + $rhs_value) & 0xFF; break;
        case '-=': $lhs_value = ($lhs_value - $rhs_value) & 0xFF; break;
        case '*=': $lhs_value = ($lhs_value * $rhs_value) & 0xFF; break;
        case '/=': $lhs_value = $rhs_value !== 0 ? intdiv($lhs_value, $rhs_value) : 0; break;
        case '%=': $lhs_value = $rhs_value !== 0 ? ($lhs_value % $rhs_value) : 0; break;
        }
    }
    return $memory;
}
function rq_run_asm(string $code): array
{
    $memory = array_fill(0, 256, 0);
    $ptr = 0;
    $lines = preg_split('/[\r\n;]+/', $code);
    foreach ($lines as $line)
    {
        $line = trim($line);
        if ($line === '') continue;
        if (!preg_match('/^(MOV|ADD|SUB|MUL|DIV|MOD)\s+(\[\d*\]|\[\])\s*,\s*(\[\d*\]|\[\]|\d+)$/i', $line, $m)) continue;

        $cmd = strtoupper($m[1]);
        $lhs_raw = $m[2];
        $rhs_raw = $m[3];
        $lhs_index = $lhs_raw === '[]' ? $ptr : (int) trim($lhs_raw, '[]');
        $lhs_value = &$memory[$lhs_index];

        if (preg_match('/^\[\d*\]|\[\]$/', $rhs_raw))
        {
            $rhs_index = $rhs_raw === '[]' ? $ptr : (int) trim($rhs_raw, '[]');
            $rhs_value = $memory[$rhs_index];
        }
        else {
            $rhs_value = (int)$rhs_raw;
        }
        switch ($cmd) {
        case 'MOV': $lhs_value = $rhs_value; break;
        case 'ADD': $lhs_value = ($lhs_value + $rhs_value) & 0xFF; break;
        case 'SUB': $lhs_value = ($lhs_value - $rhs_value) & 0xFF; break;
        case 'MUL': $lhs_value = ($lhs_value * $rhs_value) & 0xFF; break;
        case 'DIV': $lhs_value = $rhs_value !== 0 ? intdiv($lhs_value, $rhs_value) : 0; break;
        case 'MOD': $lhs_value = $rhs_value !== 0 ? ($lhs_value % $rhs_value) : 0; break;
        }
    }
    return $memory;
}
function format_memory_dump_bin(array $mem): string
{
    $lines = [];

    // Заголовок (offset + 16 колонок)
    $header = '    BIN   |';
    for ($col = 0; $col < 16; $col++)
    {
        $label = '0000:' . str_pad(decbin($col), 4, '0', STR_PAD_LEFT);
        $header .= ' ' . str_pad($label, 10, ' ', STR_PAD_RIGHT); // 10 символов на каждую колонку (включая пробел)
    }
    $lines[] = $header;

    // Разделитель строк (по ширине заголовка)
    $lines[] = '----------|' . str_repeat('-', strlen($header) - strlen('-----------|'));

    // Содержимое памяти
    for ($row = 0; $row < 256; $row += 16)
    {
        $chunk = array_slice($mem, $row, 16);
        //$offset = strtoupper(str_pad(dechex($row), 4, '0', STR_PAD_LEFT));
        //$line = strtoupper(str_pad(decbin($row), 4, '0', STR_PAD_LEFT)) . ' |';
        //$line = ' 0000:' . str_pad(decbin($row), 4, '0', STR_PAD_LEFT) . ' |';
        $offset = str_pad(decbin($row), 8, '0', STR_PAD_LEFT);
        $line = '';
        $line .= '' . str_pad(substr($offset, 0, 4) . ':' . substr($offset, 4, 4), 10, ' ', STR_PAD_RIGHT) . '|';
        foreach ($chunk as $val)
        {
            $bin = str_pad(decbin($val), 8, '0', STR_PAD_LEFT);
            $line .= ' ' . str_pad(substr($bin, 0, 4) . ':' . substr($bin, 4, 4), 10, ' ', STR_PAD_RIGHT);
        }
        $lines[] = $line;
    }
    return '<pre>' . implode("\n", $lines) . '</pre>';
}
function format_memory_dump(array $mem): string
{
    $dec_lines = [];
    $hex_lines = [];
    //$bin_lines = [];
    $ascii_lines = [];

    // Заголовки
    $dec_lines[]   = 'DEC | 000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015';
    $hex_lines[]   = 'HEX | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F';
    //$bin_lines[]   = "BIN | 0000:0000 0000:0001 0000:0010 0000:0011 0000:0100 0000:0101 0000:0110 0000:0111 0000:1000 0000:1001 0000:1010 0000:1011 0000:1100 0000:1101 0000:1110";
    $ascii_lines[] = 'ASCII';

    $dec_lines[]   = '----|----------------------------------------------------------------';
    $hex_lines[]   = '----|------------------------------------------------';
    //$bin_lines[]   = "----|----------------------------------------------------------------------------';
    $ascii_lines[] = '----------------';

    for ($row = 0; $row < 16; $row++)
    {
        $dec_row = [];
        $hex_row = [];
        //$bin_row = [];
        $ascii_row = [];

        for ($i = 0; $i < 16; $i++)
        {
            $index = $row * 16 + $i;
            $val = $mem[$index] ?? 0;

            $dec_row[]  = str_pad((string)$val, 3, '0', STR_PAD_LEFT);
            $hex_row[]  = strtoupper(str_pad(dechex($val), 2, '0', STR_PAD_LEFT));
            //$bin_row[]  = str_pad(decbin($val), 8, '0', STR_PAD_LEFT);
            $ascii_row[] = ($val >= 32 && $val <= 126) ? chr($val) : '.';
        }

        $dec_offset   = str_pad((string)($row * 16), 3, '0', STR_PAD_LEFT);
        $hex_offset   = strtoupper(str_pad(dechex($row * 16), 2, '0', STR_PAD_LEFT));
        //$ascii_prefix = '';

        $dec_lines[]   = "$dec_offset | " . implode(' ', $dec_row);
        $hex_lines[]   = "$hex_offset  | " . implode(' ', $hex_row);
        //$bin_lines[]   = $ascii_prefix . '  ' . implode(' ', $bin_row);
        $ascii_lines[] = /*$ascii_prefix . '  ' . */ implode('', $ascii_row);
    }
    return "<div style=\"display: flex; gap: 30px; font-family: monospace; font-size: 14px;\">
        <pre style=\"overflow-x: hidden;\">" . implode("\n", $dec_lines) . "</pre>
        <pre style=\"overflow-x: hidden;\">" . implode("\n", $hex_lines) . "</pre>
        <pre style=\"overflow-x: hidden;\">" . implode("\n", $ascii_lines) . "</pre>
    </div>";
    /*
    return "<div style=\"display: flex; gap: 30px; font-family: monospace; font-size: 14px;\">
        <pre>" . implode("\n", $dec_lines) . "</pre>
        <pre>" . implode("\n", $hex_lines) . "</pre>
        <pre>" . implode("\n", $ascii_lines) . "</pre>
    </div>";
    */
}//<pre>" . implode("\n", $bin_lines) . "</pre>
$mode = $_POST['mode'] ?? 'bf';
$inputCode = $_POST['code'] ?? '';
$mem = array_fill(0, 256, 0);

if ($_SERVER['REQUEST_METHOD'] === 'POST' && $inputCode !== '') {
    $mem = $mode === 'asm' ? rq_run_asm($inputCode) : rq_run_bf($inputCode);
}
$memoryDump = format_memory_dump($mem);
?>
<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="UTF-8">
  <link rel="icon" type="image/x-icon" href="/favicon/logo(16x16).ico">
  <title>RQ Интерпретатор</title>
  <style>
  html, body {
    margin: 0;
    padding: 0;
    height: 100%;
    background: #111;
    color: #eee;
    font-family: monospace;
  }
  body {
    display: flex;
    flex-wrap: wrap;
    gap: 2em;
    padding: 2em;
    justify-content: center; /* горизонтальное центрирование */
    /*align-items: center;   /* вертикальное центрирование */
    /*min-height: 100vh;     /* чтобы align-items сработал по высоте окна */
  }
  .container {
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 2em;
    width: 100%;
    max-width: 1400px;
  }
  form {
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 1em;
  }
  .controls {
    display: flex;
    gap: 1em;
    align-items: center;
  }
  textarea, select, input[type=submit] {
    font-family: monospace;
    background: #222;
    color: #fff;
    border: none;
    padding: 0.5em;
  }
  textarea {
    width: 600px;
    height: 300px;
    resize: none; /* 🔒 запрещаем изменение размера */
  }
  .memory {
    flex: 1 1 45%;
    max-width: 800px;
  }
  pre {
    background: #222;
    padding: 1em;
    margin-top: 1em;
    line-height: 1.3em;
    overflow-x: auto;
  }
  </style>
</head>
<body>
  <div class="container">
  <details style="width: 100%; max-width: 800px; margin-bottom: 2em;">
  <summary style="cursor: pointer; font-weight: bold; font-size: 16px;">📘 Инструкции по использованию</summary>
  <pre style="background:#222; color:#eee; padding:1em; overflow-x:auto; font-family:monospace; font-size:14px;">
🔀 Синтаксис:
- Brainfuck++: символьный упрощённый стиль
- Assembly: текстовый формат команд

🧠 Brainfuck++:
- []     = текущая ячейка
- [n]    = ячейка с индексом n

Допустимые операции:
  [] = 42       ; запись значения
  [5] += 3      ; прибавление
  [10] *= [11]  ; умножение с другой ячейкой

Операторы:
  =   +=   -=   *=   /=   %=

⚙️ Assembly (asm):
Синтаксис: OPCODE [адрес], [значение/адрес]

Доступные команды:
  MOV [0], 42
  ADD [], [1]
  SUB [2], 10
  MUL [1], [3]
  DIV [4], 2
  MOD [5], 7

Пример:
  MOV [0], 10
  MOV [1], 20
  ADD [0], [1]

📦 Память:
- 256 байт: 0–255
- Значения обрезаются до байта (mod 256)
- Отображение: DEC / HEX / ASCII / BIN

💡 Подсказка:
- [ ] — текущая позиция
- [n] — абсолютная позиция
  </pre>
  </details>
  <form method="post">
    <h2>RQ Интерпретатор v.0.1 (доделан не до конца, продолжаются работы)</h2>
    <div class="controls">
      <label>Синтаксис:
        <select name="mode">
        <option value="bf" <?=$mode === 'bf' ? 'selected' : ''?>>Brainfuck++</option>
        <option value="asm" <?=$mode === 'asm' ? 'selected' : ''?>>Assembly</option>
        </select>
      </label>
      <input type="submit" value="Выполнить">
    </div>
    <textarea name="code" rows="10" cols="40" placeholder="Введите код..."><?=htmlspecialchars($inputCode)?></textarea>
  </form>
  <div class="memory">
    <h2>Память (DEC/HEX/ASCII)</h2>
    <?=$memoryDump?>
  </div>
  <div class="memory">
    <h2>Память (BIN)</h2>
    <?=format_memory_dump_bin($mem)?>
  </div>
</div>
</body>
</html>
<!-- kramer -->