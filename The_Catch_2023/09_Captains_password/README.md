# Captain’s password (2)

## Zadání

Ahoy, officer,

our captain has had too much naval espresso and is temporary unfit for duty. The chief officer is in command now, but he does not know the captain's passwords for key ship systems. Good news is that the captain uses password manager and ship chief engineer was able to acquire captain's computer memory crash dump. Your task is to acquire password for signalization system.

May you have fair winds and following seas!

Download the [database and memory dump](https://owncloud.cesnet.cz/index.php/s/LhKWx4kA8xWQq25/download).

(MD5 checksum: `7c6246d6e21bd0dbda95a1317e4ae2c9`)

## Nápovědy (Hints)

1. At first, identify the password manager.

## Řešení

Stažený ZIP soubor obsahuje výpis paměti a soubor s uloženými hesly program KeePass Password Safe. K tomuto programu existuje [CVE-2023-32784](https://nvd.nist.gov/vuln/detail/CVE-2023-32784) a PoC <https://github.com/vdohney/keepass-password-dumper>.

Po stažení, přeložení a spuštění programu je vypsáno

`Combined: ●{), ÿ, a, :, |, í, W, 5, , r, ¸}ssword4mypreciousship`

Heslo je tedy `password4mypreciousship`.

Po otevření souboru hesel v programu KeePass Password Safe a zadání nalezeného hesla už snadno najdeme FLAG u záznamu `Main Flag System`.

## Flag

`FLAG{pyeB-941A-bhGx-g3RI}`
