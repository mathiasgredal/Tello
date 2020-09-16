## Logbog, uge 37

I denne uge arbejdede vi ikke på droneprojektet, men i stedet kiggede vi på hvordan man kunne bruge biblioteker i c++ via. codeblocks og cmake.

Fordele og ulemper ved forskellige bibliotekstyper:

Header:

- Fordele
  - Simpelt
  - Næsten ingen konfiguration
- Ulemper
  - Mere linkertid
  - Større objektfiler

Statisk:

- Fordele
  - Man skal ikke sende biblioteket med programmet
  - Mindre størrelse programfiler, da man kan trimme kode fra, som ikke bliver brugt, ved kompileringstid
- Ulemper
  - Højere linker og kompileringstider
  - Fylder mere på harddisken hvis mange programmer bruger det samme bibliotek

Dynamisk:

- Fordele
  - Kan bruges af flere programmer
  - Programmer fylder mindre i sig selv, dog så fylder de mere end statiske biblioteker, hvis der kun er et program som bruger biblioteket
  - Biblioteket kan få sikkerhedsopdateringer uden programmet skal opdateres
- Ulemper
  - Mere opsætning i forhold til at få sendt det dynamiske bibliotek sammen med brugeren
  - Langsommere programstart