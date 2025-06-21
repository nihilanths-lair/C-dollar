<?php
function rq_run_bf(string $code): array
{
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
function rq_run_asm(string $code): array
{
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
function format_memory_dump(array $mem, string $format): string {
    $lines = [];
    for ($row = 0; $row < 256; $row += 16) {
        $cells = array_slice($mem, $row, 16);
        $nums = array_map(function($v) use ($format) {
            return $format === 'hex' ? strtoupper(str_pad(dechex($v), 2, '0', STR_PAD_LEFT)) : str_pad((string)$v, 3, ' ', STR_PAD_LEFT);
        }, $cells);
        $ascii = array_map(function($v) {
            return ($v >= 32 && $v <= 126) ? chr($v) : '.';
        }, $cells);
        $offset = str_pad((string)$row, 3, '0', STR_PAD_LEFT);
        $lines[] = "$offset  " . implode(' ', $nums) . "   | " . implode('', $ascii);
    }
    return "<pre>" . implode("\n", $lines) . "</pre>";
}
$mode = $_POST['mode'] ?? 'bf';
$base = $_POST['base'] ?? 'dec';
$inputCode = $_POST['code'] ?? '';
$result = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
    $mem = $mode === 'asm' ? rq_run_asm($inputCode) : rq_run_bf($inputCode);
    $result = format_memory_dump($mem, $base);
}
?>
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>RQ Интерпретатор</title>
    <style>
        body { font-family: monospace; background: #111; color: #eee; padding: 2em; }
        textarea, select, input[type=submit] {
            font-family: monospace;
            background: #222;
            color: #fff;
            border: none;
            padding: 0.5em;
        }
        textarea { width: 100%; height: 200px; }
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
    <h1>RQ Интерпретатор</h1>
    <form method="post">
        <div class="controls">
            <label>Синтаксис:
                <select name="mode">
                    <option value="bf" <?= $mode === 'bf' ? 'selected' : '' ?>>Brainfuck++</option>
                    <option value="asm" <?= $mode === 'asm' ? 'selected' : '' ?>>Assembly</option>
                </select>
            </label>
            <label>Вывод:
                <select name="base">
                    <option value="dec" <?= $base === 'dec' ? 'selected' : '' ?>>Decimal</option>
                    <option value="hex" <?= $base === 'hex' ? 'selected' : '' ?>>Hex</option>
                </select>
            </label>
            <input type="submit" value="Выполнить">
        </div>
        <textarea name="code" placeholder="Введите код..."><?= htmlspecialchars($inputCode) ?></textarea>
    </form>
    <?= $result ?>
</body>
</html>