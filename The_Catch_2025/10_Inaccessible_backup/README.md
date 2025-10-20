# Inaccessible backup (3)

## Zadání

Hi, emergency troubleshooter,

One of our servers couldn’t withstand the surge of pure energy and burst into bright flames. It is backed up, but no one knows where and how the backups are stored. We only have a memory dump from an earlier investigation available. Find our backups as quickly as possible.

Stay grounded!

* [Download (memory dump for analysis)](inaccessible_backup.zip)

## Nápovědy (Hints)

1. The server was running on Debian 12 Bookworm.

## Řešení

## Flag

Když prohledáme soubor ze zadání narazíme na tuto zajímavou řádku.

`<78>Sep  3 19:46:01 CRON[12716]: (root) CMD (eval $(keychain --eval --quiet /root/.ssh/backup_key) && /usr/bin/rsync --delete -avz /var/www/html/ bkp@backup.powergrid.tcc:/zfs/backup/www/ > /dev/null 2>&1)`

Je to log CRONu z nahrávání zálohy na `backup.powergrid.tcc` jako uživatel `bkp`.

Na počítači `backup.powergrid.tcc` je otevřený pouze port 22 (SSH).

Ze zálohy budeme postupně zkoušet privátní klíče, které začínají `-----BEGIN OPENSSH PRIVATE KEY-----` a zkoušíme

`ssh -i id_rsa bkp@backup.powergrid.tcc`

Správný privátní klíč je

```text
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACD/pNcxB72+l6g/JOpPhO9XcYjz/rC+n3Ql/v03dY4jSQAAAKCumQYsrpkG
LAAAAAtzc2gtZWQyNTUxOQAAACD/pNcxB72+l6g/JOpPhO9XcYjz/rC+n3Ql/v03dY4jSQ
AAAECvUkQRNBmF/imckIfnKnRCRCtb4XnZqYjSNAiw/ngWDf+k1zEHvb6XqD8k6k+E71dx
iPP+sL6fdCX+/Td1jiNJAAAAGGJrcEBiYWNrdXAucG93ZXJncmlkLnRjYwECAwQF
-----END OPENSSH PRIVATE KEY-----
```

Po připojení se rovnou zobrazí FLAG.

`FLAG{VDg1-MfVg-LsJI-NOS4}`
