# Business as Usual (1)

## Zadání

Hi Associate Researcher,

an anonymous source has shared a PDF document originating from the Department of Hidden Messages. At first glance, it appears to be nothing more than another copy of one of the department's routine public paper. However, the department has been distributing documents with nearly identical contents for quite some time, and the reason for doing so remains unclear. Your task is to analyze the document.

Stay curious!

Download the [PDF document](document_DoHM.pdf).

## Nápovědy (Hints)

1. The Department's employees collectively own 28 cats. Could this help? Who knows, this Department is very special!
2. A capable PDF viewer with sufficient zoom may reveal details that are invisible at first sight — even feline whiskers.

## Řešení

Součástí úlohy je PDF dokument, ve kterém se vyskytují obrázky kočiček.

![ ](cat.png)

Jedná se o šifru, která se skládá z písmen, na které se kočičky dívají.

`Hi,you!Go to:hiddenpaws6.tcc`

Na adrese <http://hiddenpaws6.tcc> se nachází text `Department of Hidden Messages
Message No. 6` a obrázek kočičky. Flag nikde

![ ](image.png)

Jedná se o steganografii. Když nahrajeme obrázek na <https://www.aperisolve.com/>, objeví se flag.

![ ](flag.png)

## Flag

`FLAG{THIS-ISMY-SPOT-OKAY}`
