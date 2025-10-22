# Naval chef's recipe (2)

## Zadání

Ahoy, officer,

some of the crew started behaving strangely after eating the chef's speciality of the day - they apparently have hallucinations, because they are talking about sirens wailing, kraken on starboard, and accussed the chef being reptilian spy. Paramedics are getting crazy, because the chef refuses to reveal what he used to make the food. Your task is to find his secret recipe. It should be easy as the chef knows only security by obscurity and he has registered domain `chef-menu.galley.cns-jv.tcc`. May you have fair winds and following seas!

The chef's domain is `chef-menu.galley.cns-jv.tcc`.

## Nápovědy (Hints)

1. Use VPN to get access to the server.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Když použijeme příkaz `curl http://chef-menu.galley.cns-jv.tcc` na načtení stránky, je hledaný FLAG ihned zobrazen.

```html
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
  <title>301 Moved Permanently</title>
  <meta http-equiv="refresh" content="0;url=https://chef-menu.galley.cns-jv.tcc">
</head><body>
<h1>Moved Permanently</h1>
<p>The document has moved <a href="https://chef-menu.galley.cns-jv.tcc">here</a>.</p>
<p style="display: none">The secret ingredient is composed of C6H12O6, C6H8O6, dried mandrake, FLAG{ytZ6-Pewo-iZZP-Q9qz}, and C20H25N3O. Shake, do not mix.</p>
<script>window.location.href='https://chef-menu.galley.cns-jv.tcc'</script>
</body></html>
```

Stránka obsahuje v hlavičce\
`<meta http-equiv="refresh" content="0;url=https://chef-menu.galley.cns-jv.tcc">`\
takže pokud ji otevřeme v prohlížeči, jsme okamžitě přesměrování na https verzi stránky.

## Flag

`FLAG{ytZ6-Pewo-iZZP-Q9qz}`
