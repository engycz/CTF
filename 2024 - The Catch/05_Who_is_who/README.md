# Who is who? (1)

## Zadání

Hi, TCC-CSIRT analyst,

we have found that many analysts are unclear about basic terms. Verify your understanding by taking the quiz on the <http://csirt-quiz.e-learning.tcc>.

See you in the next incident!

## Řešení

Po otevření stránky z odkazu v zadání se zobrazí stránka s kvízovými otázkami týkajících termínů ohledně zajištění kybernetické bezpečnosti.

* The first official CSIRT team in the Czech Republic.
  * CESNET-CERTS
* CSIRT team for the EU institutions and agencies.
  * CERT-EU
* Government CERT of the Czech Republic
  * GovCERT.CZ
* Czech CSIRT team with status certified by Trusted Introducer.
  * ALEF-CSIRT
* International organization of CSIRT teams.
  * FIRST
* The European Union Agency for Cybersecurity
  * ENISA
* Czech CSIRT team celebrating 20th anniversary.
  * CESNET-CERTS
* National CSIRT of the Czech Republic
  * CSIRT.CZ
* Community of European CSIRT teams.
  * TF-CSIRT
* Database of European CSIRT teams.
  * Trusted Introducer

Pokud jsme na všechny otázky odpověděli správně, zobrazí se hledaný FLAG.

Jde to řešit i skriptem

```python
import requests
from pyquery import PyQuery

quiz = {}

i = 0
while True:
    i += 1
    print('Pass: ' + str(i))
    data = requests.get('http://csirt-quiz.e-learning.tcc/')

    pq = PyQuery(data.text)

    question = pq('pre').text()

    answers = pq('li input')
    answers = [x.attrib['value'] for x in answers]

    if not question in quiz:
        quiz[question] = answers
    else:
        for answer in quiz[question].copy():
            if not answer in answers:
                quiz[question].remove(answer)

    ok = True
    for question in quiz:
        if len(quiz[question]) != 1:
            ok = False
            break

    if ok:
        break

quiz = sorted(quiz.items())

for question in quiz:
    print(question[0] + ' --> ' + question[1][0])
```

## Flag

`FLAG{ADT0-cp6f-s071-pXsQ}`
