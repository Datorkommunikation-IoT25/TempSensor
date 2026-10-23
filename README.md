# Temperaturövervakning med ESP32

## Beskrivning
Lösningen läser av temperaturen från en DHT11-sensor med en ESP32 och skickar mätvärden till en MQTT-broker via Wi-Fi. Temperaturen publiceras som ett JSON-meddelande var femte sekund.

## Lösningens delar
- **Sensor:** Läser av temperaturen från DHT11.
- **Wi-Fi:** Ansluter ESP32 till det trådlösa nätverket.
- **MQTT:** Ansluter till en MQTT-broker och publicerar temperaturdata.
- **Programloop:** Kontrollerar anslutningen och skickar nya mätvärden med fem sekunders intervall.

Temperaturdata skickas i JSON-format med enhets-ID, temperaturvärde och enhet, exempelvis:

```json
{
  "deviceId": "sensor-01",
  "value": 23.5,
  "unit": "°C"
}
```

## Konfiguration
Fyll i följande uppgifter i koden innan start:
- Wi-Fi-nätverkets namn och lösenord.
- MQTT-brokerns adress och port.
- MQTT-användarnamn och lösenord, om sådana krävs.
- MQTT-topic för publicering.

Dessa uppgifter anges i respektive konstanter i koden. MQTT-porten måste också anges för att anslutningen ska fungera.

Installera även nödvändiga Arduino-bibliotek: `DHT`, `ESP32MQTTClient` och deras eventuella beroenden.

## Starta applikationen
1. Öppna `temp-sensor.ino` i Arduino IDE.
2. Välj rätt ESP32-kort och tillhörande COM-port.
3. Anslut ESP32 till datorn via USB.
4. Kompilera och ladda upp programmet till kortet.
5. Öppna Serial Monitor med baud rate **115200** för att se anslutningsstatus och temperaturdata.
6. Starta eller kontrollera att MQTT-brokern är tillgänglig. Använd en MQTT-klient för att prenumerera på det konfigurerade topicet och se inkommande mätvärden.

## Dataflöde
DHT11 → ESP32 → Wi-Fi → MQTT-broker → MQTT-klient eller annan konsument.
