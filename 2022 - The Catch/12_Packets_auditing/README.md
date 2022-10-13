# Packets auditing (3)

## Zadání

Hi, packet inspector,

the AI has "upgraded" our packet auditing system – time to time, it generates archive of pictures, where the state of packet and the appropriate delivery team is indicated by different colours for each packet transport number.

We have a plea from `Brenda's delivery team` to find their missing packet in state `ready for pickup` (the other teams have already delivered all their packages mentioned in last given audit archive).

Download [audit archive](https://owncloud.cesnet.cz/index.php/s/BGSbaBDCsuWdAYO) (MD5 checksum `08ee155d2c9aee13ea5cab0a11196129`), find the desired pickup code and enter it on webpage <http://pickup.mysterious-delivery.thecatch.cz> to collect pickup code.

May the Packet be with you!

## Nápovědy (Hints)

1\. Too many images for manual processing, right?

## Řešení

Po rozbalení archivu obsahuje výstupní adresář 25002 souborů typu PNG. Jeden z nich je `description.png`, který obsahuje přiřazení jmen, stavů a barev. Podle zadání musíme najít obrázek pro Brendu a se stavem `ready for pickup`

![ ](Brenda.png)

![ ](Ready.png)

Tedy oranžový podklad se zeleným balíčkem.

- oranžová - RGB (242, 121, 48)
- zelená - RGB (0, 133, 71)

Protože je souborů moc, pomůžeme si programem [FindPackage.py](FindPackage.py), který projde všechny PNG soubory a vyhledá požadovanou kombinaci.

```python
from pathlib import Path
from PIL import Image

files = list(Path(".").rglob("*.png"))

cnt = 0
for file in files:
    cnt = cnt + 1
    if cnt % 1000 == 0:
        print(str(cnt)+'/'+str(len(files)))

    img = Image.open(str(file))
    px = img.load()
    if px[0,0] == (242, 121, 48):
        if px[125, 125] == (0, 133, 71):
            print(file)
```

Hledaný obrázek je `2022-08\30\19\000000.png`

![ ](000000.png)

Kód `629-367-219-835` opíšeme na stránku ze zadání a získáme flag.

## Flag

`FLAG{rNM8-Aa5G-dF5y-6LqY}`
