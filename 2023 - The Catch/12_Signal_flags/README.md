# Signal flags (5)

## Zadání

Ahoy, officer,

after a reboot at 2023-10-02 11:30:18 UTC, the On-board signal flag recognition system (OBSF-RS) has malfunctioned. The signal flags are no more recognized and the only working function is to generate and save schematic views, which are created every time a ship in the vicinity changes its signaling. We need to know what the surrounding ships signaled and if we have missed something important.

May you have fair winds and following seas!

Download the [schematic views](signal_flags.zip).

(MD5 checksum: `c1820a3c41a0d6a0daacd84b69fd1b57`)

## Nápovědy (Hints)

1. Two consecutive signalization of the same vessel in a time interval of less than 2 minutes are part of one message.

## Řešení

Stažený ZIP soubor obsahuje 90 obrázků lodí se signalizačními vlajkami. Jedná se o námořní vlajkovou abecedu
![Alt text](image.png)

Po ručním dekódování vlajek u souboru `signalization_eqxboejickzswuiivlmjyphhpeciqbfj.png` dostaneme `0x43616e20796f752073`
Po převodu na ASCII část zprávy `Can you s`.

Když ručně dekódujeme všechny soubory, dostaneme

|Soubor|Text z vlajky|Dekódovaný text|
|--|--|--|
|signalization_eqxboejickzswuiivlmjyphhpeciqbfj|0x43616e20796f752073|Can you s|
|signalization_qronxprykjntmvcwcihtxevtefglpcea|k||
|signalization_xhbbnbucjhrheenctftiicbloqzjbojz|kl||
|signalization_cenithsibphpqiyzelzsrmksqfnsljes|0x64647265737365733f|ddresses?|
|signalization_gtzwweieommowkqgtvxkwbqndlzlpois|0x52656c61793a204861|Relay: Ha|
|signalization_gxsfjwnfbtztajgmrmklrrkzpthwbcbh|0x656c6c20757320736f|ell us so|
|signalization_mmfutnnhmbuwikijmmxvafvoscaureed|0x6d6520697076342061|me ipv4 a|
|signalization_ucrcxftmrzpgvksdmkdupahttpwhebjg|0x6c65746b613f|letka?|
|signalization_wreirpozghydrwqjgtzmtqjjrxlyhtqw|0x656e204d722e204361|en Mr. Ca|
|signalization_zzdkdzzmihnwplqldurdutuokyszakxg|0x766520796f75207365|ve you se|
|signalization_anqmnnkdilsdfqlqbbrtjrrbqtalnrgl|v||
|signalization_dfbcwuvrspgymxfanfixdsewullrmebg|0x2062726f6b656e| broken|
|signalization_jhwlxnkmestyepexockmlcurcsoneyoi|0x68696e65206973|hine is|
|signalization_kkglwxloneopikuenkdskiitjhjnxyws|0x666565206d6163|fee mac|
|signalization_naxonqjkhfmkcovdmbzmldmzpzuvvavy|0x2c207765206e65|, we ne|
|signalization_qgqzzsupoikjhhsrhnqunnsaixirwlsh|0x4f757220636f66|Our cof|
|signalization_ajshslvsnbevfdnoncbpylqgspvplqur|0x5061727479206f6e207468|Party on th|
|signalization_ikeosnxvfwqyhievhomuhjuwjnxosuil|0x2032313030205a554c552e| 2100 ZULU.|
|signalization_kiddxaeumfxjrijmscnwucgbeoatdieb|0x65642061206d65|ed a me|
|signalization_nueujtmhxwbqwjrradxhcyyrroefdszd|x||
|signalization_vgptsygrpaiosvgesiwjwcelwumalkdt|0x6520627269646765206174|e bridge at|
|signalization_xqmdpcjxntzevzjazqfbchnytzuekclz|vf||
|signalization_xskdiiwbwnkotcykqydqswbfdiaryxee|0x6368616e69632e|chanic.|
|signalization_askooyynbtnbsktiqcegkwfyfencnkim|0x467265652070696e6f7420|Free pinot |
|signalization_jagbfhvsjomvplwzwjjkxnkcnxztyjra|0x6572796f6e6521|eryone!|
|signalization_lmvaisoycxceyqholqthljfdhjgwiqar|0x434e53204a6f73656620566572696368|CNS Josef Verich|
|signalization_vlgbgamyiilvsyavsqhsadavcysqtuwr|0x6772697320666f72206576|gris for ev|
|signalization_zvdaadtixobvgkvpxolkjlqolsjxoxxp|bv||
|signalization_faymrpjbajotiyqxwuiuakimuvzhgpun|0x726b2067757920736574207570206120|rk guy set up a |
|signalization_omlrfmokmamoszvfopkgeemjvprtfhcw|bv||
|signalization_owyphcthfjiogtsskewqpqbqsufyekfs|e||
|signalization_qbtyjmwocjujdnhjkmsdqctuucngarts|0x726f7574657220666f722075733f|router for us?|
|signalization_urqlxpwijqjrbtexkqphvckibsjigsbp|0x2c2063616e20796f7572206e6574776f|, can your netwo|
|signalization_xidfjmtojrijkfhgxrkeepbfzrdioemt|honeymoon||
|signalization_bgongwndgpxslbchkesjtuflpurvgcis|0x20796f757220| your |
|signalization_omubfbsfdsrbgfupwawqlpytytxftjrd|0x686174206973|hat is|
|signalization_rctgricoofibtwqyexzurafcoybmqzbz|0x4865792c2077|Hey, w|
|signalization_rgkkrjlqkcbakebcttotmjoxhrbzypno|0x776966692050|wifi P|
|signalization_tldakfqlalagsnldvkfzemgznwocllhk|k||
|signalization_ciuemipqmmckmprwmmabuznirrqenriy|0x656e204d722e|en Mr.|
|signalization_cwautabhaqfuunecnnjapvfhdvtsiaoe|0x796f75207365|you se|
|signalization_gjdbcyxeqzufatqmbqxtdexbhtgwdqcx|0x3f|?|
|signalization_hltqwxlrqfpmdvwrefabnnctdrcwojzw|0x204861766520| Have |
|signalization_ukcnmqdmqfjnutsafajpbagiabldavaj|asswoed||
|signalization_ytythebnnuyfbdsnibjwtgwrkvddledg|0x52656c61793a|Relay:|
|signalization_cmsnkxoklpmhiqgupphptwlbguuiopjr|0x434e53204a|CNS J|
|signalization_ffbyfhydersznxishwajuolbhceqtohw|0x6b613f|ka?|
|signalization_qzsyhkpxvmowpqygwbmkpyuftatljypd|verich||
|signalization_rnkwhtwrtzadigwrpswlxqjkmaixkrnk|0x2c20617265|, are|
|signalization_uxfvfxezfopmgljghgmvdyoflyuctong|0x6f73656620|osef |
|signalization_xkarfwleyjvhkivzibqpboyuhvtunwol|0x2043616c6574| Calet|
|signalization_fwxjmwekxsvvooplbrfcetjumzorvebx|0x20796f7572| your|
|signalization_jxiuguaiqfetbhccrsocxvhuihcbbgky|0x206f6b2c20| ok, |
|signalization_malytkmuxyyhoizzlrdslphxiebxgucf|0x206e657473| nets|
|signalization_nuyblkwgzodkbjhgjgfdmzrjlmqzlfdz|0x3b2d29||-)
|signalization_zbindrcgpjnqcispavsrfuxmxydymkbf|0x746f6f3f20|too? |
|signalization_buquypzjibuuqbmycjwzuqcanxmhdokk|0x2c20796f75|, you|
|signalization_cwxrnolhaezundjspjsvrjaqnhjpngyp|0x6f73656620|osef |
|signalization_dwinjgndjrrwckmjjwagbufpnebvkvjo|verich||
|signalization_dzkkbyftmolajmgqyeiryyivrqoegekc|0x434e53204a|CNS J|
|signalization_qccxgrxhvgnfandawbuwonpytkbrhjvf|improve||
|signalization_ygwllzvvmeamixrqpviwkvxiaafctwfc|0x2063616e20| can |
|signalization_aqhzrefkuxdymavxvpkmzguvftaqzifz|0x526b784252|RkxBR|
|signalization_eufsdqatkmokqhpxtgzazfhlwzcmtnyl|0x7657473474|vWG4t|
|signalization_eymcrjssbgwivpyxnbwiyriupuxidunq|0x20627920| by |
|signalization_ktwtfrlfjsnoqukcysexvzghkxvrvlnh|0x207468656d| them|
|signalization_kzipwwkhjfttxumuwpdfkcfkobkydnen|0x4a484c544e|JHLTN|
|signalization_wkqiyufjejlddvrpdoyjairgychslbrv|0x3374735648|3tsVH|
|signalization_cimekmmmntzzeisntnmtkekkvifpblpn|0x6931614e48|i1aNH|
|signalization_clibjjykcqqsxncqusveikakzoitfnea|0x3d2021|= !|
|signalization_fytnqqtmiflldhttljkhvwcooazamptj|m||
|signalization_srjbvbrmsvfsalrkzndhyijgrhpcztfz|0x464e66513d|FNfQ=|
|signalization_zzoubizlyyipumvijaqyoudbsnngrmks|0x5957396154|YW9aT|
|signalization_brbezdorjcmststjxmvijsxmgsshmalm|0x205665726963682c20| Verich, |
|signalization_dhtdlbfedfykcfjoqkyzvyaugichcpke|0x434e53204a6f736566|CNS Josef|
|signalization_iuomilytqpucycidmkrtvcmifnqoprre|0x434e53204a6f736566|CNS Josef|
|signalization_jiaovotzbcnwdzgotxsbogondrzjnvlb|0x205665726963682c20| Verich, |
|signalization_uforuecpbsvmvbcistxbqmhbjngdihbf|0x6e69636520666c6167|nice flag|
|signalization_zqgtpzirjdgstercodmhnknefdxrukfl|0x6973204d722e204361|is Mr. Ca|
|signalization_aiaucdjpnsntplwncqozuymywjmhukwm|0x43616c65746b613f|Caletka?|
|signalization_gwpmyzutwwuaneucbblckpgqsnvyiebx|pe||
|signalization_nqrddyppighaotgvnhymrckyleogsqbg|0x7365656e204d722e20|seen Mr. |
|signalization_ryjypqpubwianhnzbavwthwoahjqirvi|0x4861766520796f7520|Have you |
|signalization_tpbgtvdfmtmiofzdobtcnrnqrlvgwmqm|0x626f6172643f|board?|
|signalization_wkqnlgjqwsojeiamybkmcybusfpsuptm|0x6c65746b61206f6e20|letka on |
|signalization_efohpauggoenqzhysleojdkwztyktsev|0x61766520796f7520|ave you |
|signalization_lcsupmmrqnorcqekocpigpapxlslottg|0x3c|<|
|signalization_qbpdlxbtjdgboyrgkocxsrlgeiyckdkp|0x7365656e204d722e|seen Mr.|
|signalization_sgavctduqwsplbpvztkbjuxyxnbrffyk|0x2043616c65746b61| Caletka|
|signalization_szrqzrcuyocbpmabxsyrocytlubcqftw|0x52656c61793a2048|Relay: H|

---
Zajímavé jsou řádky
|Soubor|Text z vlajky|Dekódovaný text|
|--|--|--|
|signalization_aqhzrefkuxdymavxvpkmzguvftaqzifz|0x526b784252|RkxBR|
|signalization_eufsdqatkmokqhpxtgzazfhlwzcmtnyl|0x7657473474|vWG4t|
|signalization_kzipwwkhjfttxumuwpdfkcfkobkydnen|0x4a484c544e|JHLTN|
|signalization_wkqiyufjejlddvrpdoyjairgychslbrv|0x3374735648|3tsVH|
|signalization_cimekmmmntzzeisntnmtkekkvifpblpn|0x6931614e48|i1aNH|
|signalization_srjbvbrmsvfsalrkzndhyijgrhpcztfz|0x464e66513d|FNfQ=|
|signalization_zzoubizlyyipumvijaqyoudbsnngrmks|0x5957396154|YW9aT|

---
Po seřazení podle času v obrázku
|Soubor|Text z vlajky|Dekódovaný text|
|--|--|--|
|signalization_aqhzrefkuxdymavxvpkmzguvftaqzifz|0x526b784252|RkxBR|
|signalization_wkqiyufjejlddvrpdoyjairgychslbrv|0x3374735648|3tsVH|
|signalization_kzipwwkhjfttxumuwpdfkcfkobkydnen|0x4a484c544e|JHLTN|
|signalization_eufsdqatkmokqhpxtgzazfhlwzcmtnyl|0x7657473474|vWG4t|
|signalization_zzoubizlyyipumvijaqyoudbsnngrmks|0x5957396154|YW9aT|
|signalization_cimekmmmntzzeisntnmtkekkvifpblpn|0x6931614e48|i1aNH|
|signalization_srjbvbrmsvfsalrkzndhyijgrhpcztfz|0x464e66513d|FNfQ=|

---
Po složení `RkxBR3tsVHJHLTNvWG4tYW9aTi1aNHFNfQ`, což už snadno dekódujeme na hledaný FLAG.

## Flag

`FLAG{lTrG-3oXn-aoZN-Z4qM}`
