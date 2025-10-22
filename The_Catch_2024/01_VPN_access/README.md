# VPN access (1)

## Zadání

Hi, CSIRT trainee,

nobody can work secure from home without VPN. You have to install and configure OpenVPN properly. Configuration file can be downloaded from CTFd's link [VPN](ctfd_ovpn.ovpn). Your task is to activate VPN, visit the testing pages, and get your code proving your IPv4 and IPv6 readiness.

* IPv4 testing page is available at <http://bellatrix.cypherfix.tcc>.
* IPv6 testing page is available at <http://gerry.cypherfix.tcc>.

See you in the next incident!

## Nápovědy (Hints)

1. <https://openvpn.net/community-resources/reference-manual-for-openvpn-2-4/>
1. Do not run more different VPNs at once.

## Řešení

Dle zadání je potřeba nejdříve z adresy <https://www.thecatch.cz/vpn> stáhnout konfigurační soubor pro aktivaci VPN a připojení k interním systémům. Jedná se o konfigurační soubor pro OpenVPN, který stačí nainstalovat a spustit se staženým konfiguračním souborem.

`openvpn ctfd_ovpn.ovpn`

Po několika sekundách se VPN připojení aktivuje.

```text
2024-10-11 08:30:40 TUN/TAP device tun0 opened
2024-10-11 08:30:40 net_iface_mtu_set: mtu 1500 for tun0
2024-10-11 08:30:40 net_iface_up: set tun0 up
2024-10-11 08:30:40 net_addr_v4_add: 10.200.0.24/21 dev tun0
2024-10-11 08:30:40 net_iface_mtu_set: mtu 1500 for tun0
2024-10-11 08:30:40 net_iface_up: set tun0 up
2024-10-11 08:30:40 net_addr_v6_add: 2001:db8:ffff::4c/64 dev tun0
2024-10-11 08:30:40 net_route_v4_add: 10.99.0.0/16 via 10.200.0.1 dev [NULL] table 0 metric -1
2024-10-11 08:30:40 add_route_ipv6(2001:db8:7cc::/64 -> 2001:db8:ffff::1 metric -1) dev tun0
2024-10-11 08:30:40 net_route_v6_add: 2001:db8:7cc::/64 via :: dev tun0 table 0 metric -1
2024-10-11 08:30:40 Initialization Sequence Completed
2024-10-11 08:30:40 Data Channel: cipher 'AES-256-CBC', auth 'SHA1', peer-id: 60
2024-10-11 08:30:40 Timers: ping 10, ping-restart 120
2024-10-11 08:30:40 Protocol options: protocol-flags cc-exit tls-ekm dyn-tls-crypt
```

Úkol slouží pro otestování správné funkce OpenVPN tunelu pro IPv4 i IPv6. Ze dvou odkazů se složí výsledný FLAG.

## Flag

`FLAG{Hgku-58OA-Hsrn-03Zr}`
