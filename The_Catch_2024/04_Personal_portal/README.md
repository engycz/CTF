# Personal portal (1)

## Zadání

Hi, CSIRT trainee,

check if there is a leak of non-public data on the personal portal.

* The portal is accessible at <http://perso.cypherfix.tcc>.

See you in the next incident!

## Nápovědy (Hints)

1. Don't waste time trying to log in.
1. Base64 is most popular encoding.

## Řešení

Po otevření stránky z odkazu v zadání se zobrazí přihlašovací dialog. Díky nápovědě víme, že se nemáme pokoušet uhádnout heslo. Kromě vstupů pro jméno a heslo je v dialogu ještě odkaz na Help. Odkaz vede na stránku <http://perso.cypherfix.tcc/files?file=help.html> a po otevření se zobrazí seznam souborů. Ale po kliknutí na jakýkoliv z nich se zobrazí chyba, že takový odkaz neexistuje.

Stačí upravit odkaz s nápovědou na <http://perso.cypherfix.tcc/files?file=meeting2024-09-05.md>. Soubor se stáhne a na konci je text v kódování Base64 `RkxBR3tZemZOLVo5UlAtb2MxUC1hdURvfQo=`.

Po rozkódování pomocí Base64 dostaneme FLAG.

## Flag

`FLAG{YzfN-Z9RP-oc1P-auDo}`
