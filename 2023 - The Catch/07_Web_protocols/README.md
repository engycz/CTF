# Web protocols (2)

## Zadání

Ahoy, officer,

almost all interfaces of the ship's systems are web-based, so we will focus the exercise on the relevant protocols. Your task is to identify all webs on given server, communicate with them properly and assembly the control string from responses.

May you have fair winds and following seas!

The webs are running on server `web-protocols.cns-jv.tcc`.

## Nápovědy (Hints)

1. Be aware that `curl` tool doesn't do everything it claims.
2. Correct response contains at least one cute cat picture.
3. Use VPN to get access to the server.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Pokus o otevření stránky ze zadání skončí chybou. Vyhledáme tedy otevřené porty a jejich služby pomocí programu `nmap`

`nmap web-protocols.cns-jv.tcc -p- -A -T5`

```text
PORT     STATE SERVICE        VERSION
5009/tcp open  airport-admin?
| fingerprint-strings:
|   DNSStatusRequestTCP, DNSVersionBindReqTCP, FourOhFourRequest, GetRequest, HTTPOptions, Help, JavaRMI, LANDesk-RC, LPDString, NotesRPC, RTSPRequest, SIPOptions, X11Probe, afp, giop:
|     HTTP/1.1 400 Bad Request
|_    Unsupported protocol version
5011/tcp open  http           Werkzeug httpd 1.0.1 (Python 3.10.13)
|_http-server-header: Werkzeug/1.0.1 Python/3.10.13
|_http-title: Site doesn't have a title (text/html; charset=utf-8).
5020/tcp open  http           Werkzeug httpd 1.0.1 (Python 3.10.13)
|_http-server-header: Werkzeug/1.0.1 Python/3.10.13
|_http-title: Site doesn't have a title (text/html; charset=utf-8).
8011/tcp open  http           nginx 1.22.1
|_http-server-header: nginx/1.22.1
|_http-title: Site doesn't have a title (text/html; charset=utf-8).
8020/tcp open  ssl/http       nginx 1.22.1
| ssl-cert: Subject: commonName=www.example.com/organizationName=Company Name/stateOrProvinceName=Oregon/countryName=US
| Not valid before: 2023-08-20T19:33:36
|_Not valid after:  2024-08-19T19:33:36
|_http-title: Site doesn't have a title (text/html; charset=utf-8).
| tls-alpn:
|   h2
|   http/1.1
|   http/1.0
|_  http/0.9
|_ssl-date: TLS randomness does not represent time
|_http-server-header: nginx/1.22.1
```

Když zkusíme v prohlížeči <http://web-protocols.cns-jv.tcc:5009>, vrátí

`Unsupported protocol version`

Ve výstupu z `nmap` je vidíme, že jsou kromě verze 1.1 podporovány i další protokoly. Ve Firefoxu je možné změnit v `about:config` v nastavení `network.http.version`. Zkusíme

* h2 - stejný výsledek
* 1.1 - stejný výsledek
* 1.0 - stejný výsledek
* 0.9 - stránka vrátí nějaká data

```text
HTTP/0.9 200 OK

SESSION=RkxBR3trckx0;
iVBORw...
```

Zajímavé je `SESSION=RkxBR3trckx0;`. Z jiné úlohy již víme, že takto začíná FLAG zakódovaný v Base64. Zde to odpovídá
`FLAG{krLt`

Nastavíme zpět výchozí hodnotu verze HTTP komunikace a zkusíme další port. <http://web-protocols.cns-jv.tcc:5011>, vrátí Base64 data a v SESSION je další část `LXJ2YnEtYWJJ`.

<http://web-protocols.cns-jv.tcc:8011>, vrátí Base64 data a v SESSION je `LXJ2YnEtYWJJ` stejné jako u portu 5011.

<http://web-protocols.cns-jv.tcc:8020>, vrátí chybu

```text
400 Bad Request
The plain HTTP request was sent to HTTPS port
```

Použijeme tedy adresu <https://web-protocols.cns-jv.tcc:8020>, která vrátí Base64 data a v SESSION je `Ui00MzNBfQ==`.

Složíme všechny části do `RkxBR3trckx0LXJ2YnEtYWJJUi00MzNBfQ==`, což odpovídá hledanému FLAGu

## Flag

`FLAG{krLt-rvbq-abIR-433A}`
