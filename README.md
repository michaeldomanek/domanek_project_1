# domanek_project_1

## Aufgabenstellung
Simulation einer Datenübertragung von ganzen Zahlen basierend auf der Kodierung Signed LEB128 wobei Zahlen (zufällig zwischen ‐100000 und 100000; über die Kommandozeile konfigurierbar) zwischen zwei Threads übertragen werden sollen. 
Es sollen permanent Zahlen im Sekundentakt übertragen werden, wobei die Übertragung als String stattfinden soll. Es sind promise und future Paare zur Kommunikation zu verwenden.


## Beschreibung

Standartmäßig werden zufällige Dezimalzahlen zu signed LEB128 konvertiert und zwischen 2 Threads übertragen und wieder zurück zu einer Dezimalzahlen konvertiert. 
Über Kommandozeileninterface ist es möglich:
- unsigned LEB128 zu verwenden
- Start und Ende des Bereichs der Zufallszahlen festzulegen
- Werte über CLI einlesen und dauerhaft übertragen
- Werte in Kommandozeile ausgeben oder in JSON, TOML, oder plain file schreiben
- Dauer zwischen der Übertragung festlegen


## Issues
Hier sind die [Issues](/../../issues)


## Libaries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)
- [pystring](https://github.com/imageworks/pystring)
- [spdlog](https://github.com/gabime/spdlog)
- [toml++](https://github.com/marzer/tomlplusplus/)
- [tfile](https://github.com/rec/tfile)
### vielleicht
- [criterion](https://github.com/p-ranav/criterion)
- [pprint](https://github.com/p-ranav/pprint)
- [rang](https://github.com/agauniyal/rang)