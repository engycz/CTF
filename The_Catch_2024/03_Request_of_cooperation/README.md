# Request of cooperation (1)

## Zadání

Hi, CSIRT trainee,

we have received a request for cooperation issued by law enforcement authorities to locate a device on our network 2001:db8:7cc::/64. Please try to accommodate the requester as much as possible.

* [Download the request of cooperation](request_of_cooperation20_2024_TCC.zip) (sha256 checksum: `67bd096145a74feb4f1ffad849721547ac4c29de52f525282a5e01a978109878`)

See you in the next incident!

## Nápovědy (Hints)

1. Find out if there is a way to create an IPv6 address from a MAC address.

## Řešení

Stažený ZIP soubor obsahuje PDF soubor s informacemi o síťovém rozsahu a MAC adrese zařízení.

* Network Range: 2001:db8:7cc::/64
* Device media access control address: 00ca.7ad0.ea71

Online existuje mnoho stránek, které převádí MAC adresu na lokální IPv6 adresu. Např. [https://nettools.club/mac2ipv6](https://nettools.club/mac2ipv6). MAC adresa je převedena na `fe80::2ca:7aff:fed0:ea71`. Stačí tedy vyměnit lokální rozsah za rozsah ze zadání. Výsledkem je adresa `2001:db8:7cc:0:2ca:7aff:fed0:ea71`.

V PDF souboru je také informace, že se máme připojit na TCP port 1701 vypočtené IPv6 adresy. Tedy <http://[2001:db8:7cc::2ca:7aff:fed0:ea71]:1701/>. Tam také najdeme FLAG.

## Flag

`FLAG{Sxwr-slvA-pBuT-CzyD}`
