# Sensor array (2)

## Zadání

Hi, emergency troubleshooter,

sensor data from the distribution network are being continuously transmitted to `broker.powergrid.tcc`. However, the outsourced provider went bankrupt last week, and no one else has knowledge of how to access these data. Find out how to regain access to the sensor array data.

Stay grounded!

## Řešení

Skenováním portů

`nmap broker.powergrid.tcc -p-`

se zobrazí jediný otevřený port.

```text
PORT     STATE SERVICE
1883/tcp open  mqtt
```

Jedná se o MQTT komunikaci. Pro čtení můžeme použít třeba [MQTT explorer](https://mqtt-explorer.com). Ale pro přihlášení je potřeba heslo.

Zkusíme ještě proskenovat UDP porty

`nmap broker.powergrid.tcc -sU --top-ports 100 -T5`

Zobrazí jediný otevřený port.

```text
PORT      STATE  SERVICE
161/udp   open   snmp
```

Vypíšeme si SNMP informace

`snmpwalk -v1 -c public broker.powergrid.tcc`

```text
iso.3.6.1.2.1.1.1.0 = STRING: "MQTT broker for power grid sensors. Only reader has the rights to subscribe to a topic!"
iso.3.6.1.2.1.1.3.0 = Timeticks: (61573674) 7 days, 3:02:16.74
iso.3.6.1.2.1.1.5.0 = STRING: "Mosquitto"
iso.3.6.1.2.1.1.6.0 = STRING: "DC A, area 51"
iso.3.6.1.2.1.1.7.0 = INTEGER: 1
```

S uživatelem `reader` a heslem `reader` se už MQTT explorer připojí a načte hledaný FLAG.

## Flag

`FLAG{0hs0-SiJm-TO5B-46HD}`
