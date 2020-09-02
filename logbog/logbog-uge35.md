## Logbog, uge 35

Reparerede den indbyggede terminal i brugerfladen, da der var problemer med biblioteket. Jeg endte med at slå autocomplete fra, hvilket fiksede problemet, dog så fandt jeg aldrig ud af hvorfor at autocomplete ødelagde funktionaliteten af programmet. 

UDP køen var slået fra, under skiftet fra boost.asio til libuv. Nu er den blevet slået til igen.

Der blev også udviklet videre på den grafiske del af programmet. Man kan nu se UDP køen, noget af telemetrien bliver afkodet korrekt samt at man kan udskrive udp svar i terminalen, dog så er jeg ikke helt sikker på om dette er trådsikkert.

Der er nogle forskellige variabler til at tjekke om der er forbindelse til dronen og om videoforbindelsen er tændt. Den normale måde dette bliver tjekket i koden er om video og udp server pointeren er nullptr. 

Datastrukturen er der til timeout tjek, men det er ikke blevet implementeret, da det ikke er kritisk.

