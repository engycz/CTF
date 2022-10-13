# DNS storage (3)

## Zadání

Hi, packet inspector,

biggest surprise of the day is that the AI has started to use DNS as a storage for its own information. The data are stored in TXT resource records in the zone `mysterious-delivery.tcc`. The zone is deployed on DNS servers `ns1.mysterious-delivery.thecatch.cz` and `ns2.mysterious-delivery.thecatch.cz`.

Analyze content of zone and focus on any codes for our depot steel safes (AI has changed the access code and we hope it is stored right in the DNS zone).

May the Packet be with you!

## Nápovědy (Hints)

1\. The zone is secured by DNSSEC.

## Řešení

Díky nápovědě víme, že zóna `mysterious-delivery.tcc` je zabezpečena pomocí DNSSEC. Pro procházení DNSSEC zóny můžeme použít následující příkaz:

`ldns-walk @ns1.mysterious-delivery.thecatch.cz mysterious-delivery.tcc`

V zóně je několik stovek serverů, proto zpracování zautomatizujeme a pro každý server získáme maximum informací pomocí `dig ANY <server> @ns1.mysterious-delivery.thecatch.cz` a výsledek uložíme do souboru `zoneinfo`.

`ldns-walk @ns1.mysterious-delivery.thecatch.cz mysterious-delivery.tcc | awk '{system("dig " $1 " @ns1.mysterious-delivery.thecatch.cz")'} > zoneinfo`

Ve výstupním souboru je zajímavá řádka

`depot-secret-upon-flag.mysterious-delivery.tcc. 86400 IN TXT "secret code for steel safe is: RkxBR3tZcjMxLVhvWEUtNEZxOC02UElzfQ=="`

Pokud dekódujeme `RkxBR3tZcjMxLVhvWEUtNEZxOC02UElzfQ==` pomocí base64 získáme hledaný flag.

K procházení DNSSEC je možné použít také nmap skript `nmap -sSU -p 53 --script dns-nsec-enum --script-args dns-nsec-enum.domains=mysterious-delivery.tcc`, ale jen do verze nmap 7.70, protože v commitu `https://github.com/nmap/nmap/commit/0500811f5ae53036c5e99a92dc97a5e37dbcc9ff#diff-9036da4d88913df7949ffa3aa09f509b62af74228283f353bf94d676de3528a8` byla do skriptu zanesena chyba, kvůli které skript nefunguje. Chyba opravena a poslána ke schválení `https://github.com/nmap/nmap/pull/2544`.

## Flag

`FLAG{Yr31-XoXE-4Fq8-6PIs}`
