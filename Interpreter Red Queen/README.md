# 🧠 Red Queen DSL — Hello World

Пример программы на языке **Red Queen DSL** (Inspired by Brainfuck), которая выводит `Hello World!`.

---

## 📜 Исходный код с поддержкой комментариев

```brainfuck
++++++++++           ; [0] = 10 — подготовим счётчик для цикла
[
  >                  ; перешли к [1] += 7  → после цикла: 7 * 10 = 70
  >++++++++++        ; [2] += 10 → 10 * 10 = 100
  >+++               ; [3] += 3  → 3 * 10 = 30
  >+                 ; [4] += 1  → 1 * 10 = 10
  <<<<               ; назад к [0]
  -                  ; [0]-- → уменьшаем счётчик
]                    ; конец цикла (повторяется 10 раз)
; Вывод строки посимвольно
>++~                 ; [1] = 070 + 2 =  72 → 'H'
>+~                  ; [2] = 100 + 1 = 101 → 'e'
+++++++~             ; [2] = 108           → 'l'
~                    ; [2] = 108           → 'l'
+++~                 ; [2] = 111           → 'o'
>++~                 ; [3] = 030 + 2 = 32  → ' '
<<+++++++++++++++~   ; [1] = 072 + 15 = 87 → 'W'
>~                   ; [2] = 111           → 'o'
+++~                 ; [2] = 114           → 'r'
------~              ; [2] = 108           → 'l'
--------~            ; [2] = 100           → 'd'
>+~                  ; [3] = 032 + 1 = 33  → '!'
```

---

## 🧾 Результат выполнения:

```
Hello World!
```

---

## 🚀 Запуск

```bash
irq hello_world.rq
```

*(где `irq` — это интерпретатор, а `hello_world.rq` — файл с исходным кодом)*
