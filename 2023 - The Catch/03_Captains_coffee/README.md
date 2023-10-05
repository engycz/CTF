# Captain's coffee (1)

## Zadání

Ahoy, deck cadet,

your task is to prepare good coffee for captain. As a cadet, you are prohibited from going to the captain's cabin, so you will have to solve the task remotely. Good news is that the coffee maker in captain's cabin is online and can be managed via API.

May you have fair winds and following seas!

Coffee maker API is available at <http://coffee-maker.cns-jv.tcc>.

## Nápovědy (Hints)

1. Use VPN to get access to the coffee maker.
1. API description si available at <http://coffee-maker.cns-jv.tcc/docs>.

## Řešení

K řešení této úlohy je potřeba připojení pomocí VPN z úlohy [VPN access](../01_VPN_access/README.md)

Po otevření stránky z odkazu v zadání se zobrazí JSON odpověď

```json
{
    "status":"Coffemaker ready",
    "msg":"Visit /docs for documentation"
}
```

Stránka <http://coffee-maker.cns-jv.tcc/docs> obsahuje popis API, které je možné z této stránky také volat.

Prvním krokem je zobrazení `coffeeMenu`.

`curl -X 'GET' 'http://coffee-maker.cns-jv.tcc/coffeeMenu' -H 'accept: application/json'`

```json
{
  "Menu": [
    {
      "drink_name": "Espresso",
      "drink_id": 456597044
    },
    {
      "drink_name": "Lungo",
      "drink_id": 354005463
    },
    {
      "drink_name": "Capuccino",
      "drink_id": 234357596
    },
    {
      "drink_name": "Naval Espresso with rum",
      "drink_id": 501176144
    }
  ]
}
```

Dalším nabízenou API funkcí je `makeCoffee`. Jedná se o API funkci typu POST ve které je třeba zadat `drink_id`. Seznam `drink_id` známe z volání první API funkce `coffeeMenu`.

Zkusíme 456597044.

`curl -X 'POST' 'http://coffee-maker.cns-jv.tcc/makeCoffee/' -H 'accept: application/json' -H 'Content-Type: application/json' -d '{"drink_id": 456597044}'`

Odpověď obsahuje FLAG

```json
{
  "message": "Your Espresso is ready for pickup",
  "validation_code": "Use this validation code FLAG{ccLH-dsaz-4kFA-P7GC}"
}
```

## Flag

`FLAG{ccLH-dsaz-4kFA-P7GC}`
