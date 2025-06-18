<?php
function interpret_rq(string $code) : string
{
    $tape = array_fill(0, 30000, 0);
    $ptr = 0;
    $output = '';

    file_put_contents('no_urlencode.log', $code);
    file_put_contents('urlencode.log', urlencode($code));

    // Фильтруем только допустимые команды
    $code = preg_replace('/[^+\-<>.]/', '', $code);
    $len = mb_strlen($code);

    for ($i = 0; $i < $len; $i++)
    {
        switch ($code[$i]){
        case '+': $tape[$ptr] = ($tape[$ptr] + 1) % 256; break;
        case '-': $tape[$ptr] = ($tape[$ptr] - 1 + 256) % 256; break;
        case '>': $ptr++; break;
        case '<': $ptr--; break;
        case '.': // 046 2E - выводит символ с текущей ячейки памяти (поместим в сборник/накопитель)
        {
            //$output .= mb_chr($tape[$ptr], 'UTF-8');
            file_put_contents('debugss.txt', "Отладочное значение \$tape[\$ptr]: " . $tape[$ptr] . "\n"); // <--- ЛОГИРУЕМ ЗДЕСЬ!
            $output .= mb_chr($tape[$ptr], 'UTF-8');

            //$dec[] = sprintf("%03d", $tape[$ptr]);
            //$hex[] = sprintf("%02X", $tape[$ptr]);
            break;
        }
        }
    }
    // Добавим вывод байтов в конце (в виде отладочной информации)
    //$output .= "\n\nОтладочная информация:\nDEC(10) | HEX(16)\n" . implode(', ', $dec) . ' | ' . implode(', ', $hex);
    return 'Ваш текст: ' . $output . '.';
}
//return iconv('UTF-8', 'UTF-8//IGNORE', $output);
/*
%D0
%9F
%D1
%80
%D0
%BE
%D0
%B2
%D0
%B5
%D1
%80
%D0
%BA
%D0
%B0
*/