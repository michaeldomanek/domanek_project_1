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

## Verwendung

Usage: leb128 [OPTIONS]

| Option                   | Typ                           | Bedingungen             | Beschreibung                                 |
| ------------------------ | ----------------------------- | ----------------------- | -------------------------------------------- |
| -h,--help                | FLAG                          |                         | Print this help message and exit             |
| -u,--unsigned            | FLAG                          |                         | Encode with unsigned LEB128                  |
| --show-encoded           | FLAG                          |                         | Show encoded values                          |
| -s,--start               | INT:INT in [-100000 - 100000] | Excludes: --values      | Start of the range of random numbers         |
| -e,--end                 | INT:INT in [-100000 - 100000] | Excludes: --values      | End of the range of random numbers           |
| -d,--delay               | UINT                          |                         | Delay between data transfer in ms            |
| -v,--values              | INT:INT in [-100000 - 100000] | Excludes: --start --end | Values to transfer in a loop                 |
| --json-output-name       | TEXT                          |                         | Name of json output file                     |
| --toml-path              | TEXT:FILE                     |                         | Name of toml configuration file (overrides)  |

## Issues
Hier sind die [Issues](/../../issues)

## Libaries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [json](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)
- [toml++](https://github.com/marzer/tomlplusplus/)
