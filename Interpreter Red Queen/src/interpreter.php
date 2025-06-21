<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
function rq_run_bf(string $code): array {
    $memory = array_fill(0, 256, 0);
    $ptr = 0;
    $code = preg_replace('/\s+/', '', $code);
    preg_match_all('/(\[\d*\])\s*([+\-*\/%=]=?|=)\s*(\[\d*\]|\d+)/', $code, $matches, PREG_SET_ORDER);
    foreach ($matches as $match) {
        [$full, $lhs, $op, $rhs] = $match;
        $lhs_index = strlen($lhs) === 2 ? $ptr : (int) trim($lhs, '[]');
        $lhs_value = &$memory[$lhs_index];
        if (str_starts_with($rhs, '[')) {
            $rhs_index = strlen($rhs) === 2 ? $ptr : (int) trim($rhs, '[]');
            $rhs_value = $memory[$rhs_index];
        } else {
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

function rq_run_asm(string $code): array {
    $memory = array_fill(0, 256, 0);
    $ptr = 0;
    $lines = preg_split('/[\r\n;]+/', $code);
    foreach ($lines as $line) {
        $line = trim($line);
        if ($line === '') continue;
        if (!preg_match('/^(MOV|ADD|SUB|MUL|DIV|MOD)\s+(\[\d*\]|\[\])\s*,\s*(\[\d*\]|\[\]|\d+)$/i', $line, $m)) continue;
        $cmd = strtoupper($m[1]);
        $lhs_raw = $m[2];
        $rhs_raw = $m[3];
        $lhs_index = $lhs_raw === '[]' ? $ptr : (int) trim($lhs_raw, '[]');
        $lhs_value = &$memory[$lhs_index];
        if (preg_match('/^\[\d*\]|\[\]$/', $rhs_raw)) {
            $rhs_index = $rhs_raw === '[]' ? $ptr : (int) trim($rhs_raw, '[]');
            $rhs_value = $memory[$rhs_index];
        } else {
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

function format_memory_dump(array $mem): string
{
    $lines = [];
    $lines[] = "Dec | 000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 || Hex | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F || ASCII";
    $lines[] = "--- | --------------------------------------------------------------- || --- | ----------------------------------------------- || -----";

    for ($row = 0; $row < 256; $row += 16)
    {
        $dec_values = [];
        $hex_values = [];
        $ascii_chars = [];

        for ($i = 0; $i < 16; $i++)
        {
            $index = $row + $i;
            $val = $mem[$index] ?? 0;

            $dec_values[] = str_pad((string)$val, 3, '0', STR_PAD_LEFT);
            $hex_values[] = strtoupper(str_pad(dechex($val), 2, '0', STR_PAD_LEFT));
            $ascii_chars[] = ($val >= 32 && $val <= 126) ? chr($val) : '.';
        }

        $dec_offset = str_pad((string)$row, 3, '0', STR_PAD_LEFT);

        // реальный hex offset каждого байта (00 01 02 ...)
        $hex_offset_values = [];
        for ($i = 0; $i < 16; $i++)
        {
            $hex_offset_values[] = strtoupper(str_pad(dechex($row + $i), 2, '0', STR_PAD_LEFT));
        }

        $lines[] =
            "$dec_offset | " . implode(' ', $dec_values) .
            " || " . implode(' ', $hex_offset_values) . " | " . implode(' ', $hex_values) .
            " || " . implode('', $ascii_chars);
    }
    return "<pre>" . implode("\n", $lines) . "</pre>";
}

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
    <title>RQ Интерпретатор</title>
    <style>
        body {
            font-family: monospace;
            background: #111;
            color: #eee;
            padding: 2em;
            display: flex;
            gap: 2em;
        }
        form {
            flex: 1;
        }
        .memory {
            flex: 1;
            max-width: 700px;
        }
        textarea, select, input[type=submit] {
            font-family: monospace;
            background: #222;
            color: #fff;
            border: none;
            padding: 0.5em;
        }
        textarea {
            width: 100%;
            height: 300px;
        }
        pre {
            background: #222;
            padding: 1em;
            margin-top: 1em;
            line-height: 1.3em;
        }
        .controls {
            margin-top: 1em;
            display: flex;
            gap: 1em;
            align-items: center;
        }
    </style>
</head>
<body>
    <form method="post">
        <h2>RQ Интерпретатор</h2>
        <div class="controls">
            <label>Синтаксис:
                <select name="mode">
                    <option value="bf" <?= $mode === 'bf' ? 'selected' : '' ?>>Brainfuck++</option>
                    <option value="asm" <?= $mode === 'asm' ? 'selected' : '' ?>>Assembly</option>
                </select>
            </label>
            <input type="submit" value="Выполнить">
        </div>
        <textarea name="code" placeholder="Введите код..."><?= htmlspecialchars($inputCode) ?></textarea>
    </form>

    <div class="memory">
        <h2>Память</h2>
        <?= $memoryDump ?>
    </div>
</body>
</html>