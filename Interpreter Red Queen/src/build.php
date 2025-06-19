<?php
declare(strict_types=1);
function preprocess_file(string $filename): string
{
    $content = file_get_contents($filename);
    $lines = explode("\n", $content);
    $output = '';

    foreach ($lines as $line)
    {
        $line = trim($line);
        if ($line === '' || str_starts_with($line, ';') || str_starts_with($line, '//')) continue;

        // Обработка import
        if (preg_match('/^import\s+\"(.+)\"$/', $line, $m))
        {
            $output .= preprocess_file($m[1]) . "\n";
            continue;
        }
        // Макросы и директивы позже
        $output .= $line . "\n";
    }
    return $output;
}
$code = preprocess_file('main.rq');
file_put_contents('compiled.rq', $code);
echo "✓ Сборка завершена: compiled.rq\n";
