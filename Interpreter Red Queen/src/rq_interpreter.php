<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

//require_once 'rq_run_simple.php';
require_once 'rq_c_interpreter_v3.php';

$output = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
    $rq_code = $_POST['rq_code'] ?? '';
    $strict = isset($_POST['strict']);
    $output = exec_block($lines, $vars, $funcs, $output);
}
?>
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>RQ Vim Runner</title>
    <style>
        body {
            background-color: #1e1e1e;
            color: #d4d4d4;
            font-family: monospace;
            margin: 0;
            padding: 20px;
        }
        .container {
            max-width: 900px;
            margin: auto;
            background-color: #2e2e2e;
            border: 1px solid #444;
            border-radius: 6px;
            padding: 20px;
            box-shadow: 0 0 10px #000;
        }
        h1 {
            color: #98c379;
            font-weight: normal;
        }
        textarea {
            width: 100%;
            height: 250px;
            background-color: #1e1e1e;
            color: #dcdcdc;
            border: 1px solid #555;
            padding: 10px;
            font-family: monospace;
            font-size: 14px;
            resize: vertical;
        }
        pre {
            background-color: #1e1e1e;
            border: 1px solid #555;
            padding: 10px;
            white-space: pre-wrap;
            color: #dcdcdc;
            margin-top: 10px;
        }
        button {
            background-color: #5f5fff;
            color: white;
            border: none;
            padding: 10px 18px;
            font-size: 16px;
            cursor: pointer;
            margin-top: 10px;
        }
        button:hover {
            background-color: #7878ff;
        }
        label {
            display: block;
            margin-top: 10px;
        }
        input[type=checkbox] {
            transform: scale(1.2);
            margin-right: 8px;
        }
    </style>
</head>
<body>
<div class="container">
    <h1>RQ Vim Runner</h1>
    <form method="post">
        <label>
            <textarea name="rq_code" required><?= htmlspecialchars($_POST['rq_code'] ?? '') ?></textarea>
        </label>
        <label>
            <input type="checkbox" name="strict" <?= isset($_POST['strict']) ? 'checked' : '' ?>>
            strict mode
        </label>
        <button type="submit">:wq & Run</button>
    </form>

    <?php if ($output !== ''): ?>
        <h2 style="color: #61afef;">Output:</h2>
        <pre><?= htmlspecialchars($output) ?></pre>
    <?php endif; ?>
</div>
</body>
</html>