# Route tracking (2)

## Zadání

Hi, packet inspector,

all our delivery vans use password instead of standard car keys. Today, we have found out that the AI has implemented a new security measure – the vans are now referred as "AES Vans" and the password has been changed and encrypted. The decryption part is not yet finished, so we can't start any delivery van since morning!

Good news is that we managed to get the latest version of the decryption script from git repository. Bad news is that the script is not finished yet! Your task is to the finalize the script and decrypt the password as soon as possible.

Download [the script and encrypted password](van_keys.zip) (MD5 checksum `e67c86a277b0d8001ea5b3e8f6eb6868`).

May the Packet be with you!

## Nápovědy (Hints)

1\. Correct error(s) and add missing part(s).

## Řešení

Archiv obsahuje soubor `van_keys_enc.aes` se zakódovaným flagem a neúplný Python program `code.py` s chybami pro jeho dekódování. V programu chybí importy a místy dvojtečky a dále tabulátor místo mezer. Po opravě chyb a spuštění nahlásí program chybějící soubor `pi_dec_1m.txt`. Google pomůže - jedná se o soubor s číslem pi na 1 000 000 míst, který se dá stáhnout např. z <http://pi2e.ch/blog/wp-content/uploads/2017/03/pi_dec_1m.txt>.

Do programu je ještě potřeba doplnit načtení souboru `van_keys_enc.aes`:

```python
with open('van_keys_enc.aes', 'rb') as f:
    data = f.read()

print(obj.decrypt(data))
```

Celý opravený program je zde [code.py](code.py).

## Flag

`FLAG{ITRD-Pyuv-JuLt-9zpM}`
