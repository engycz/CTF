# VPN Test (1)

## Zadání

Hi, trainee,

nobody can work secure without VPN. You have to install and configure OpenVPN properly. Configuration file can be downloaded from CTFd's link [VPN](ctfd_ovpn.ovpn). Your task is to activate VPN, visit the testing pages, and get your code proving your IPv4 and IPv6 readiness.

Stay grounded!

* IPv4 testing page is available at <http://volt.powergrid.tcc>.
* IPv6 testing page is available at <http://ampere.powergrid.tcc>.

## Nápovědy (Hints)

1. OpenVPN Install <https://openvpn.net/community-resources/installing-openvpn/>

## Řešení

Dle zadání je potřeba nejdříve z adresy <https://www.thecatch.cz/vpn> stáhnout konfigurační soubor pro aktivaci VPN a připojení k interním systémům. Jedná se o konfigurační soubor pro OpenVPN, který stačí nainstalovat a spustit se staženým konfiguračním souborem.

`openvpn ctfd_ovpn.ovpn`

Po několika sekundách se VPN připojení aktivuje.

```text
2025-10-12 19:39:43 Note: cipher 'AES-256-CBC' in --data-ciphers is not supported by ovpn-dco, disabling data channel offload.
2025-10-12 19:39:43 OpenVPN 2.6.8 [git:v2.6.8/3b0d9489cc423da3] Windows [SSL (OpenSSL)] [LZO] [LZ4] [PKCS11] [AEAD] [DCO] built on Nov 17 2023
2025-10-12 19:39:43 Windows version 10.0 (Windows 10 or greater), amd64 executable
2025-10-12 19:39:43 library versions: OpenSSL 3.1.4 24 Oct 2023, LZO 2.10
2025-10-12 19:39:43 DCO version: 1.0.0
2025-10-12 19:39:43 TCP/UDP: Preserving recently used remote address: [AF_INET]78.128.211.51:1194
2025-10-12 19:39:43 Socket Buffers: R=[65536->65536] S=[65536->65536]
2025-10-12 19:39:43 UDPv4 link local: (not bound)
2025-10-12 19:39:43 UDPv4 link remote: [AF_INET]78.128.211.51:1194
2025-10-12 19:39:43 TLS: Initial packet from [AF_INET]78.128.211.51:1194, sid=b329be7b 69a3f3b5
...
2025-10-12 19:39:50 Initialization Sequence Completed
```

Úkol slouží pro otestování správné funkce OpenVPN tunelu pro IPv4 i IPv6. Ze dvou odkazů se složí výsledný FLAG.

## Flag

`FLAG{mkuV-TEnW-slYz-TFnx}`
