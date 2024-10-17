# Old service (2)

## Zadání

Hi, TCC-CSIRT analyst,

rumors are spreading in the ticketing system that an old, unmaintained service is running somewhere in the network, which could be a security risk (even though it's called `3S - Super Secure Service`). Old-timers claim that it had the domain name `supersecureservice.cypherfix.tcc`. This name does not exist in the current DNS, but some information might still be available on the DNS server `ns6-old.tcc`, which will be shut down soon.

Explore the service and gather as much information as possible about 3S.

See you in the next incident!

## Nápovědy (Hints)

1. Any resource records can be useful.

## Řešení

Nejdřív zkusíme zjistit něco o adrese `supersecureservice.cypherfix.tcc`.

`dig +noall +answer ANY supersecureservice.cypherfix.tcc`

Žádný výsledek. Zkusíme se zeptat DNS serveru `ns6-old.tcc` ze zadání.

`dig +noall +answer ANY supersecureservice.cypherfix.tcc @ns6-old.tcc`

```test
supersecureservice.cypherfix.tcc. 86400 IN TXT  "Super secure service in testing mode, any records are hipsters friendly!"
supersecureservice.cypherfix.tcc. 86400 IN HINFO "TCC 686" "TCC-OS 20.20"
supersecureservice.cypherfix.tcc. 86400 IN SVCB 1 web3s-746865636174636832303234.cypherfix.tcc. alpn="h2,h3,mandatory=alpn" port=8020
supersecureservice.cypherfix.tcc. 86400 IN SVCB 4 web3s-7468656361746368323032343.cypherfix.tcc. alpn="h2,h3,mandatory=alpn" port=8020
supersecureservice.cypherfix.tcc. 86400 IN SVCB 2 web3s-7468656361746368323032342.cypherfix.tcc. alpn="h2,h3,mandatory=alpn" port=8020
supersecureservice.cypherfix.tcc. 86400 IN A    10.99.24.21
supersecureservice.cypherfix.tcc. 86400 IN AAAA 2001:db8:7cc::24:21
```

Na stránce <http://web3s-746865636174636832303234.cypherfix.tcc:8020/> je hledaný FLAG.

## Flag

`FLAG{yNx6-tH9y-hKtB-20k6}`
