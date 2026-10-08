# It's a Trap! (2)

## Zadání

Hi Senior Researcher,

we've received a report about an unregistered web service operating somewhere in the `10.100.26.0/24` network. There is one problem: the subnet has become unusually noisy. Every address and every port seems eager to talk, driving our AI-driven automated scanner completely mad.

Your task is to find the web service.

Stay curious!

## Nápovědy (Hints)

1. Archived documentation for CIDR `10.100.26.0/24` contains a link to
[this document](its_a_trap.pdf). Maybe it can help.

## Řešení

Dokument obsahuje informaci, že na rozsahu 10.100.26.0/24 je spuštěna detekce útoků, kterou hlídá LaBrea (<https://github.com/Hirato/LaBrea/>).

Klíčové chování LaBrea:

* Obsadí nepřidělené IP adresy v podsíti a vytvoří na nich virtuální stroje.
* Na každý TCP port každé obsazené adresy odpoví SYN/ACK (handshake se dokončí).
* Navíc potvrdí spojení SYN/ACK s nastaveným TCP oknem na hodnotu 3, což je neobvyklé, ale dají se podle toho detekovat IP adresy, které LaBrea obsadila.

nmap na první IP v rozsahu 10.100.26.1 ukázal otevřené porty 22 a 5666

```text
PORT     STATE SERVICE
22/tcp   open  ssh
5666/tcp open  nrpe
```

Nmap neumí filtrovat podle TCP window, tak si vedle spustíme tcpdump s filtrem jen na SYN+ACK a window různé od 3.

`tcpdump -i tun0 -nn "tcp[tcpflags] & 0x12 == 0x12 and tcp[14:2] != 3"`

A spustíme nmap na celý rozsah a port 22

`nmap -sT -p 22 10.100.26.0/24 -n --host-timeout 1s`

V tcpdumpu se vypíší jen IP adresy 10.100.26.1, 10.100.26.10 a 10.100.26.66

Spustíme nmap na nelezené IP adresy

`└─# nmap -p- -n 10.100.26.1 10.100.26.10 10.100.26.66`

```text
Nmap scan report for 10.100.26.1
Host is up (0.019s latency).
Not shown: 65533 closed tcp ports (reset)
PORT     STATE SERVICE
22/tcp   open  ssh
5666/tcp open  nrpe

Nmap scan report for 10.100.26.10
Host is up (0.016s latency).
Not shown: 65533 closed tcp ports (reset)
PORT     STATE SERVICE
22/tcp   open  ssh
5666/tcp open  nrpe

Nmap scan report for 10.100.26.66
Host is up (0.026s latency).
Not shown: 65532 closed tcp ports (reset)
PORT     STATE SERVICE
22/tcp   open  ssh
2025/tcp open  ellpack
5666/tcp open  nrpe
```

Na <http://10.100.26.66:2025> se ukrývá hledaný flag.

PS: Schovaná služba je vzpomínka na Pavla Vachka, autora přiloženého článku, který LaBrea do CESNETu přinesl <https://www.cesnet.cz/novinky/vzpominka-na-pavla-vachka-260>.

## Flag

`FLAG{O2pS-yj4k-Qd8U-v9ft}`
