# domanek_project_1

## Aufgabenstellung
Simulation einer Datenübertragung von ganzen Zahlen basierend auf der Kodierung Signed LEB128 wobei Zahlen (zufällig zwischen ‐100000 und 100000; über die Kommandozeile konfigurierbar) zwischen zwei Threads übertragen werden sollen. 
Es sollen permanent Zahlen im Sekundentakt übertragen werden, wobei die Übertragung als String stattfinden soll. Es sind promise und future Paare zur Kommunikation zu verwenden.


## Beschreibung

Standartmäßig werden zufällige Dezimalzahlen zu signed LEB128 konvertiert und zwischen 2 Threads übertragen und wieder zurück zu einer Dezimalzahlen konvertiert. 
Über Kommandozeileninterface ist es möglich:
- unsigned LEB128 zu verwenden
- Start und Ende des Bereichs der Zufallszahlen festzulegen
- Werte über CLI einlesen und wiederholt übertragen
- Werte in Kommandozeile ausgeben oder in JSON schreiben
- Dauer zwischen der Übertragung festlegen
- TOML Konfigurationsfile einlesen die alle genannten Optionen beinhalten kann 


## Issues
Hier sind die [Issues](/../../issues)

## Libaries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt) <!-- fehlt -->
- [json](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog) <!-- fehlt -->
- [toml++](https://github.com/marzer/tomlplusplus/)
### vielleicht
- [criterion](https://github.com/p-ranav/criterion)
- [rang](https://github.com/agauniyal/rang)
- [tabulate](https://github.com/p-ranav/tabulate)