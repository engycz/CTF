# Old webpages (1)

## Zadání

Hi, packet inspector, the AI has apparently some problems to transfer data from previous information system to new one. All packages in state "waiting for pickup" were erroneously moved to state "delivered". Now, we have an angry customer in our depot and she want her package with shipment ID 2022-0845.

In the previous IS, each package had his own domain name (for example, `ID 2022-0845` can be tracked on <http://tracking-2022-0845.mysterious-delivery.thecatch.cz)>.

Find the pickup code for package `2022-0845` as soon as possible, so we can give it to depot drone.

May the Packet be with you!

## Nápovědy (Hints)

1\. The previous system was really old, one can say even ancient or archive.

## Řešení

Po otevření stránky ze zadání se zobrazí informace `No code for already delivered packages.`, tedy žádný kód pro vyzvednuté zásilky. Protože má každá zásilka svojí doménovou adresu, mohla by být historie stránek uchována pomocí [web.archive.org](http://web.archive.org/web/20220808090332/http://tracking-2022-0845.mysterious-delivery.thecatch.cz). Ve verzi z 8.10.2022 je možné najít flag.

## Flag

`FLAG{pUVd-t1k9-DbkL-4r5X}`
