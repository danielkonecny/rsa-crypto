# Projekt č. 2 - Implementace a prolomení RSA

## Zadání

V tomto projektu se seznámíte s asymetrickým šifrováním, konkrétně s algoritmem RSA. Váš program bude umět generovat parametry RSA (soukromý a veřejný klíč), šifrovat a dešifrovat. Dále bude umět prolomit RSA pomocí faktorizace slabého veřejného modulu. Vámi vytvořený faktorizační nástroj bude schopen faktorizovat nejen slabé RSA klíče, ale jakékoli složené číslo do 96 bitů.

## Algoritmus RSA

* Generuj dvě velká prvočísla p a q
* n = p * q
* phi ( n ) = (p - 1) * (q - 1)
* Zvol náhodně e mezi 1 a phi( n ) tak, že gcd(e, phi( n )) = 1
* Vypočítej d = inv(e, phi( n )) - inv je operace nalezení inverzního prvku (Multiplicative inverse). Popis tohoto algoritmu najdete v textu J. Nechvatal - Public-Key Cryptography (NIST SP 800-2), který je dostupný z hlavní stránky kurzu.
* Veřejný resp. soukromý klíč je potom dvojice (e,n) resp. (d,n)

Generování resp. testování prvočísel pomocí metody Solovay-Strassen, případně Miller-Rabin je podrobně popsané v textu Public-Key Cryptography.

## Při implementaci faktorizační části postupujte takto:

* Veřejný modulus nejdříve zkontrolujte pomocí metody "triviálního (zkusmého) dělení" pro "prvních 1 000 000 čísel".
    * Metoda triviálního dělení je nejjednodušší metodou pro faktorizaci celých čísel
    * Metoda pracuje na základě zvolení počátečního dělitele (např. 2) a následném ověření, zda dělitel opravdu dělí zadané číslo beze zbytku. Pokud ne, dojde k inkrementaci hodnoty dělitele a opět se ověřuje dělitelnost beze zbytku. Tento proces je opakován tak dlouho, dokud není nalezen správný dělitel.
    * Z principu metody lze vypozorovat, že se jedná o metodu s exponenciální časovou složitostí
    * Zároveň se jedná o nejpomalejší faktorizační metodu, a proto ji používáme pouze pro rychlé ověření, zda zadané složené číslo není dělitelné nějakým malým dělitelem (faktorem)
    * To bude také cílem i v našem případně, pouze rychle ověřit dělitelnost malých čísel touto metodou
    * Pokud se nad metodou zamyslíte, zjistíte, že lze tuto metodu optimalizovat a výrazně ji tak urychlit
* Pokud se nepodaří nalézt dělitele metodou triviálního dělení, bude nutné nasadit "sofistikovanější metodu"
    * Doporučuje se využít některou z ostatních faktorizačních metod s exponenciální časovou složitostí
    * Na rozdíl od metody triviálního dělení tyto metody již mají základy v teorii čísel, a jsou tedy mnohonásobně rychlejší
    * Díky tomu je lze efektivně využít i pro faktorizaci čísel o velikosti 96 bitů
    * Vybrat si můžete například jednu z těchto:
        * [Fermat Factorization Method](http://mathworld.wolfram.com/FermatsFactorizationMethod.html)
        * [Pollard Rho Method](http://mathworld.wolfram.com/PollardRhoFactorizationMethod.html)
        * [Pollard p-1 Method](http://mathworld.wolfram.com/Pollardp-1FactorizationMethod.html)
    * Obecně ale můžete použít jakoukoli jinou faktorizační metodu než triviální dělení, klidně i nějakou vlastní či nějakou se subexponenciální časovou složitostí, implementace takové metody je ale časově výrazně náročnější.
* Faktorizační nástroj musí být schopen faktorizovat jakékoli složené číslo až do délky "96 bitů" do "120s"
* Cílem implementace takového faktorizačního nástroje je vám ukázat:
    * Proč u asymetrické kryptografie používáme klíče mnohem větší délky než u kryptografie symetrické
    * Jak velký rozdíl v době faktorizace může být u metod, které spadají do stejné třídy časové složitosti 
      (triviální dělení vs jiná metoda s exponenciální časovou složitostí)
    * Proč je nutné brát i ohled na celkovou rychlost faktorizace
        * Pokud chcete vytvořit použitelný faktorizační nástroj, rychlost faktorizace je hlavním kritériem
        * Nikdo váš faktorizační nástroj nebude používat, pokud nějaké větší číslo faktorizuje za 2 dny, když existuje jiný faktorizační nástroj, který stejné číslo faktorizuje za 2 hodiny.

## Specifikace překladu

Překlad musí podporovat dvě metody, a to "release" a "debug". Ve verzi Release budou povoleny optimalizace překladu a linkování. Debug verze bude mít zapnuté ladicí symboly a sanitizéry "address" a "undefined behavior". Obě verze budou otestovány z hlediska správné implementace.

Protože se jedná o šifrovací algoritmus, musíte dbát na správný a bezpečný zápis zdrojového kódu, aby nedocházelo k chybám typu "memory leak" apod.

## Shrnutí zadání

Vytvořte program, který bude umět:

* Generovat parametry RSA (soukromý a veřejný klíč)
* Šifrovat
* Dešifrovat
* Faktorizovat
    * nejdříve se provede triviální dělení pro "prvních 1 000 000 čísel"
    * v případě neúspěchu se vykoná "efektivnější faktorizační metoda"
    * celková doba faktorizace pro všechna čísla "do 96 bitů" bude "nejvýše 120s"
* Dále se vyžaduje vytvoření dokumentace:
    * pečlivě zdokumentujte vaší implementaci
    * v případě faktorizace dále popište vámi zvolenou metodu a vysvětlete, proč jste tuto metodu zvolili

## Požadavky

Program implementujte v jazyce C/C++. Pro implementaci a faktorizaci RSA můžete použít knihovnu pro práci s velkými 
čísly GMP, používejte ji však "pouze" pro základní operace s velkými čísly jako je například násobení nebo 
modulo. Je zakázáno používat tyto [operace](https://gmplib.org/manual/Number-Theoretic-Functions.html#Number-Theoretic-Functions) a další pokočilé operace. Pro generování náhodných čísel však  operace zajištující vygenerování náhodného čísla, které jsou implementovany v knihovně GMP, použít můžete. Při generování čísla o n-bitech musí mít bit na n-tém bitě hodnotu 1. Program bude napsán bezpečně a volně dostupné statické analyzátory (`cppcheck`, `clang/scan-build`) a sanitizéry (`-fsanitize=address,undefined`) nenajdou ve vašem zdrojovém kódu závažné problémy.

## Testování a hodnocení

* Generování klíčů (2.6b)
    * vstup: ./kry -g B 
    * výstup: P Q N E D
* Šifrování (0.4b)
    * vstup: ./kry -e E N M
    * výstup: C
* Dešifrování (0.4b)
    * vstup: ./kry -d D N C
    * výstup: M
* Prolomení RSA (2.6b)
    * vstup: ./kry -b N
        * Bezpečnost řešení (1b)
        * Statická analýza (0.3b)
        * Sanitizéry (0.7b)
    * výstup: P

### Vysvětlivky

* B ... požadovaná velikost veřejného modulu v bitech (např. 1024)
* P ... prvočíslo (při generování náhodné)
* Q ... jiné prvočíslo (při generování náhodné)
* N ... veřejný modulus
* E ... veřejný exponent (většinou 3)
* D ... soukromý exponent
* M ... otevřená zpráva (číslo, nikoli text)
* C ... zašifrovaná zpráva (číslo, nikoli text)

* všechna čísla na vstupu i výstupu (kromě B) jsou hexadecimální, začínají "0x"
* výstup končí novým řádkem
* v případě, že danou funkci neimplementujete, program skončí bez výstupu

### Příklad

* vstup: `./kry -b 0x397a52dccd55d39823964ae25`
* výstup: `0x16098b2cd`

## Vypracování a odevzdání projektu

Projekt je možné vypracovávat na serveru merlin.fit.vutbr.cz, kde je nahrána knihovna GMP. Zde bude také váš program testován a hodnocen.

**Projekt odevzdávejte do Moodlu.**

Projekt se odevzdává jako archiv "xloginXX.zip" (xloginXX nahraďte vaším loginem), který obsahuje všechny zdrojové soubory *.c nebo *.cpp (případně i *.h pokud máte), soubor Makefile a PDF soubor s dokumentací. Neodevzdávejte binárky! Makefile přeloží a vytvoří spustitelný soubor "./kry". "Testování bude probíhat na serveru merlin.fit.vutbr.cz". Dodržujte zásady správného programování (v opačném případě si vyhrazujeme právo snížit bodové hodnocení). Každá část zadání se bude hodnotit nezávisle. Výstupy vašich programů se budou kontrolovat automaticky, proto prosím dodržujte předepsaný formát.

**POZOR! Projekt řešte samostatně, jakékoliv opisování povede k disciplinární komisi a k hodnocení 0b. I nekompletní řešení se hodnotí kladně.**

## Dodatek

Pokud se vám při překladu objeví chyba: `/usr/local/include/c++/4.9.3/cstddef:51:11: error: ::max_align_t has not been declared;: using ::max_align_t;`

Pak vložte do svého zdrojového kódu `#include <cstddef>` příp. `#include <stddef.h>` . Tím se problém vyřeší.

Další možný problém s překladem lze vyřešit pomocí `-L/usr/local/lib64/`

## Konzultace

Konzultace k projektu poskytuje: Marek Tamaškovič - itamaskovic@fit.vutbr.cz
