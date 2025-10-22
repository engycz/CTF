# Suspicious traffic (5)

## Zadání

Ahoy, officer,

the chief security officer has told you, he came across a very suspicious event in ship's security logs on the occasion of a planned monthly log check. The event is related to accessing the secret file `secret.db` on server on cargo deck. Luckily, the ship is equipped with an automatic network traffic recorder, which was activated by the suspicious event and provided corresponding packet capture. Your task is to confirm that the mentioned file has been exfiltrated and to examine its contents.

May you have fair winds and following seas!

Download the [pcap](suspicious_traffic.zip).

(MD5 checksum: `6e7cfe473b222ff766e04196e35df304`)

## Nápovědy (Hints)

1. Do a detailed analysis of the given file, because the main suspect is the auxiliary workforce J. Hook, who is known for unusual and impulsive behavior.

## Řešení

Stažený ZIP soubor obsahuje pcap soubor se zachycenou síťovou komunikací. Po otevření v programu Wireshark zjistíme, že od rámce č. 319 probíhá FTP komunikace

```text
220 (vsFTPd 3.0.3)
USER james
331 Please specify the password.
PASS james.f0r.FTP.3618995
230 Login successful.
SYST
215 UNIX Type: L8
TYPE I
200 Switching to Binary mode.
PORT 172,20,0,7,213,251
200 PORT command successful. Consider using PASV.
STOR home.tgz
150 Ok to send data.
226 Transfer complete.
PORT 172,20,0,7,149,183
200 PORT command successful. Consider using PASV.
STOR etc.tgz
150 Ok to send data.
226 Transfer complete.
QUIT
221 Goodbye.
```

V rámci této komunikace jsou uživatelem `james` nahrány soubory `home.tgz` a `etc.tgz`. Přenos těchto souborů můžeme z Wiresharku snadno uložit. V archivu `home.tgz` je zajímavý soubor `home/.bash_history`. Obsahuje řádku

`openssl enc -aes-256-cbc -salt -pbkdf2 -in secret.db -out secret.db.enc -k R3alyStr0ngP4ss!`

Tedy zakódování hledaného databázového souboru pomocí openssh a heslem `R3alyStr0ngP4ss!`. Nic dalšího v archivu `home.tgz` ani `etc.tgz` není.

Dále je v záznamu komunikace několikrát přenos souborů `hostory.db` a `employees.db`, které ale nehledáme a není v nich zajímavého. Dále je v zaznamenané komunikaci skenování pomocí `NMAP`. Zajímavá je část od rámce č. 2052 se záznamem šifrované komunikace protokolem SMB3. Tuto komunikaci je možné při znalosti hesla dekódovat. Návod na dekódování je na <https://medium.com/maverislabs/decrypting-smb3-traffic-with-just-a-pcap-absolutely-maybe-712ed23ff6a2>

Nejdříve je nutné zjistit heslo. Podle návodu vytvoříme hash

`james_admin::LOCAL.TCC:78c8f4fdf5927e58:8bc34ae8e76fe9b8417a966c2f632eb4:01010000000000003ab4fc1550e2d901b352a9763bdec89a00000000020018004100360037004600320042004100340045003800460032000100180041003600370046003200420041003400450038004600320004000200000003001800610036003700660032006200610034006500380066003200070008003ab4fc1550e2d901060004000200000008003000300000000000000000000000000000002581558b8f3cf059f3661e7cb3af60d9b63a7561b7f48607589fb37e551862b10a0010000000000000000000000000000000000009001e0063006900660073002f0073006d006200730065007200760065007200320000000000`

Z výpisu FTP komunikace známe heslo `james.f0r.FTP.3618995`. Pokud byl uživatel `john` při tvorbě hesla neopatrný, bude mít heslo pro SMB podobný formát.

K hledání hesla je možné využít buď `hashcat` nebo `john`.

`john hash.txt --mask=james_admin.f0r.SMB.?d?d?d?d?d?d?d`

Heslo `james_admin.f0r.SMB.8089078` bylo nalezeno.

Dále potřebujeme z hesla a dalších informací vytvořit _Random Session Key_. Python skript na výpočet je <https://gist.githubusercontent.com/khr0x40sh/747de1195bbe19f752e5f02dc22fce01/raw/11f5ed93c1786c65a9002be993b0ae7d50fcef27/random_session_key_calc.py>

`python random_session_key_calc.py -u james_admin -d LOCAL.TCC -p james_admin.f0r.SMB.8089078 -n 8bc34ae8e76fe9b8417a966c2f632eb4 -k 4292dac3c7a0510f8b26c969e1ef0db9`

```text
USER WORK: JAMES_ADMINLOCAL.TCC
PASS HASH: 7cf87b641c657bf9e3f75d93308e6db3
RESP NT:   a154f31a5ecc711694c3e0d064bac78e
NT PROOF:  8bc34ae8e76fe9b8417a966c2f632eb4
KeyExKey:  6a1d3b41cdf3d40f15a6c15b80d567d0
Random SK: 7a93dee25de4c2141657e7037dddb8f1
```

Random Session Key je `7a93dee25de4c2141657e7037dddb8f1`, Session ID je `49b136b900000000`.

Po vyplnění ve Wiresharku dojde k dešifrování SMB3 komunikace a je vidět, že dochází k přenosu souboru `secret.db.enc`. Soubor z komunikace uložíme a pomocí openssl dekryptujeme podle dříve získaného hesla `R3alyStr0ngP4ss!` z `bash_history`.

`openssl enc -d  -aes-256-cbc  -pbkdf2 -in secret.db.enc -out secret.db -k R3alyStr0ngP4ss!`

V `secret.db` už snadno najdeme hledaný FLAG.

## Flag

`FLAG{5B9B-lwPy-OfRS-4uEN}`
