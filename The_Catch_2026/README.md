# Řešení úloh soutěže The Catch 2026

The Catch je soutěž typu CTF (Capture The Flag) organizovaná FLAB (Forenzní laboratoří) CESNET. Soutěž je veřejně dostupná na webové stránce [www.thecatch.cz](https://www.thecatch.cz) a probíhala v termínu 5 October 2026 12:00 UTC - 2 November 2026 12:00 UTC.

Cílem je v každé úloze získat tzv. _flag_, který je ve formátu `FLAG{xxxx-xxxx-xxxx-xxxx}`. Hra je rozdělena do několika sekcí a celkem obsahuje následujících 23 úkolů (challenges):

1. The Story
   - [Introduction](00_Story) (0)
   - [Promotion](00_Story) (0)
   - [Epilogue](00_Story) (0)
1. Research Excellence Institute
   - [VPN Test](01_VPN_Test) (1) 🩸
   - [Dr. Dvorak's Document](02_Dr_Dvoraks_Document) (1)
   - [CRMS](03_CRMS) (1) 🩸
   - [Business as Usual](04_Business_as_Usual) (1)
1. They Lie to You!
   - [Chapter 1: Exploration](05_They_Lie_to_You) (2)
   - [Chapter 2: The Owner](05_They_Lie_to_You) (3)
   - [Chapter 3: Count with Me](05_They_Lie_to_You) (3)
   - [Chapter 4: Hidden Information](05_They_Lie_to_You) (4)
1. SALVOR
   - [Chapter 1: Act-er](06_SALVOR) (3)
   - [Chapter 2: 'Bəfər](06_SALVOR) (5)
   - [Chapter 3: Prompt](06_SALVOR) (1)
   - [Chapter 4: Raggad](06_SALVOR) (2)
1. Miscellaneous
   - [Document Scan](07_Document_Scan) (2)
   - [It's a Trap!](08_Its_a_Trap) (2)
   - [Science Hot News](09_Science_Hot_News) (2)
   - [Experimental Server](10_Experimental_Server) (4)
1. Internal Systems
   - [Data Storage](11_Data_Storage) (3)
   - [Ticket System](12_Ticket_System) (3)
   - [Secure Terminal Server](13_Secure_Terminal_Server) (3)
   - [Quantum Keys Repository](14_Quantum_Keys_Repository) (4)

Hodnota v závorce za názvem úlohy udává počet bodů za její vyřešení. Uvedená řešení jsou postupy, kterými jsem se dobral k získání FLAGu, ale nemusí se jednat o nejefektivnější nebo zamýšlený postup.

Scoreboard Top 20

| Place  | User               | Score  | Time            |
|--------|--------------------|--------|-----------------|
| 1      | fearless           | 50     | 0d 03:15:13     |
| 2      | matejsmycka        | 50     | 0d 01:35:02     |
| 3      | santomet           | 50     | 0d 06:05:08     |
| 4      | dlabi              | 50     | 0d 06:40:54     |
| 5      | Harpy01            | 50     | 0d 07:16:00     |
| 6      | capyplivl          | 50     | 0d 07:39:26     |
| 7      | SadParad1se        | 50     | 0d 07:50:05     |
| 8      | techmandancz       | 50     | 0d 08:01:58     |
| 9      | SIGKITTEN          | 50     | 0d 02:13:59     |
| 10     | skakacihrad        | 50     | 0d 08:07:13     |
| 11     | Grzechu            | 50     | 0d 09:34:14     |
| 12     | Kvikimart          | 50     | 0d 11:57:25     |
| 13     | zombie             | 50     | 0d 14:54:26     |
| 14     | charlie            | 50     | 0d 15:12:01     |
| **15** | **EngyCZ**         | **50** | **0d 19:27:10** |
| 16     | quackquack         | 50     | 0d 04:27:27     |
| 17     | nguyenphhse180511  | 50     | 0d 01:45:33     |
| 18     | H4ck3rL0rd         | 50     | 0d 04:26:40     |
| 19     | tomjab             | 50     | 0d 18:41:49     |
| 20     | lazie              | 50     | 0d 00:12:39     |
