# VPN Test (1)

## Zadání

Hi Associate Researcher,

nobody can work secure without VPN. You have to install and configure OpenVPN properly. Configuration file can be downloaded from  CTFd's link [VPN](ctfd_ovpn.ovpn). Your task is to activate VPN, visit the testing pages, and get your code proving your IPv4 and IPv6 readiness.

Stay curious!

* IPv4 testing page is available at <http://yesterday.nren.tcc>.
* IPv6 testing page is available at <http://tomorrow.nren.tcc>.

## Nápovědy (Hints)

1. <https://openvpn.net/community-resources/installing-openvpn/>

## Řešení

Dle zadání je potřeba nejdříve z adresy <https://www.thecatch.cz/vpn> stáhnout konfigurační soubor pro aktivaci VPN a připojení k interním systémům. Jedná se o konfigurační soubor pro OpenVPN, který stačí nainstalovat a spustit se staženým konfiguračním souborem.

`openvpn ctfd_ovpn.ovpn`

Po několika sekundách se VPN připojení aktivuje.

```text
2026-10-06 19:28:51 Protocol options: protocol-flags cc-exit tls-ekm dyn-tls-crypt
2026-10-06 19:28:56 TEST ROUTES: 2/2 succeeded len=2 ret=1 a=0 u/d=up
2026-10-06 19:28:56 C:\WINDOWS\system32\route.exe ADD 10.99.0.0 MASK 255.255.0.0 10.200.0.1
2026-10-06 19:28:56 ROUTE: CreateIpForwardEntry succeeded with dwForwardMetric1=25 and dwForwardType=4
2026-10-06 19:28:56 Route addition via ipapi [adaptive] succeeded
2026-10-06 19:28:56 C:\WINDOWS\system32\route.exe ADD 10.100.0.0 MASK 255.255.0.0 10.200.0.1
2026-10-06 19:28:56 ROUTE: CreateIpForwardEntry succeeded with dwForwardMetric1=25 and dwForwardType=4
2026-10-06 19:28:56 Route addition via ipapi [adaptive] succeeded
2026-10-06 19:28:56 add_route_ipv6(2001:db8:7cc::/64 -> 2001:db8:ffff::1 metric -1) IF 7
2026-10-06 19:28:56 IPv6 route added using ipapi
2026-10-06 19:28:56 add_route_ipv6(2001:db8:7c26::/64 -> 2001:db8:ffff::1 metric -1) IF 7
2026-10-06 19:28:56 IPv6 route added using ipapi
2026-10-06 19:28:56 Initialization Sequence Completed
```

Úkol slouží pro otestování správné funkce OpenVPN tunelu pro IPv4 i IPv6. Ze dvou odkazů se složí výsledný FLAG.

## Flag

`FLAG{7JTu-5A8s-bPlH-f4rD}`
