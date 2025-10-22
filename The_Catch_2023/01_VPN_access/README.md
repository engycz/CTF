# VPN access (1)

## Zadání

Ahoy, deck cadet,

a lot of ship systems is accessible only via VPN. You have to install and configure OpenVPN properly. Configuration file can be downloaded from CTFd's link [VPN](ctfd_ovpn.ovpn). Your task is to activate VPN and visit the testing page.

May you have fair winds and following seas!

Testing page is available at <http://vpn-test.cns-jv.tcc>.

## Nápovědy (Hints)

1. <https://openvpn.net/community-resources/reference-manual-for-openvpn-2-4/>
1. Do not run more different VPNs at once.

## Řešení

Dle zadání je potřeba nejdříve z adresy <https://www.thecatch.cz/vpn> stáhnout konfigurační soubor pro aktivaci VPN a připojení k interním systémům. Jedná se o konfigurační soubor pro OpenVPN, který stačí nainstalovat a spustit se staženým konfiguračním souborem.

`openvpn ctfd_ovpn.ovpn`

Po několika sekundách se VPN připojení aktivuje.

```text
2023-10-04 13:19:17 TUN/TAP device tun0 opened
2023-10-04 13:19:17 net_iface_mtu_set: mtu 1500 for tun0
2023-10-04 13:19:17 net_iface_up: set tun0 up
2023-10-04 13:19:17 net_addr_v4_add: 10.200.0.36/21 dev tun0
2023-10-04 13:19:17 net_route_v4_add: 10.99.0.0/24 via 10.200.0.1 dev [NULL] table 0 metric -1
2023-10-04 13:19:17 Initialization Sequence Completed
```

a je možné otevřít stránku <http://vpn-test.cns-jv.tcc> na které je zobrazen následující flag.

## Flag

`FLAG{smna-m11d-hhta-ONOs}`
