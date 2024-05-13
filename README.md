# Daemon synchronizacji plików

Daemon synchronizacji plików jest narzędziem przeznaczonym do synchronizowania plików między dwoma katalogami. Działa jako proces demoniczny w tle, okresowo skanując katalog źródłowy i kopiując nowe lub zmodyfikowane pliki do katalogu docelowego. Do porównywania ze sobą plików program wykorzystuje obliczanie sumy kontrolnej przy użyciu algorytmu SHA-1.

## Funkcje
- Porównywanie ze sobą plików z wykorzystaniem sumy kontrolnej przy użyciu algorytmu SHA-1.
- Możliwość ustawienia progu rozróżniającego małe i duże pliki, z różnymi mechanizmami kopiowania dla efektywności.
- Monitorowanie katalogu źródłowego w poszukiwaniu zmian i synchronizacja plików do katalogu docelowego.
- Obsługa opcjonalnego parametru czasu uśpienia, umożliwiającego kontrolowanie interwału między cyklami synchronizacji.
- Możliwość natychmiastowego obudzenia demona poprzez wysłanie sygnału SIGUSR1 do procesu demona.

## Wymagania wstępne
- Kompilator GCC
- Biblioteka OpenSSL
- System z rodziny Linux

## Uruchomienie

Aby uruchomić demona, należy użyć poniższej komendy:
```bash
./deamon <source_dir> <dest_dir> [sleep_time] [threshold]
```
- `<source_dir>`: Ścieżka do katalogu źródłowego.
- `<dest_dir>`: Ścieżka do katalogu docelowego.
- `[sleep_time]`: (Opcjonalne) Czas w sekundach pomiędzy kolejnymi synchronizacjami. Domyślnie ustawione na 300 sekund (5 minut).
- `[threshold]`: (Opcjonalne) Progowa wartość, powyżej której plik jest traktowany jako duży i kopiowany jest za pomocą bardziej efektywnej metody. Domyślnie ustawione na 1 MB.

## Obsługa sygnału SIGUSR1

Demon można również obudzić natychmiastowo, wysyłając mu sygnał SIGUSR1. 
```bash
kill -SIGUSR1 <pid>
```
Gdzie `<pid>` to identyfikator procesu demona.

## Przykład
```bash
./deamon /home/user/source /home/user/destination 300 1048576
```

Uruchomi demona monitorującego katalog `/home/user/source`, który synchronizuje zmiany z katalogiem `/home/user/destination` co 300 sekund. Pliki o rozmiarze powyżej 1 MB będą traktowane jako duże i kopiowane za pomocą bardziej efektywnych metod.

