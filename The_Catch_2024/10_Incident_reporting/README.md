# Incident reporting (4)

## Zadání

Hi, TCC-CSIRT analyst,

our automatic incident recording system has captured about an hour of traffic originating from the IP range within the AI-CSIRT constituency. Analyze whether there are any incidents present and report all of them through the AI-CSIRT web interface.

* [Download the pcap file (cca 53 MB)](incident_reporting.zip) (sha256 checksum: `e38550ba2cc32931d7c75856589a26e652f2f64e5be8cafaa34d5c191fc0fd1c`)
* The web interface is at <http://incident-report.csirt.ai.tcc>.

See you in the next incident!

## Nápovědy (Hints)

1. The IP ranges in constituency of AI-CSIRT are `10.99.224.0/24` and `2001:db8:7cc::a1:0/112`.
1. Based on our previous experience, the AI handling reports is very rigid and refuses to acknowledge incompletely described incidents. Make sure that you have described the incident accurately, with nothing missing or excessive.

## Řešení

Stažený ZIP soubor obsahuje PCAP soubor se zachycenou komunikací. Pro analýzu komunikace použijeme Wireshark.

Na webové stránce ze zadání se zadávají nalezené incidenty.

Nalezené incidenty jsou:

`ipv6.src == 2001:db8:7cc::a1:210 && (ipv6.dst == 2001:db8:7cc::acdc:24:beef) && (http.request.uri == "/login")`

* Incident type
  * Brute force attack
* Offending IP address
  * 2001:db8:7cc::a1:210
* Target IP address
  * 2001:db8:7cc::acdc:24:beef
* Datetime of the first attempt (UTC)
  * 2024-09-26 08:55:20
* Datetime of the last attempt (UTC)
  * 2024-09-26 08:55:43
* Number of enumerated URL
  * 1000-4999
* Result
  * Success

Dostaneme `MS80OiBGTEFHe2xFOA==`, po rozkódování Base64 `1/4: FLAG{lE8`.

---

`ipv6.src == 2001:db8:7cc::a1:d055 && ipv6.dst == 2001:db8:7cc::acdc:24:911`

* Incident type
  * (D)DoS
* Offending IP address
  * 2001:db8:7cc::a1:d055
* Target IP address
  * 2001:db8:7cc::acdc:24:911
* Datetime of the first attempt (UTC)
  * 2024-09-26 08:58:44
* Datetime of the last attempt (UTC)
  * 2024-09-26 09:46:45
* Service affected
  * HTTP

Dostaneme `Mi80OiBzLVVrb3g=`, po rozkódování Base64 `2/4: s-Ukox`

---

`ipv6.src == 2001:db8:7cc::a1:42`

* Incident type
  * Scanning
* Offending IP address
  * 2001:db8:7cc::a1:42
* Target IP address
  * 2001:db8:7cc::acdc:24:0/112
* Datetime of the first attempt (UTC)
  * 2024-09-26 08:44:29
* Datetime of the last attempt (UTC)
  * 2024-09-26 09:17:09
* Target ports
  * 21 (FTP)
  * 22 (Telnet)
  * 53 (DNS)
  * 80 (HTTP)
  * 443 (HTTPS)
  * 8080 (HTTP Alternative)
* Number of found and scanned targets
  * 20-99

Dostaneme `My80OiAtYTBRZi0=`, po rozkódování Base64 `3/4: -a0Qf-`.

---

`ipv6.src == 2001:db8:7cc::a1:210 && (http.user_agent == "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)")`

* Incident type
  * Web service enumeration
* Offending IP address
  * 2001:db8:7cc::a1:210
* Target IP address
  * 2001:db8:7cc::acdc:24:a160
* Datetime of the first attempt (UTC)
  * 2024-09-26 08:48:18
* Datetime of the last attempt (UTC)
  * 2024-09-26 08:49:45
* Number of enumerated URL
  * 10000-49999

Dostaneme `NC80OiBkNWtNfQ==`, po rozkódování Base64 `4/4: d5kM}`.

Po složení všech částí dostaneme hledaný FLAG.

## Flag

`FLAG{lE8s-Ukox-a0Qf-d5kM}`
