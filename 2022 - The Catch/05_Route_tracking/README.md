# Route tracking (2)

## Zadání

Hi, packet inspector,

our company uses on-board route recorders, so traffic controller can optimize movement of all vehicles and also control the schedule. Any route can be described by a text string that contains the codes of individual sites in the order in which they were visited (except depot, because each drive starts and also ends there).

Unfortunately, one of the recorders has been damaged and the particular sites were not recorded, just the total length of the route is known (exactly 163 912 meters). In addition, the driver told us that he never visited the same place more than once (except depot, of course).

Your task is to identify the exact route of the vehicle.

Download [the map of vehicle operating area and backround info](route_tracking.zip) (MD5 checksum `5fd3f52bcb404eae543eba68d7f4bb0a`).

May the Packet be with you!

## Nápovědy (Hints)

1\. The map of vehicle operating area is available as a picture and also in dot language.

## Řešení

Cílem úlohy je najít takovou cestu, při které bude součet vzdáleností 163 912m a zároveň se každým uzlem projede maximálně jednou. Ve staženém archivu je soubor `Area_52.png` s obrázkem spojů mezi uzly včetně vzdáleností a tak0 soubor `Area_52.dot` s textovým popisem grafu.

PS: Obrázek je z textové předlohy generován pomocí GraphViz.

Pro hledání cesty, která by splnila pravidla, je nejlepší řešit programově (viz [Route.py](Route.py)). Definice uzlů a cest je přepsaná přímo do kódu a pro urychlení výpočtu se testuje začátek cesty na `FLAG{`.

## Flag

`FLAG{SLiH-QPWV-hIm5-hWcU}`
