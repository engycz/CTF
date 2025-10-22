# Cat code  (3)

## Zadání

Ahoy, officer,

due to the lack of developers on board, the development of the access code generator for the satellite connection was entrusted to the cat of the chief officer. Your task is to analyze the cat's creation and find out the code.

May you have fair winds and following seas!

Download the [cat code](cat_code.zip).

(MD5 checksum: `aac150b3f24e5b047ee99e25ad263f56`)

## Nápovědy (Hints)

1. Cats are cute and all, but studies have shown that they really aren't good developers.

## Řešení

Stažený ZIP soubor obsahuje dva python skripty. Po spuštění očekává skript zadání `kittens`, aby se posunul dál. Pak se spustí výpočet, který ale v rozumném čase neskončí.

Výpočet se spouští se součtem znaků slova `kittens`, tedy `770`.

Když si vytvoříme jednoduchý skript

```python
UNITE = 1
UNITED = 2

def meow(kittens_of_the_world):
    if kittens_of_the_world < UNITED:
        return kittens_of_the_world
    return meow(kittens_of_the_world - UNITE) + meow(kittens_of_the_world - UNITED)

for i in range(1,10):
    print(meow(i))
```

Tak z výpisu

```text
1
1
2
3
5
8
13
21
34
```

je zřejmé, že se jedná o Fibonacciho posloupnost. V souboru `meow.py` nahradíme výpočet rychlejší nerekurzivní funkcí

```python
def meow(kittens_of_the_world):
     a, b = 0, 1
     for _ in range(kittens_of_the_world):
         a, b = b, a+b
     return a
```

a po spuštění je ihned zobrazen hledaný FLAG.

## Flag

`FLAG{YcbS-IAbQ-KHRE-BTNR}`
