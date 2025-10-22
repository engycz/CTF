# Sonar logs (2)

## Zadání

Ahoy, officer,

each crew member must be able to operate the sonar and understand its logs. Your task is to analyze the given log file, and check out what the sonar has seen.

May you have fair winds and following seas!

Download the [logs](sonar_logs.zip).

(MD5 checksum: `b946f87d0231fcbdbc1e76e27ebf45c7`)

## Nápovědy (Hints)

1. The entry in the log consists of two parts - the timestamp and the message.
2. Update: Be aware that some devices do not use up-to-date libraries - this sonar, for example, is based on python and uses an old `pytz` library version 2020.4.

## Řešení

Stažený ZIP soubor obsahuje soubor se sonarovými logy. Zajímavé jsou řádky obsahující `Object detected in depth`. Například tato

`2023-10-02 00:52:22 America/Creston - Object detected in depth 65 (0x41)`

V každém záznamu je uvedena jiná časová zóna. Bude potřeba převést všechny časy do jedné časové zóny, seřadit a převézt čísla na ASCII znaky. Můžeme dle nápovědy využít Python knihovnu pytz a napsat si jednoduchý skript [solve.py](solve.py)

```python
from pytz import timezone
from datetime import datetime

lines = open('sonar.log').read().split('\n')
data = []
for line in lines:
    if 'Object detected in depth' in line:
        line = line.split(' ')
        time = datetime.strptime(line[0] + ' ' + line[1], '%Y-%m-%d %H:%M:%S')
        time = timezone(line[2]).localize(time)
        data.append([time, chr(int(line[8]))])

data.sort()
flag = ''
for line in data:
    flag = flag + line[1]

print(flag)
```

Výsledkem je `FLAG{3YAG-2rb-KWoZ-LwWmj}`. Díky nápovědě č. 3 víme, že byla použita starší verze `pytz` knihovny, takže zobrazený FLAG je špatně. Nainstalujeme si tedy správnou verzi.

`pip install pytz==2020.4`

a dostaneme již správný FLAG.

## Flag

`FLAG{3YAG-2rbj-KWoZ-LwWm}`
