# Bitcoin wallet (1)

## Zadání

Hi, promising candidate,

our customers paying by bitcoin to our wallet `bc1q8vnufzpyurlnvrxavrn2vxe5z0nafrp2d8nzng` can get their package pickup code on <http://pay-check.mysterious-delivery.thecatch.cz> by entering their wallet ID.

Find out the pickup code for package that has not yet been claimed, although it was already paid for on Aug 8th 2022.

May the Packet be with you!

## Nápovědy (Hints)

1\. The question is: What is a blockchain?

## Řešení

Pro nalezení flagu je potřeba nejdříve prozkoumat Bitcoinovou peněženku `bc1q8vnufzpyurlnvrxavrn2vxe5z0nafrp2d8nzng`. Pro zobrazení Bitcoin blockchain transakcí je k dispozici několik online služeb, například <https://www.blockchain.com/explorer>. Po vyhledání `bc1q8vnufzpyurlnvrxavrn2vxe5z0nafrp2d8nzng` získáme z výpisu transakcí číslo zdrojové peněženky `bc1qrqqjjuefgc4akxl05cd4haxp5jznmmptjrllft`. Po jejím zadán na stránce <http://pay-check.mysterious-delivery.thecatch.cz> je zobrazena informace o platbě a po stisku tlačítka `Show` je zobrazen flag.

## Flag

`FLAG{PWei-v9hV-tekF-ptEl}`
