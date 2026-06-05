# Zadanie 2 - Laboratoria Programowanie Aplikacji w Chmurze Obliczeniowej
>**Autor**: Szymon Kasperczuk, 101587, s101587@pollub.edu.pl
>Obraz: `ghcr.io/SKasStudent/weather-server`
>Cache: `docker.io/s101587/weather-cache`

## Opis i sposób działania łańcucha
|Krok|Opis|
|----|----|
|**Checkout**|Pobiera kod źródłowy z repozytorium|
|**QEMU Setup**|Emulator do budowania obrazow na architekture arm64|
|**Buildx Setup**|Buildx - do obslugi wielu architektur|
|**Dockerhub Login**|Logowanie do dockerhuba|
|**GHCR Login**|Logowanie do ghcr|
|**Metadata**|Generowanie tagów i etykiet dla obrazow|
|**BuildCVE**|Zbudowanie obrazu do Skanu CVE|
|**ScanCVE**|Skanowanie obrazu przy użyciu Trivy|
|**Push**|Po udanym skanie cve budowa finalnego obrazu na obie architektury i push do repozytorium ghcr|

## Schemat Tagowania Obrazów
**Przyjęty schemat:**
|Tag|Użycie|
|---|------|
|sha-<short_hash>| Hash który posiada każdy commit - jest unikalny dla każdej wersji |
|latest| Zawsze wskazuje na najnowszy build |
|cache| Stały tag używany przy cache'u |
**Uzadadnienie:**
Schemat tagowania 'sha+latest' jest najbardziej rekomendowanym schematem w dokumentacji Docker i innych zaufanych źródłach:
- **Docker Docs** ([Build, tag, and publish an image](https://docs.docker.com/get-started/docker-concepts/building-images/build-tag-and-publish-an-image/))
- **Microsoft Azure Container Registry** ([Image Tag Best Practices](https://learn.microsoft.com/en-us/azure/container-registry/container-registry-image-tag-version))
Z tych źródeł wynika, że ten schemat tagowania:
- Zapewnia wygodę dostępu do najnowszej wersji (latest)
- Umożliwia wygodne audyty, rollbacki i identyfikacje wersji (sha)

Tag cache'u jest stały ponieważ jest każdorazowo, nadpisywany najnowszym stanem.
## Wybór narzędzia skanowania
Do wykonania skanu CVE zostało użyte narzędzie Trivy.
**Uzasadnienie:**
-Docker scout wymaga oddzielnego logowania
-Przy użyciu parametru exit-code: 1, pipeline zostaje automatycznie przerwany przy wykryciu zagrożeń ( W tym przypadku HIGH, CRITICAL )
-Pełna funkcjonalność docker scout jest zablokowana w wersji darmowej.

## Potwierdzenie działania:
<img width="1604" height="523" alt="Screenshot From 2026-06-05 17-30-51" src="https://github.com/user-attachments/assets/dcc640a4-63f4-49bd-853b-2309f33e039b" />

