# DNS madness (2)

## Zadání

Hi, TCC-CSIRT analyst,

for infrastructure management, the Department of Operations must maintain a large number of DNS records in many zones. The DNS administrator has gone crazy from the sheer number of records, sitting in front of the monitor, swaying back and forth, and repeatedly saying "It came to me, my own, my love, my own, my precious `infra.tcc`". Your task is to examine the zones, resource records, and their counts so that we can hire a sufficiently resilient administrator next time.

See you in the next incident!

## Nápovědy (Hints)

1. On the administrator's desk lies a well-worn copy of RFC (number of RFC is unreadable due to a huge coffee stain).
1. In the past weeks, the administrator has been passionately talking about a new method for provisioning secondary servers in which a list of zones to be served is stored in a DNS zone and can be propagated to slaves via AXFR/IXFR.

## Řešení

Na základě nápověd se bude jednat o [RFC 9432](https://www.rfc-editor.org/rfc/rfc9432.pdf).

Zjistíme si, jaký DNS server hostuje doménu `infra.tcc`.

`dig +noall +answer ANY infra.tcc`

```text
infra.tcc.              68440   IN      SOA     jessiejames.infra.tcc. hostmaster.infra.tcc. 2024100701 604800 86400 2419200 86400
infra.tcc.              59243   IN      NS      jamison.infra.tcc.
infra.tcc.              59243   IN      NS      jameson.infra.tcc.
```

Je to tedy server `jessiejames.infra.tcc`.

Seznam zón se dá vyčíst z katalogu.

`dig +noall +answer AXFR catalog.infra.tcc @jessiejames.infra.tcc`

```text
catalog.infra.tcc.      86400   IN      SOA     . . 2024100802 604800 86400 2419200 86400
catalog.infra.tcc.      86400   IN      NS      invalid.
version.catalog.infra.tcc. 86400 IN     TXT     "2"
aldoriainfratcc.zones.catalog.infra.tcc. 3600 IN PTR aldoria.infra.tcc.
aldorninfratcc.zones.catalog.infra.tcc. 3600 IN PTR aldorn.infra.tcc.
arionisinfratcc.zones.catalog.infra.tcc. 3600 IN PTR arionis.infra.tcc.
averoninfratcc.zones.catalog.infra.tcc. 3600 IN PTR averon.infra.tcc.
banneroninfratcc.zones.catalog.infra.tcc. 3600 IN PTR banneron.infra.tcc.
beloriainfratcc.zones.catalog.infra.tcc. 3600 IN PTR beloria.infra.tcc.
...
```

Vyčteme obsah každé zóny a vyfiltrujeme pouze TXT záznamy.

`dig +short AXFR catalog.infra.tcc @jessiejames.infra.tcc | xargs dig @jessiejames.infra.tcc AXFR | grep TXT`

```text
40ae12928dbf450106d8097a7ec875ea.banneron.infra.tcc. 86400 IN TXT "RkxBR3tBdlhPLWlNazctM2JvSC1pWURwfQ=="
```

Po rozkódování pomocí Base64 získáme FLAG.

## Flag

`FLAG{AvXO-iMk7-3boH-iYDp}`
