# Download backup (2)

## Zadání

Hi, packet inspector,

our former employee Brenda (head of PR department) was working on new webpage with superdiscount code for VIP customers, but she get fired by AI because of "disturbing lack of machine precision".

Your task is to find the code as soon as possible. The only hope is an automated backup of Brenda's `Download` directory (there is a high probability that she had downloaded the discount page or part of it).

Download [the backup file](download_backup.zip) (MD5 checksum `2fd749e99a0237f506a0eb3e81633ad7`).

May the Packet be with you!

## Nápovědy (Hints)

1\. Brenda's favorite browser was `MS Edge`, i.e. she used `MS Windows` (running the filesystem `NTFS`).

## Řešení

Součástí ZIP archivu je další RAR archiv a soubor s heslem `password`. Archiv rozbalíme pomocí `7z x download_backup.rar`. Ve výstupním adresáři jsou sobory

```text
 14839798 Aug  8 07:45 1098612x18781390.pdf
      539 Aug  8 07:45 1098612x18781390.pdf:Zone.Identifier
   834077 Aug  8 07:41 img.png
      161 Aug  8 07:41 img.png:Zone.Identifier
    55385 Aug  8 07:44 thecatch2022-form-header.png
      127 Aug  8 07:44 thecatch2022-form-header.png:Zone.Identifier
    67811 Aug  8 07:42 xDracula_08-03-2012.jpg
      172 Aug  8 07:42 xDracula_08-03-2012.jpg:Zone.Identifier
```

Soubor `img.png:Zone.Identifier` obsahuje URL <http://self-service.mysterious-delivery.thecatch.cz/> na které je uveden hledaný flag.

Bližší info viz <https://www.digital-detective.net/forensic-analysis-of-zone-identifier-stream/>.

## Flag

`FLAG{16bd-0c4x-ZRJe-8HC3}`
