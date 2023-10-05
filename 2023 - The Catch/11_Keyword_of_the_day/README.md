# Keyword of the day (4)

## Zadání

Ahoy, officer,

one of deck cadets (remember your early days onboard) had a simple task to prepare simple web application to announce keyword of the day. He claimed that the task is completed, but he forgot on which port the application is running. Unfortunately, he also prepared a lot of fake applications, he claims it was necessary for security reasons. Find the keyword of the day, so daily routines on ship can continue.

May you have fair winds and following seas!

The webs are running somewhere on server `keyword-of-the-day.cns-jv.tcc`.

## Nápovědy (Hints)

1. Use VPN to get access to the server.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Skenováním portů zjistíme, že na portech 60000..60495 běží WEB server. Všechny webové stránky jsou si podobné, obsahují obfuscovaný skript, který se dá snadno deobfuscovat

```js
function getRandomInt(_0x12721b, _0x4bd30f) {
  _0x12721b = Math.ceil(_0x12721b);
  _0x4bd30f = Math.floor(_0x4bd30f);
  return Math.floor(Math.random() * (_0x4bd30f - _0x12721b) + _0x12721b);
}
setTimeout(function () {
  fn = getRandomInt(1, 4);
  document.getElementById("loader").style.display = "none";
  qn = "8d1c936eeb";
  document.getElementById("myImage").src = "img/" + fn + ".png";
}, getRandomInt(1, 7) * 1000);
```

Každou sekundu se mění hodnota proměnné `qn`.

Jednoduchým skriptem zjistíme, jestli se hodnota `qn` mění u všech portů

```python
import requests

portMin = 60000
portMax = 60495

data = [{},{}]
for k in range(0, 2):
    for i in range(portMin, portMax + 1):
        try:
            print(k, i)
            r = requests.get("http://keyword-of-the-day.cns-jv.tcc:"+str(i), timeout=0.2)
            data[k][i] = r.text
        except:
            pass

for i in range(portMin, portMax + 1):
    if i in data[0]:
        if data[0][i] == data[1][i]:
            print("==== " + str(i) + " ====")
```

Ve dvou čteních se nezměnila u portu 60257. Po zobrazení stránky <http://keyword-of-the-day.cns-jv.tcc:60257/>

![Alt text](948cd06ca7.png)

A na adrese <http://keyword-of-the-day.cns-jv.tcc:60257/948cd06ca7> už je hledaný FLAG.

## Flag

`Your flag is FLAG{DEIE-fiOr-pGV5-8MPc}`
