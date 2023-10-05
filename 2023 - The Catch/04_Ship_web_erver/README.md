# Ship web server (1)

## Zadání

Ahoy, deck cadet,

there are rumors that on the ship web server is not just the official presentation. Your task is to disprove or confirm these rumors.

May you have fair winds and following seas!

Ship web server is available at <http://www.cns-jv.tcc>.

## Nápovědy (Hints)

1. Use VPN to get access to the ship web server.
1. Check the content of the certificate of the web.
1. Visit the other web sites hosted on the same server. Don't let non-existent DNS records to stop you.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Po otevření stránky z odkazu v zadání se zobrazí varování o samopodepsaném certifikátu. Po zobrazení certifikátu zjistíme, že je vydaný pro tyto DNS názvy.

```text
www.cns-jv.tcc
documentation.cns-jv.tcc
home.cns-jv.tcc
pirates.cns-jv.tcc
structure.cns-jv.tcc
```

Po akceptaci certifikátu se zobrazí stránky, v jejíž patičce je řetězec

`ver. RkxBR3sgICAgLSAgICAtICAgIC0gICAgfQ==`

Po dekódování pomocí Base64 získáme

`FLAG{    -    -    -    }`

Když zkusíme otevřít další stránky z certifikátu, tak připojení skončí s chybou. Příkaz

`nslookup documentation.cns-jv.tcc`

také skončí s chybou. Je tedy nutné manuálně zadat jejich IP adresu, jak nám radí hint č. 3. IP adresa `www.cns-jv.tcc` je `10.99.0.64`. Doplníme tedy do souboru `/etc/hosts`

```text
10.99.0.64 documentation.cns-jv.tcc
10.99.0.64 home.cns-jv.tcc
10.99.0.64 pirates.cns-jv.tcc
10.99.0.64 structure.cns-jv.tcc
```

Nyní již stránky na dalších doménách otevřít jdou. Na stránce <https://documentation.cns-jv.tcc> je v patičce

`RkxBR3sgICAgLSAgICAtICAgIC1nTXdjfQ==` což odpovídá `FLAG{    -    -    -gMwc}`

Na stránce <https://home.cns-jv.tcc> není v patičce nic, ale když otevřeme libovolný odkaz tak najdeme v patičce

`RkxBR3tlamlpLSAgICAtICAgIC0gICAgfQ==` což odpovídá `FLAG{ejii-    -    -    }`

Na stránce <https://pirates.cns-jv.tcc> je v patičce

`RkxBR3sgICAgLSAgICAtUTUzQy0gICAgfQ==` což odpovídá `FLAG{    -    -Q53C-    }`

Na stránce <https://structure.cns-jv.tcc> není další část na první pohled viditelná, ale prozkoumáním obsahu HTML najdeme

`RkxBR3sgICAgLXBsbVEtICAgIC0gICAgfQ==` což odpovídá `FLAG{    -plmQ-    -    }`

Složením všech částí získáme FLAG

## Flag

`FLAG{ejii-plmQ-Q53C-gMwc}`
