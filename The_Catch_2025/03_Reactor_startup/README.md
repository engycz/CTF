# Reactor startup (1)

## Zadání

Hi, trainee,

normally, this task would be assigned to a senior worker, but none are available now, so your mission will be to start the shut-down reactor at the Granite Peak Nuclear as quickly as possible — by following the startup checklist and entering the sequence step by step. However, we have a big problem: the standard items (e.g., "primary circuit leak test") have somehow been renamed to nonsense by some cat-holic. Only the first item, `Initiate Control Circuits`, and the last one, `Phase the Power Plant`, remain unchanged.

The radiation situation at Granite Peak Nuclear remains normal.

Stay grounded!

Interface for starting sequence is at [http://gpn.powergrid.tcc/](http://gpn.powergrid.tcc/)

## Nápovědy (Hints)

1. All services in powergrid.tcc domain are accessible via VPN only.

1. Don’t worry, the reactor has built-in safeguards, so in case of an incorrect sequence it will automatically shut down again.

1. Some simple automation with a script would be quite handy (doing it manually is feasible, but would be a gargantuan task).

## Řešení

Na webové stránce ze zadání je možné zadávat povely reaktoru. První je `Initiate Control Circuits`. Pokud se zadá `Phase the Power Plant`, vypíše se povel, který byl očekáván. Podle nápovědy bude tedy nejlepší řešit úlohu skriptem, ve kterém budeme postupně přidávat příkazy.

```python
import requests
import re

s = requests.Session()

commands = ["Initiate Control Circuits"]
lastCommand = "Phase the Power Plant"

while True:
    for command in commands:
        r = s.post("http://gpn.powergrid.tcc/", data={"command": command})

    r = s.post("http://gpn.powergrid.tcc/", data={"command": lastCommand})

    if "Invalid sequence. Item" in r.text:
        missingCommand = r.text.split("&#039;")
        print(missingCommand[1])
        commands.append(missingCommand[1])
    else:
        print(re.search(r".*(FLAG{.*}).*", r.text).group(1))
        break
```

## Flag

`FLAG{WuYg-ynFt-US0N-ZYv9}`
